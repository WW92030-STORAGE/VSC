#ifndef BOUNDINGVOLUMETESTS_VSC_PHYC
#define BOUNDINGVOLUMETESTS_VSC_PHYC

#include <string>
#include <vector>

#include "../../utils/Math.h"
#include "BoundingVolume.h"
#include "BoundingVolumeUtils.h"

// Tests on bounding volumes. UNUSED -- WILL BE DEPRECATED MOST LIKELY

bool testBVCollision(BoundingAABB x1, BoundingAABB x2) {
    return x1.overlaps(&x2);
    return true;
}

bool testBVCollision(BoundingSphere x1, BoundingSphere x2) {
    return x1.overlaps(&x2);
}

bool testBVCollision(BoundingOBB a, BoundingOBB b) {
    return a.overlaps(&b);
}

#endif