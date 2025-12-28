#ifndef BOUNDINGVOLUME_VSC_PHYC
#define BOUNDINGVOLUME_VSC_PHYC

#include <string>

#include "../../utils/Math.h"

struct BoundingVolume {
    Vector3 position = Vector3(0, 0, 0);

    BoundingVolume(Vector3 p);

    virtual std::string to_string();
};

struct BoundingSphere : public BoundingVolume {
    float radius = 1;

    BoundingSphere(Vector3 p, float r);

    virtual std::string to_string();
};

struct BoundingAABB : BoundingVolume {
    Vector3 halfrad = Vector3(1, 1, 1); // half-distance along each dimension

    BoundingAABB(Vector3 p, Vector3 r);

    virtual std::string to_string();
};

struct BoundingOBB : BoundingVolume {
    Quaternion rotation = Quaternion::eye();
    Vector3 halfrad = Vector3(1, 1, 1); // half-distance along each dimension

    BoundingOBB(Vector3 p, Quaternion q, Vector3 r);

    virtual std::string to_string();
};

// Static methods

static BoundingOBB fromAABB(BoundingAABB aabb) {
    BoundingOBB res = {aabb.position, Quaternion::eye(), aabb.halfrad};
    return res;
}

#endif