#include <string>
#include <algorithm>

#include "../../../include/utils/Math.h"



#include "../../../include/physics/collisions/BoundingVolume.h"



// BoundingVolume

BoundingVolume::BoundingVolume(Vector3 p) {
    position = p;
}

std::string BoundingVolume::to_string() {
    return "BoundingVolume[" + position.to_string() + "]";
}

// BoundingSphere

BoundingSphere::BoundingSphere(Vector3 p, float r) : BoundingVolume(p) {
    radius = r;
}

std::string BoundingSphere::to_string() {
    return "BoundingSphere[" + position.to_string() + ", " + std::to_string(radius) + "]";
}

// BoundingAABB

BoundingAABB::BoundingAABB(Vector3 p, Vector3 r) : BoundingVolume(p) {
    halfrad = r;
}

std::string BoundingAABB::to_string() {
    return "BoundingAABB[" + position.to_string() + ", " + halfrad.to_string() + "]";
}

// BoundingOBB

BoundingOBB::BoundingOBB(Vector3 p, Quaternion q, Vector3 r) : BoundingVolume(p) {
    halfrad = r;
    rotation = q;
}

std::string BoundingOBB::to_string() {
    return "BoundingOBB[" + position.to_string() + ", " + halfrad.to_string() + ", " + rotation.to_string() + "]";
}
