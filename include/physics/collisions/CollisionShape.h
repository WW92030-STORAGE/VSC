#ifndef COLLISIONSHAPEXXXXXX_VSC_PHYC
#define COLLISIONSHAPEXXXXXX_VSC_PHYC

#include <vector>
#include <string>
#include <algorithm>
#include "../../utils/Math.h"
#include "../../utils/geometry/Shapes.h"
#include "../../objects/Mesh.h"
#include "BoundingVolume.h"

struct CollisionShape {
    Vector3 position = Vector3(0, 0, 0);
    CollisionShape() {
    }

    CollisionShape(Vector3 v) {
        position = v;
    }

    virtual Mesh getMesh() {
        return Mesh();
    }

    virtual std::string to_string() {
        return "CollisionShape[" + position.to_string() + "]";
    }
};

struct CollisionSphere : public CollisionShape {
    float radius = 1;

    CollisionSphere() : CollisionShape() {
        radius = 1;
    }

    CollisionSphere(Vector3 p, float r = 1) : CollisionShape(p) {
        radius = r;
    }

    virtual Mesh getMesh() {
        Mesh m = icosphere(radius, 1);
        m.Trans(Transform(position));
        return m;
    }

    virtual std::string to_string() {
        return "CollisionSphere[" + position.to_string() + ", " + std::to_string(radius) + "]";
    }
};

struct CollisionBox : public CollisionShape {
    Vector3 halfrad = Vector3(0.5, 0.5, 0.5); 
    Quaternion basis = Quaternion::eye();

    CollisionBox() : CollisionShape() {

    }

    CollisionBox(Vector3 p, Vector3 hr = Vector3(0.5, 0.5, 0.5), Quaternion b = Quaternion::eye()) : CollisionShape(p) {
        halfrad = hr;
        basis = b;
    }

    virtual Mesh getMesh() {
        Mesh m = rectprism(halfrad * 2);
        m.Trans(Transform(position, basis.toRotation()));
        return m;
    }

    virtual std::string to_string() {
        return "CollisionBox[" + position.to_string() + ", " + halfrad.to_string() + ", " + basis.to_string() + "]";
    }

    std::vector<Vector3> verts() {
        std::vector<Vector3> v(8);
        constexpr int dx[8] = {1, -1, 1, -1, 1, -1, 1, -1};
        constexpr int dy[8] = {1, 1, -1, -1, 1, 1, -1, -1};
        constexpr int dz[8] = {1, 1, 1, 1, -1, -1, -1, -1};
        for (int i = 0; i < 8; i++) {
            v[i] = position + basis(Vector3(halfrad.x * dx[i], halfrad.y * dy[i], halfrad.z * dz[i]));
        }
        return v;
    }
};

static BoundingSphere CollisionSphereToBoundingSphere(CollisionSphere& sphere) {
    return BoundingSphere(sphere.position, sphere.radius);
}

static BoundingSphere CollisionBoxToBoundingSphere(CollisionBox& box) {
    return BoundingSphere(box.position, box.halfrad.length());
}

static BoundingAABB CollisionSphereToBoundingAABB(CollisionSphere& sphere) {
    float d = sphere.radius;
    return BoundingAABB(sphere.position, Vector3(d, d, d));
}

static BoundingAABB CollisionBoxToBoundingAABB(CollisionBox& box) {
    Vector3 inferior = box.position;
    constexpr int dx[8] = {1, -1, 1, -1, 1, -1, 1, -1};
    constexpr int dy[8] = {1, 1, -1, -1, 1, 1, -1, -1};
    constexpr int dz[8] = {1, 1, 1, 1, -1, -1, -1, -1};
    for (int i = 0; i < 8; i++) {
        inferior = max(inferior, box.basis(Vector3(box.halfrad.x * dx[i], box.halfrad.y * dy[i], box.halfrad.z * dz[i])));
    }
    return BoundingAABB(box.position, inferior);
}

#endif