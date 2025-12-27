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

    TriangleData();

    TriangleData(int t, Mesh* m, BaseMaterial* mm, bool i);

    TriangleData(const TriangleData& o);

    Triangle3 get();

    bool operator<(TriangleData& o);

    std::string to_string(bool nofluff = false);
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



    bool isObject();

    bool isBox();

    BVHNode();

    BVHNode(const BVHNode& o);

    BVHNode(int o, BaseMaterial* mat, Mesh* mesh, bool interp);

    BVHNode(AABB aabb);

    ~BVHNode();

    int size();

    // Intersection with the box, or with the internal object.
    IntersectionPoint simpleIntersection(Ray r);

    // Intersection point with the first Triangle3 contained in the subtree.
    IntersectionPoint IntersectRay(Ray r);
};

struct BVH {
    BVHNode* root = nullptr;
    int N;

    BVH();

    BVH(BVHNode* n);
    void inorder(std::vector<BVHNode*>& v, BVHNode* x);

    std::vector<BVHNode*> traverse();

    ~BVH();

    IntersectionPoint IntersectRay(Ray r);
};

// Find the SA heuristic value for the ddv array (sorted by centroid coordinate), the AABBs of the left and right splits, and the index splitting to [0 ... i] [i + 1 ... N]
static float heuristic(int N, AABB left, AABB right, int index) {
    return left.area() * index + right.area() * (N - index);
}

static void awaken(BVHNode* node, std::vector<TriangleData> v) {
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
    float infp = aabb.inferior.get(0);

    for (int d = 0; d < 3; d++) {
        float minp = -INF;
        float maxp = INF;

        minp = aabb.inferior.get(d);
        maxp = aabb.superior.get(d);
        /*
        for (int i = 0; i < n; i++) {
            AABB box(v[i].get());
            minp = fmin(box.inferior.get(d), minp);
            maxp = fmax(box.superior.get(d), maxp);
        }
        */
        if (maxp - minp > md) {
            md = maxp - minp;
            infp = minp;
            axis = d;
        }
    }

    std::map<std::pair<float, int>, TriangleData> dds;
    std::vector<std::pair<float, TriangleData>> ddv;
    int index = 0;
    for (auto data : v) dds.insert({{data.get().centroid().get(axis), index++}, data});
    for (auto data : v) ddv.push_back({data.get().centroid().get(axis), data});
    int N = ddv.size();

    const bool SAH = false;

    if (SAH) {
    // Surface area partition

    std::vector<AABB> prefix(N + 1);
    prefix[0 ] = AABB(ddv[0].second.get());
    std::vector<AABB> suffix(N + 1);
    suffix[N] = AABB(ddv[N - 1].second.get());

    for (int i = 0; i < N; i++) {
        prefix[i + 1] = prefix[i].merge(AABB(ddv[i].second.get()));
        suffix[N - i - 1] = suffix[N - i].merge(AABB(ddv[N - i - 1].second.get()));
    }

    int mindex = 0;
    float SA = heuristic(N, prefix[mindex], suffix[mindex], 0);

    for (int index = 1; index < N - 1; index++) {
        float test = heuristic(N, prefix[index], suffix[index], index);
        if (test < SA) {
            SA = test;
            mindex = index;
        }
    }

    for (int index = 0; index < N; index++) {
        auto data = ddv[index];
        if (index <= mindex) LEFT.push_back(data.second);
        else RIGHT.push_back(data.second);
    }

    } else {

    // Median index partition

    index = 0;
    for (auto data : ddv) {
        if (index < n / 2) LEFT.push_back(data.second);
        else RIGHT.push_back(data.second);
        index++;
    }

    }

    BVHNode* L = new BVHNode();
    BVHNode* R = new BVHNode();

    node->L = L;
    node->R = R;

    awaken(L, LEFT);
    awaken(R, RIGHT);
}

static BVH* create(std::vector<Mesh*> meshes, std::vector<BaseMaterial*> materials, std::vector<bool> NormInterps) {
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