#include <string>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <vector>
#include <map>

#include "../../include/raytracer/aabb.h"
#include "../../include/raytracer/Ray.h"
#include "../../include/objects/Object.h"



#include "../../include/raytracer/BVH.h"

/*

BVH - Bounding Volume Hierarchy

The BVH will never store triangles or materials on their own--those belong to the Ray Tracer.
The only thing owned by the BVH are its nodes and bounding boxes.

*/

// TriangleData
    TriangleData::TriangleData() {
        tri = 0;
        mesh = nullptr;
        mat = nullptr;
        interpnorms = false;
    }

    TriangleData::TriangleData(int t, Mesh* m, BaseMaterial* mm, bool i) {
        tri = t;
        mesh = m;
        mat = mm;
        interpnorms = i;
    }

    TriangleData::TriangleData(const TriangleData& o) {
        tri = o.tri;
        mesh = o.mesh;
        mat = o.mat;
        interpnorms = o.interpnorms;
    }

    Triangle3 TriangleData::get() {
        return mesh->makeTriangle(tri);
    }

    bool TriangleData::operator<(TriangleData& o) {
        Triangle3 a = get();
        Triangle3 b = o.get();
        if (a.centroid() != b.centroid()) return a.centroid() < b.centroid();
        if (tri != o.tri) return tri < o.tri;
        if (interpnorms != o.interpnorms) return interpnorms < o.interpnorms;
        return false;
    }

    std::string TriangleData::to_string(bool nofluff) {
        
        std::string res = "TriangleData[" + std::to_string(tri) + ", " + std::to_string((unsigned long long)(mesh));
        if (!nofluff) res = res + ", " + get().to_string();
        if (!nofluff) res = res + ", " + mat->to_string() + ", " + std::to_string(interpnorms);
        res = res + "]";
        return res;
    }

// BVHNode

    bool BVHNode::isObject() {
        return isObj;
    }

    bool BVHNode::isBox() {
        return !isObj;
    }

    BVHNode::BVHNode() {

    }

    BVHNode::BVHNode(const BVHNode& o) {
        box = AABB(o.box);
        obj = TriangleData(o.obj);

        L = o.L;
        R = o.R;
        splitAxis = o.splitAxis;
        splitValue = o.splitValue;
        isObj = o.isObj;
    }

    BVHNode::BVHNode(int o, BaseMaterial* mat, Mesh* mesh, bool interp) {
        obj = TriangleData{o, mesh, mat, interp};
        box = AABB();
        isObj = true;
    }

    BVHNode::BVHNode(AABB aabb) {
        box = AABB(aabb);
        obj = TriangleData{0, nullptr, nullptr, 0};
        isObj = false;
    }

    BVHNode::~BVHNode() {
        if (L) delete L;
        if (R) delete R;
    }

    int BVHNode::size() {
        int res = 1;
        if (L) res += L->size();
        if (R) res += R->size();
        return res;
    }

    // Intersection with the box, or with the internal object.
    IntersectionPoint BVHNode::simpleIntersection(Ray r) {
        if (!isObject()) {
            float time = box.IntersectRay(r);
            return IntersectionPoint(nullptr, time, NILVEC3, Vector2(0, 0));
        }
        else {
            Triangle3 trix = obj.get();
            float time = INF;
            
            Vector3 normal(NILVEC3);
            Vector2 uv(0, 0);

            if (trix.N * r.slope > 0) time = INF;
            else {
                time = trix.intersectionTime(r.toLine());
                if (std::isnan(time)) time = INF;
                else if (time < 0) time = INF;
                else {
                    Vector3 normal(trix.normal());
                    Vector3 pp = r.get(time);

                    Vector3 bary = trix.bary(pp);

                    if (obj.interpnorms) normal = obj.mesh->getInterpolatedNormal_bary(bary, obj.tri, trix);

                    Vector2 uv = obj.mesh->getUV_bary(bary, obj.tri, trix);

                    return IntersectionPoint(obj.mat, time, normal, uv);
                }
            }
            return IntersectionPoint(nullptr, time, normal, uv);
        }
    }

    // Intersection point with the first Triangle3 contained in the subtree.
    IntersectionPoint BVHNode::IntersectRay(Ray r) {
        if (isObject()) {
            // std::cout << "NODE " << (unsigned long long)(this) << " = " << isObject() << " " << r.to_string() << "\n";
            auto ip = simpleIntersection(r);
            // std::cout << "??? " << ip.to_string() << "\n";
            return ip;
        }

        IntersectionPoint ip;

        IntersectionPoint confirmation = simpleIntersection(r);
        // std::cout << confirmation.to_string() << "\n";
        if (confirmation.time != INF) {
            IntersectionPoint LI = L->IntersectRay(r);
            IntersectionPoint RI = R->IntersectRay(r);
            if (LI.time < ip.time && LI.time > 0) ip = LI;
            if (RI.time < ip.time && RI.time > 0) ip = RI;
        }
        return ip;
    }

// BVH

    BVH::BVH() {
    }

    BVH::BVH(BVHNode* n) { 
        root = n; 
    }
    void BVH::inorder(std::vector<BVHNode*>& v, BVHNode* x) {
        if (!x) return;
        inorder(v, x->L);
        v.push_back(x);
        inorder(v, x->R);
    }

    std::vector<BVHNode*> BVH::traverse() {
        std::vector<BVHNode*> allnodes;
        inorder(allnodes, root);
        return allnodes;
    }

    BVH::~BVH() {
        if (root) delete root;
    }

    IntersectionPoint BVH::IntersectRay(Ray r) {
        auto ip = root->IntersectRay(r);
        // std::cout << ip.to_string() << "\n";
        return ip;
    }