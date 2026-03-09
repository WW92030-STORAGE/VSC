#ifndef CONVEXCOLLISION_VSC_PHYS
#define CONVEXCOLLISION_VSC_PHYS

#include <vector>
#include <string>
#include "../../utils/Math.h"
#include "../../objects/Mesh.h"
#include "CollisionShape.h"

bool checkSeparatingAxis(Vector3 v, std::vector<Vector3>& a, std::vector<Vector3>& b);

/*

Collisions between convex objects.

*/

// No pointers are owned.
struct CollisionData {
    CollisionShape* a = 0;
    CollisionShape* b = 0;
    Vector3 normal = Vector3(); // From A into B

    inline bool exists() {
        return a || b;
    }

    std::string to_string() {
        if (!exists()) return "CollisionData[]";
        return "CollisionData[" + a->to_string() + ", " + b->to_string() + " | " + normal.to_string() + "]";
    }
};

CollisionData checkCollision(CollisionSphere& x1, CollisionSphere& x2) {
    CollisionData cd;

    BoundingSphere b1 = CollisionSphereToBoundingSphere(x1);
    BoundingSphere b2 = CollisionSphereToBoundingSphere(x2);

    if (b1.overlaps(&b2)) {
        cd.a = &x1;
        cd.b = &x2;
        cd.normal = (x2.position - x1.position).normalized();
    }
    return cd;
}

CollisionData checkCollision(CollisionBox& x1, CollisionBox& x2) {
    CollisionData cd;
    Matrix3 b1 = x1.basis.toRotation();
    Matrix3 b2 = x2.basis.toRotation();

    constexpr int dx[8] = {1, -1, 1, -1, 1, -1, 1, -1};
    constexpr int dy[8] = {1, 1, -1, -1, 1, 1, -1, -1};
    constexpr int dz[8] = {1, 1, 1, 1, -1, -1, -1, -1};

    std::vector<Vector3> p1 = x1.verts();
    std::vector<Vector3> p2 = x2.verts();

    // Face normals...
    for (int i = 0; i < 3; i++) {
        Vector3 axis = b1.getCol(i);
        if (checkSeparatingAxis(axis, p1, p2)) {
            return cd;
        }

        axis = b2.getCol(i);
        if (checkSeparatingAxis(axis, p1, p2)) {
            return cd;
        }
    }

    // Cross products...
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Vector3 a1 = b1.getCol(i);
            Vector3 a2 = b2.getCol(j);
            Vector3 c = a1.cross(a2);
            if (c == VEC3_ZERO) continue;
            if (checkSeparatingAxis(c, p1, p2)) {
                return cd;
            }
        }
    }
    cd.a = &x1;
    cd.b = &x2;

    return cd;
}

// Separating Axis Test (return true if no collision in that direction)

bool checkSeparatingAxis(Vector3 v, std::vector<Vector3>& a, std::vector<Vector3>& b) {
    v = v.normalized();
    float minA = FLT_MAX;
    float maxA = -1 * FLT_MAX;
    float minB = FLT_MAX;
    float maxB = -1 * FLT_MAX;


    for (auto p : a) {
        Vector3 proj = p.proj(v);
        float dist = proj.dot(v);
        minA = std::min(minA, dist);
        maxA = std::max(maxA, dist);
    }

    for (auto p : b) {
        Vector3 proj = p.proj(v);
        float dist = proj.dot(v);
        minB = std::min(minB, dist);
        maxB = std::max(maxB, dist);
    }
    // std::cout << v.to_string() << " | " << minA << " " << maxA << " | " << minB << " " << maxB << "\n";
    if (std::max(minA, minB) <= std::min(maxA, maxB)) return false;
    return true;
}

#endif