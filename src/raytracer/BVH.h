#ifndef BVH_EXT
#define BVH_EXT

#include <string>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <vector>
#include <map>

#include "aabb.h"
#include "Ray.h"
#include "../objects/Object.h"

/*

BVH - Bounding Volume Hierarchy

The BVH will never store triangles or materials on their own--those belong to the Ray Tracer.
The only thing owned by the BVH are its nodes and bounding boxes.

*/

struct TriangleData {
    int tri = 0;
    Mesh* mesh = nullptr;
    BaseMaterial* mat = nullptr;
    bool interpnorms = false;

    TriangleData() {
        tri = 0;
        mesh = nullptr;
        mat = nullptr;
        interpnorms = false;
    }

    TriangleData(int t, Mesh* m, BaseMaterial* mm, bool i) {
        tri = t;
        mesh = m;
        mat = mm;
        interpnorms = i;
    }

    TriangleData(const TriangleData& o) {
        tri = o.tri;
        mesh = o.mesh;
        mat = o.mat;
        interpnorms = o.interpnorms;
    }

    inline Triangle3 get() {
        return mesh->makeTriangle(tri);
    }

    inline bool operator<(TriangleData& o) {
        Triangle3 a = get();
        Triangle3 b = o.get();
        if (a.centroid() != b.centroid()) return a.centroid() < b.centroid();
        if (tri != o.tri) return tri < o.tri;
        if (interpnorms != o.interpnorms) return interpnorms < o.interpnorms;
        return false;
    }

    inline std::string to_string(bool nofluff = false) {
        
        std::string res = "TriangleData[" + std::to_string(tri) + ", " + std::to_string((unsigned long long)(mesh));
        if (!nofluff) res = res + ", " + get().to_string();
        if (!nofluff) res = res + ", " + mat->to_string() + ", " + std::to_string(interpnorms);
        res = res + "]";
        return res;
    }
};

class BVHNode {
    public:
    AABB box;
    TriangleData obj;

    BVHNode* L = nullptr;
    BVHNode* R = nullptr;
    int splitAxis = 0;
    float splitValue = 0;
    bool isObj = true;



    bool isObject() {
        return isObj;
    }

    bool isBox() {
        return !isObj;
    }

    BVHNode() {

    }

    BVHNode(const BVHNode& o) {
        box = AABB(o.box);
        obj = TriangleData(o.obj);

        L = o.L;
        R = o.R;
        splitAxis = o.splitAxis;
        splitValue = o.splitValue;
        isObj = o.isObj;
    }

    BVHNode(int o, BaseMaterial* mat, Mesh* mesh, bool interp) {
        obj = TriangleData{o, mesh, mat, interp};
        box = AABB();
        isObj = true;
    }

    BVHNode(AABB aabb) {
        box = AABB(aabb);
        obj = TriangleData{0, nullptr, nullptr, 0};
        isObj = false;
    }

    ~BVHNode() {
        if (L) delete L;
        if (R) delete R;
    }

    inline int size() {
        int res = 1;
        if (L) res += L->size();
        if (R) res += R->size();
        return res;
    }

    // Intersection with the box, or with the internal object.
    inline IntersectionPoint simpleIntersection(Ray r) {
        if (!isObject()) {
            float time = box.IntersectRay(r);
            return IntersectionPoint(nullptr, time, NILVEC3, Vector2(0, 0));
        }
        else {
            Triangle3 trix = obj.get();
            float time = trix.intersectionTime(r.toLine());
            
            Vector3 normal(NILVEC3);
            Vector2 uv(0, 0);

            if (trix.N * r.slope > 0) time = INF;
            else if (std::isnan(time)) time = INF;
            else if (time < 0) time = INF;
            else {
                Vector3 normal(trix.normal());
                Vector3 pp = r.get(time);

                if (obj.interpnorms) normal = obj.mesh->getInterpolatedNormal(pp, obj.tri);

                Vector2 uv = obj.mesh->getUV(pp, obj.tri);

                return IntersectionPoint(obj.mat, time, normal, uv);
            }
            return IntersectionPoint(nullptr, time, normal, uv);
        }
    }

