#ifndef BOUNDINGVOLUMEHIERARCHY_VSC_PHYC
#define BOUNDINGVOLUMEHIERARCHY_VSC_PHYC

#include "BoundingVolume.h"
#include "BoundingVolumeTests.h"

/*

BVH system and system for storing potential collisions

*/

struct DetectedCollision {
    RigidBody* bodies[2] = {0, 0};
};

// BV must extend BoundingVolume
template <class BV> 
struct TrueBVHNode {
    BVHNode* L;
    BVHNode* R;
    BV volume;
    RigidBody* body = 0;

    bool isLeaf() const { 
        return body;
    }

    
};



#endif