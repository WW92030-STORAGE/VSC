#include <string>
#include <algorithm>

#include "../../../include/utils/Math.h"



#include "../../../include/physics/collisions/BoundingVolume.h"
#include "../../../include/physics/collisions/BoundingVolumeCreation.h"

// BoundingVolume

BoundingVolume::BoundingVolume() {

}

BoundingVolume::BoundingVolume(Vector3 p) {
    position = p;
}

std::string BoundingVolume::to_string() {
    return "BoundingVolume[" + position.to_string() + "]";
}

bool BoundingVolume::overlaps(const BoundingVolume* other)  const {
    return 0;
}

void BoundingVolume::expand(float f) {

}

void BoundingVolume::merge(BoundingVolume* a, BoundingVolume* b) {
    
}

float BoundingVolume::getSize() {
    return 0;
}

float BoundingVolume::getGrowth(const BoundingVolume& other) {
    return 0;
}

// BoundingSphere

BoundingSphere::BoundingSphere() : BoundingVolume() {
    radius = 1;
}

BoundingSphere::BoundingSphere(Vector3 p, float r = 1.0) : BoundingVolume(p) {
    radius = r;
}

std::string BoundingSphere::to_string() {
    return "BoundingSphere[" + position.to_string() + ", " + std::to_string(radius) + "]";
}

bool BoundingSphere::overlaps(const BoundingSphere* other) const {
    auto pp = position;
    Vector3 disp = pp - other->position;
    float rsum = radius + other->radius;
    return disp.normsquared() <= rsum * rsum;
}

void BoundingSphere::expand(float f) {
    radius = std::max(0.0f, radius + f);
}

void BoundingSphere::merge(BoundingSphere* a, BoundingSphere* b) {
    Vector3 disp = (b->position - a->position);
    Vector3 disp_n = disp.normalized();

    Vector3 pa = a->position - disp_n * a->radius;
    Vector3 pb = b->position + disp_n * b->radius;

    position = (pa + pb) * 0.5;
    radius = (pb - position).length() + BV_MERGE_MARGIN;
}

float BoundingSphere::getSize() {
    constexpr float coeff = M_PI * 4.0 / 3.0;
    return coeff * radius * radius * radius;
}

float BoundingSphere::getGrowth(const BoundingSphere& ss) {
    BoundingSphere ns;
    BoundingSphere sss = ss;
    ns.merge(this, &sss);
    float rr = ns.radius;
    return rr * rr - radius * radius;
}


// BoundingAABB

BoundingAABB::BoundingAABB() : BoundingVolume() {
    halfrad = VEC3_ONE;
}
BoundingAABB::BoundingAABB(Vector3 p, Vector3 r = VEC3_ONE) : BoundingVolume(p) {
    halfrad = r;
}

std::string BoundingAABB::to_string() {
    return "BoundingAABB[" + position.to_string() + ", " + halfrad.to_string() + "]";
}

bool BoundingAABB::overlaps(const BoundingAABB* other) const {
    Vector3 op(other->position);
    Vector3 oh(other->halfrad);

    auto pp = position;
    auto hr = halfrad;

    for (int i = 0; i < 3; i++) {
        if (fabs(pp.get(i) - op.get(i)) > hr.get(i) + oh.get(i)) return false;
    }
    return true;
}

void BoundingAABB::expand(float f) {
    halfrad.x = std::max(0.0f, halfrad.x + f);
    halfrad.y = std::max(0.0f, halfrad.y + f);
    halfrad.z = std::max(0.0f, halfrad.z + f);
}

void BoundingAABB::merge(BoundingAABB* a, BoundingAABB* b) {
    Vector3 inferior = min(a->position - a->halfrad, b->position - b->halfrad);
    Vector3 superior = max(a->position + a->halfrad, b->position + b->halfrad);

    position = (inferior + superior) * 0.5;
    halfrad = superior - position + Vector3(BV_MERGE_MARGIN, BV_MERGE_MARGIN, BV_MERGE_MARGIN);
}

float BoundingAABB::getSize() {
    return 8.0 * halfrad.x * halfrad.y * halfrad.z;
}

float BoundingAABB::getGrowth(const BoundingAABB& bb) {
    /*
    
    o.x * o.y + o.y * o.z + o.z * o.x - x * y - y * z - z * x
    
    */

    BoundingAABB bbb = bb;
    BoundingAABB other;
    other.merge(this, &bbb);

    float sa = halfrad.x * (halfrad.y + halfrad.z) + halfrad.y * halfrad.z;
    float osa = other.halfrad.x * (other.halfrad.y + other.halfrad.z) + other.halfrad.y * other.halfrad.z;

    return osa - sa;

}