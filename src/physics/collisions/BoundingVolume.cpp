#include <string>
#include <algorithm>

#include "../../../include/utils/Math.h"



#include "../../../include/physics/collisions/BoundingVolume.h"


// BoundingVolume

BoundingVolume::BoundingVolume() {

}

BoundingVolume::BoundingVolume(Vector3 p) {
    position = p;
}

std::string BoundingVolume::to_string() {
    return "BoundingVolume[" + position.to_string() + "]";
}

bool BoundingVolume::overlaps(const BoundingVolume* other) {
    return 0;
}

// BoundingSphere

BoundingSphere::BoundingSphere() : BoundingVolume() {
    radius = 1;
}

BoundingSphere::BoundingSphere(Vector3 p = VEC3_ZERO, float r = 1.0) : BoundingVolume(p) {
    radius = r;
}

std::string BoundingSphere::to_string() {
    return "BoundingSphere[" + position.to_string() + ", " + std::to_string(radius) + "]";
}

bool BoundingSphere::overlaps(const BoundingSphere* other) {
    Vector3 disp = position - other->position;
    float rsum = radius + other->radius;
    return disp.normsquared() <= rsum * rsum;
}

// BoundingAABB

BoundingAABB::BoundingAABB() : BoundingVolume() {
    halfrad = VEC3_ONE;
}
BoundingAABB::BoundingAABB(Vector3 p = VEC3_ZERO, Vector3 r = VEC3_ONE) : BoundingVolume(p) {
    halfrad = r;
}

std::string BoundingAABB::to_string() {
    return "BoundingAABB[" + position.to_string() + ", " + halfrad.to_string() + "]";
}

bool BoundingAABB::overlaps(const BoundingAABB* other) {
    Vector3 op(other->position);
    Vector3 oh(other->halfrad);

    for (int i = 0; i < 3; i++) {
        if (fabs(position.get(i) - op.get(i)) > halfrad.get(i) + oh.get(i)) return false;
    }
    return true;
}


// BoundingOBB

BoundingOBB::BoundingOBB(Vector3 p, Matrix3 q, Vector3 r) : BoundingVolume(p) {
    halfrad = r;
    rotation = q;
}

std::string BoundingOBB::to_string() {
    return "BoundingOBB[" + position.to_string() + ", " + halfrad.to_string() + ", " + rotation.to_string() + "]";
}

bool BoundingOBB::overlaps(const BoundingOBB* other) {
    BoundingOBB a(*this);
    BoundingOBB b(*other);
    float ra, rb;
    Matrix3 R, absR;
    const float epsilon = 0.000001;

    // first we need to convert b into a's space

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) R.set(i, j, a.rotation.getCol(i).dot(b.rotation.getCol(j)));
    }

    Vector3 t = b.position - a.position;
    t = Vector3(t.dot(a.rotation.getCol(0)), t.dot(a.rotation.getCol(1)), t.dot(a.rotation.getCol(2)));


    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) absR.set(i, j, fabs(R.get(i, j)) + epsilon);
    }

    // Axis testing

    for (int i = 0; i < 3; i++) {
        ra = a.halfrad.get(i);
        rb = b.halfrad * absR.getRow(i);
        if (fabs(t.get(i)) > ra + rb) return 0;
    }

    for (int i = 0; i < 3; i++) {
        ra = a.halfrad * absR.getCol(i);
        rb = b.halfrad.get(i);

        if (fabs(t * R.getCol(i)) > ra + rb) return 0;
    }

    // (A, B): (0, 0)
    ra = a.halfrad.get(1) * absR.get(2, 0) + a.halfrad.get(2) * absR.get(1, 0);
    rb = b.halfrad.get(1) * absR.get(0, 2) + b.halfrad.get(2) * absR.get(0, 1);
    if (fabs(t.get(2) * R.get(1, 0) - t.get(1) * R.get(2, 0)) > ra + rb) return 0;

    // (A, B): (0, 1)
    ra = a.halfrad.get(1) * absR.get(2, 1) + a.halfrad.get(2) * absR.get(1, 1);
    rb = b.halfrad.get(0) * absR.get(0, 2) + b.halfrad.get(2) * absR.get(0, 0);
    if (fabs(t.get(2) * R.get(1, 1) - t.get(1) * R.get(2, 1)) > ra + rb) return 0;

    // (A, B): (0, 2)
    ra = a.halfrad.get(1) * absR.get(2, 2) + a.halfrad.get(2) * absR.get(1, 2);
    rb = b.halfrad.get(0) * absR.get(0, 1) + b.halfrad.get(1) * absR.get(0, 0);
    if (fabs(t.get(2) * R.get(1, 2) - t.get(1) * R.get(2, 2)) > ra + rb) return 0;


    // (A, B): (1, 0)
    ra = a.halfrad.get(0) * absR.get(2, 0) + a.halfrad.get(2) * absR.get(0, 0);
    rb = b.halfrad.get(1) * absR.get(1, 2) + b.halfrad.get(2) * absR.get(1, 1);
    if (fabs(t.get(0) * R.get(2, 0) - t.get(2) * R.get(0, 0)) > ra + rb) return 0;

    // (A, B): (1, 1)
    ra = a.halfrad.get(0) * absR.get(2, 1) + a.halfrad.get(2) * absR.get(0, 1);
    rb = b.halfrad.get(0) * absR.get(1, 2) + b.halfrad.get(2) * absR.get(1, 0);
    if (fabs(t.get(0) * R.get(2, 1) - t.get(2) * R.get(0, 1)) > ra + rb) return 0;

    // (A, B): (1, 2)
    ra = a.halfrad.get(0) * absR.get(2, 2) + a.halfrad.get(2) * absR.get(0, 2);
    rb = b.halfrad.get(0) * absR.get(1, 1) + b.halfrad.get(1) * absR.get(1, 0);
    if (fabs(t.get(0) * R.get(2, 2) - t.get(2) * R.get(0, 2)) > ra + rb) return 0;


    // (A, B): (2, 0)
    ra = a.halfrad.get(0) * absR.get(1, 0) + a.halfrad.get(1) * absR.get(0, 0);
    rb = b.halfrad.get(1) * absR.get(2, 2) + b.halfrad.get(2) * absR.get(2, 1);
    if (fabs(t.get(1) * R.get(0, 0) - t.get(0) * R.get(1, 0)) > ra + rb) return 0;

    // (A, B): (2, 1)
    ra = a.halfrad.get(0) * absR.get(1, 1) + a.halfrad.get(1) * absR.get(0, 1);
    rb = b.halfrad.get(0) * absR.get(2, 2) + b.halfrad.get(2) * absR.get(2, 0);
    if (fabs(t.get(1) * R.get(0, 1) - t.get(0) * R.get(1, 1)) > ra + rb) return 0;

    // (A, B): (2, 2)
    ra = a.halfrad.get(0) * absR.get(1, 2) + a.halfrad.get(1) * absR.get(0, 2);
    rb = b.halfrad.get(0) * absR.get(2, 1) + b.halfrad.get(1) * absR.get(2, 0);
    if (fabs(t.get(1) * R.get(0, 2) - t.get(0) * R.get(1, 2)) > ra + rb) return 0;

    return 1;
}
