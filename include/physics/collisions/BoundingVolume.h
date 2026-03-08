#ifndef BOUNDINGVOLUME_VSC_PHYC
#define BOUNDINGVOLUME_VSC_PHYC

#include <string>

#include "../../utils/Math.h"

/*

Bounding volumes can be used for both fine and coarse collision phases

WARNING - BoundingOBB is not used for now. It is not implemented and should not be used.

*/

struct BoundingVolume {
    Vector3 position = Vector3(0, 0, 0);

    BoundingVolume();
    BoundingVolume(Vector3 p);

    virtual std::string to_string();

    virtual bool overlaps(const BoundingVolume* other) const;

    virtual void expand(float f);

    virtual void merge(BoundingVolume* a, BoundingVolume* b);

    virtual float getSize();
    
    virtual float getGrowth(const BoundingVolume& other);
};

struct BoundingSphere : public BoundingVolume {
    float radius = 1;

    BoundingSphere();
    BoundingSphere(Vector3 p, float r);

    virtual std::string to_string();

    virtual bool overlaps(const BoundingSphere* other) const;

    virtual void expand(float f);

    virtual void merge(BoundingSphere* a, BoundingSphere* b);

    virtual float getSize();

    virtual float getGrowth(const BoundingSphere& other);
};

struct BoundingAABB : BoundingVolume {
    Vector3 halfrad = Vector3(1, 1, 1); // half-distance along each dimension

    BoundingAABB();
    BoundingAABB(Vector3 p, Vector3 r);

    virtual std::string to_string();

    virtual bool overlaps(const BoundingAABB* other) const;

    virtual void expand(float f);

    virtual void merge(BoundingAABB* a, BoundingAABB* b);

    virtual float getSize();

    virtual float getGrowth(const BoundingAABB& other);
};



#endif