#ifndef CONVEXCOLLISION_VSC_PHYS
#define CONVEXCOLLISION_VSC_PHYS

#include <vector>
#include <array>
#include <string>
#include "../../utils/Math.h"
#include "../../objects/Mesh.h"
#include "CollisionShape.h"

#include <iostream>

bool checkSeparatingAxis(Vector3 v, std::vector<Vector3>& a, std::vector<Vector3>& b);

/*

Collisions between convex objects.

*/

// No pointers are owned.
struct Collision {
    CollisionShape* shapes[2] = {0, 0}; // THE FIRST SHAPE IS THE ONE THAT NEEDS TO MOVE WITH DISPACEMENT (normal) TO NOT INTERSECT.
	RigidBody* bodies[2] = {0, 0};
    Vector3 normal = Vector3(); // From A into B
	Vector3 point = Vector3();

    inline bool exists() {
        return shapes[0];
    }

    std::string to_string() {
        if (!exists()) return "CollisionData[]";
		std::string x0 = shapes[0] ? shapes[0]->to_string() : "NULL";
		std::string x1 = shapes[1] ? shapes[1]->to_string() : "NULL";
        return "CollisionData[shapes=(" + x0 + ", " + x1 + "), N=" + normal.to_string() + ", P=" + point.to_string() + "]";
    }
};

// UTIL METHODS. Each method generates a Collision, a std::vector<Collision>, or some relevant data that is sufficient (when used with the inputs) to gain all information about a collision.

// Generates the penetration normal between a plane and a point, from the perspective of the point (i.e. normal points where the point needs to go to "escape" the box)
// The outside of the plane is considered in the direction of its normal.
Vector3 planePoint(Plane& plane, Vector3& p) {
	float distance = (p - plane.p) * plane.n;
	if (distance > 0) return Vector3();
	return plane.n * -distance;
}

// Generates the standardized collision data between a box and a point, from the perspective of the point (i.e. normal points where the point needs to go to "escape" the box)
Collision boxPoint(CollisionBox& box, Vector3& p) {
	Matrix3 axes = box.basis.toRotation();
	Vector3 relPt = Transform(box.position, box.basis).inv() * p;
	Vector3 normal;
	
	float min_depth = box.halfrad.x - fabs(relPt.x);
	if (min_depth < 0) return Collision();
	normal = axes.xAxis * (relPt.x < 0 ? -1 : 1);

	float depth = box.halfrad.y - fabs(relPt.y);
	if (depth < 0) return Collision();
	else if (depth < min_depth) {
		min_depth = depth;
		normal = axes.yAxis * (relPt.y < 0 ? -1 : 1);
	}

	depth = box.halfrad.z - fabs(relPt.z);
	if (depth < 0) return Collision();
	else if (depth < min_depth) {
		min_depth = depth;
		normal = axes.zAxis * (relPt.z < 0 ? -1 : 1);
	}

	Collision cd;
	cd.normal = normal;
	cd.point = p;
	cd.shapes[0] = &box;
	return cd;
}

// Generates collision data between a sphere and a plane
Collision spherePlane(CollisionSphere& sphere, Plane& p) {
	float verticalDistance = (sphere.position - p.p) * p.n;
	if (verticalDistance > sphere.radius) return Collision();
	Collision cd;
	cd.shapes[0] = &sphere;
	cd.point = sphere.position - p.n * sphere.radius;
	cd.normal = p.n * ((p.p - cd.point) * p.n);
	return cd;
}

// Determines the radius of a box, when projected onto some axis by using some botched dot products
float boxRadius(CollisionBox& box, Vector3& axis) {
	Matrix3 rot = box.basis.toRotation();
	return box.halfrad.x * fabs(axis * rot.xAxis) + box.halfrad.y * fabs(axis * rot.yAxis) + box.halfrad.z * fabs(axis * rot.zAxis);
}

// Generates penetration distance of two boxes along some axis, given two boxes, an axis, and a precomputed displacement between the box centers.
// Done by computing the radii of the boxes along the axis and finding the interval stuff. Negative values indicate no penetration.
float boxPenetrationAxis(CollisionBox& b1, CollisionBox& b2, Vector3& axis, Vector3& disp) {
	float r1 = boxRadius(b1, axis);
	float r2 = boxRadius(b2, axis);
	return r1 + r2 - fabs(disp * axis);
}

