#ifndef BOUNDINGVOLUME_VSC_PHYC
#define BOUNDINGVOLUME_VSC_PHYC

#include <string>

#include "../../utils/Math.h"

/*

Bounding volumes can be used for both fine and coarse collision phases

WARNING - BoundingOBB is not used for now.

*/

struct BoundingVolume {
    Vector3 position = Vector3(0, 0, 0);

    BoundingVolume();
    BoundingVolume(Vector3 p);

    virtual std::string to_string();

    virtual bool overlaps(const BoundingVolume* other);
};

struct BoundingSphere : public BoundingVolume {
    float radius = 1;

    BoundingSphere();
    BoundingSphere(Vector3 p, float r);

    virtual std::string to_string();

    virtual bool overlaps(const BoundingSphere* other);
};

struct BoundingAABB : BoundingVolume {
    Vector3 halfrad = Vector3(1, 1, 1); // half-distance along each dimension

    BoundingAABB();
    BoundingAABB(Vector3 p, Vector3 r);

    virtual std::string to_string();

    virtual bool overlaps(const BoundingAABB* other);
};

struct BoundingOBB : BoundingVolume {
    Matrix3 rotation = Matrix3::eye();
    Vector3 halfrad = Vector3(1, 1, 1); // half-distance along each dimension

    BoundingOBB(Vector3 p, Matrix3 q, Vector3 r);

    virtual std::string to_string();

    virtual bool overlaps(const BoundingOBB* other);
};

// Static methods

static BoundingOBB fromAABB(BoundingAABB aabb) {
    BoundingOBB res = {aabb.position, Matrix3::eye(), aabb.halfrad};
    return res;
}

#endif