    // Intersection point with the first Triangle3 contained in the subtree.
    inline IntersectionPoint IntersectRay(Ray r) {
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
};

struct BVH {
    BVHNode* root = nullptr;

    BVH() {
    }

    BVH(BVHNode* n) { 
        root = n; 
    }

    inline void inorder(std::vector<BVHNode*>& v, BVHNode* x) {
        if (!x) return;
        inorder(v, x->L);
        v.push_back(x);
        inorder(v, x->R);
    }

    inline std::vector<BVHNode*> traverse() {
        std::vector<BVHNode*> allnodes;
        inorder(allnodes, root);
        return allnodes;
    }

    ~BVH() {
        if (root) delete root;
    }

    inline IntersectionPoint IntersectRay(Ray r) {
        auto ip = root->IntersectRay(r);
        // std::cout << ip.to_string() << "\n";
        return ip;
    }
};

inline void awaken(BVHNode* node, std::vector<TriangleData> v) {
    // std::cout << " SIZE " << v.size() << "\n";
    if (v.size() <= 1) {
        node->obj = TriangleData(v[0]);
        // std::cout << "A " << node->obj.to_string() << "\n";
        node->isObj = true;
        return;
    }
    node->isObj = false;
    int n = v.size();

    // Set the AABB of the thing.

    AABB aabb(v[0].get());
    for (auto i : v) aabb = aabb.merge(AABB(i.get()));
    node->box = aabb;

    std::vector<TriangleData> LEFT;
    std::vector<TriangleData> RIGHT;

    // Partition by longest axis

    int axis = 0;
    float md = 0;

    for (int d = 0; d < 3; d++) {
        float minp = -INF;
        float maxp = INF;
        for (int i = 0; i < n; i++) {
            AABB box(v[i].get());
            minp = fmin(box.inferior.get(d), minp);
            maxp = fmax(box.superior.get(d), maxp);
        }
        if (maxp - minp > md) {
            md = maxp - minp;
            axis = d;
        }
    }

    std::map<std::pair<float, int>, TriangleData> dds;
    int index = 0;
    for (auto data : v) dds.insert({{data.get().centroid().get(axis), index++}, data});

    index = 0;
    for (auto data : dds) {
        if (index < n / 2) LEFT.push_back(data.second);
        else RIGHT.push_back(data.second);
        index++;
    }

    /*

    // Simpler but less effective partitioning

    std::sort(v.begin(), v.end());

    for (int i = 0; i < n; i++) {
        if (i < n / 2) LEFT.push_back(v[i]);
        else RIGHT.push_back(v[i]);
    }

    */

    // Recursion step

    /*

    std::cout << "BVHNODE ";
    for (auto i : v) std::cout << i.to_string(true) << " ";

    std::cout << "\nLEFT ";
    for (auto i : LEFT) std::cout << i.to_string(true) << " ";
    std::cout << "\nRIGHT ";
    for (auto i : RIGHT) std::cout << i.to_string(true) << " ";

    std::cout << "\n\n";

    */

    BVHNode* L = new BVHNode();
    BVHNode* R = new BVHNode();

    node->L = L;
    node->R = R;

    awaken(L, LEFT);
    awaken(R, RIGHT);
}

inline BVH* create(std::vector<Mesh*> meshes, std::vector<BaseMaterial*> materials, std::vector<bool> NormInterps) {
    BVHNode* node = new BVHNode();
    BVHNode* nn = node;

    std::vector<TriangleData> data;
    for (int i = 0; i < meshes.size(); i++) {
        Mesh* mesh = meshes[i];
        for (int j = 0; j < mesh->size; j++) {
            data.push_back(TriangleData{j, mesh, materials[i], NormInterps[i]});
            // std::cout << "S " << data[j].to_string() << "\n";
        }
    }

    std::sort(data.begin(), data.end());


    awaken(node, data);

    return new BVH{nn};
}

#endif