// Generates standardized collision data between two boxes, along some axis. The second box is perspective here, first box contains the axis we compare against.
// Disp is the distance between b2 - b1 and pen is the precomputed penetration distance from b1 to b2 (positive if intersecting).
Collision boxVertexToFace(CollisionBox& b1, CollisionBox& b2, Vector3 axis, Vector3 disp, float pen) {
	Matrix3 mat = b2.basis.toRotation();
	Vector3 normal = axis.normalized();
	if (axis * disp > 0) normal = normal * -1;

	Vector3 v = b2.halfrad;
	if (mat.xAxis * normal < 0) v.x = -v.x;
	if (mat.yAxis * normal < 0) v.y = -v.y;
	if (mat.zAxis * normal < 0) v.z = -v.z;

	Collision cd;
	cd.normal = normal * pen;
	cd.point = Transform(b2.position, mat) * v;
	cd.shapes[0] = &b2;
	cd.shapes[1] = &b1;
	return cd;
}

// METHODS ON ENTIRE SHAPES

// SPHERE ON SPHERE COLLISIONS
Collision checkCollision(CollisionSphere& x1, CollisionSphere& x2) {
    Collision cd;

    BoundingSphere b1 = CollisionSphereToBoundingSphere(x1);
    BoundingSphere b2 = CollisionSphereToBoundingSphere(x2);

    if (b1.overlaps(&b2)) {
        cd.shapes[0] = &x1;
        cd.shapes[1] = &x2;
        cd.normal = (x2.position - x1.position).normalized();
		cd.point = x1.position + cd.normal * x1.radius;
    }
    return cd;
}

// BOX ON BOX COLLISIONS
Collision checkCollision(CollisionBox& x1, CollisionBox& x2) {
    Collision cd;
    Matrix3 b1 = x1.basis.toRotation();
    Matrix3 b2 = x2.basis.toRotation();

    Vector3 disp = x2.position - x1.position;

	float minPenetration = FLT_MAX;
	int index = 0;
	int counter = 0;

    // Face normals...
    for (int i = 0; i < 3; i++) {
        Vector3 axis = b1.getCol(i);
		float pen = boxPenetrationAxis(x1, x2, axis, disp);
		std::cout << i << " " << pen << "\n";
        if (pen < 0) return cd;
		if (pen < minPenetration) {
			minPenetration = pen;
			index = counter; 
		}
		counter++;
	}
	for (int i = 0; i < 3; i++) {
        Vector3 axis = b2.getCol(i);
		float pen = boxPenetrationAxis(x1, x2, axis, disp);
		std::cout << i << " " << axis.to_string() << " " << pen << "\n";
        if (pen < 0) return cd;
		if (pen < minPenetration) {
			minPenetration = pen;
			index = counter; 
		}
		counter++;
    }

    // Cross products...
	
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Vector3 a1 = b1.getCol(i);
            Vector3 a2 = b2.getCol(j);
            Vector3 axis = a1.cross(a2);
            if (axis == VEC3_ZERO) continue;
			axis = axis.normalized();
			float pen = boxPenetrationAxis(x1, x2, axis, disp);
			std::cout << i << " " << j << " " << a1.to_string() << " " << a2.to_string() << " " << axis.to_string() << " " << pen << "\n";
            if (pen < 0) return cd;
			if (pen < minPenetration) {
				minPenetration = pen;
				index = counter;
			}
			counter++;
        }
    }

	// Determine events

	// 1. index < 6: smallest penetrating axis is along the basis of a box. This means one of the vertices of the other box intersects with the first one. Let's find out which one.
	if (index < 3) {
		return boxVertexToFace(x1, x2, b1.getCol(index), disp, minPenetration);
	}
	if (index < 6) {
		return boxVertexToFace(x2, x1, b2.getCol(index - 3), disp * -1, minPenetration);
	}

    cd.shapes[0] = &x1;
    cd.shapes[1] = &x2;

    return cd;
}

Collision checkCollision(CollisionShape* x1, CollisionShape* x2) {
    if (!x1 || !x2) return Collision();
	Collision cd;

    if (CollisionSphere* c1 = dynamic_cast<CollisionSphere*>(x1)) {
        if (CollisionSphere* c2 = dynamic_cast<CollisionSphere*>(x2)) return checkCollision(*c1, *c2);
    }
    if (CollisionBox* c1 = dynamic_cast<CollisionBox*>(x1)) {
        if (CollisionBox* c2 = dynamic_cast<CollisionBox*>(x2)) return checkCollision(*c1, *c2);
    }


    return cd;
}

// OLD Separating Axis Test (return true if no collision in that direction)

/*
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
	*/

#endif