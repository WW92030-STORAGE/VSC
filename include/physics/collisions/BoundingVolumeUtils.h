#ifndef BOUNDINGVOLUMEUTILS_VSC_PHYC
#define BOUNDINGVOLUMEUTILS_VSC_PHYC

#include <string>
#include <vector>

#include "../../utils/Math.h"
#include "BoundingVolume.h"

static BoundingSphere growSphere(BoundingSphere sphere, Vector3 p) {
    Vector3 disp = (sphere.position - p);
    float dist = disp.length();
    if (dist < sphere.radius) return sphere;

    // The new sphere spans the diameter from (p) to (sphere.position + disp.normalized() * sphere.radius)
    // its vector is (disp) + (disp.normalized() * sphere.radius)
    Vector3 diameter = disp + disp.normalized() * sphere.radius;

    return BoundingSphere(p + diameter * 0.5, diameter.length() * 0.5);
} 

static BoundingSphere fromDiameter(Vector3 a, Vector3 b) {
    Vector3 center = (a + b) * 0.5;

    return BoundingSphere(center, (b - center).length());
}

#endif