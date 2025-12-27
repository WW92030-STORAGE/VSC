#ifndef PRIMITIVES_EXT
#define PRIMITIVES_EXT

#include <string>
#include "../math/Vectors.h"
#include "../math/Matrices.h"
#include "../math/BASE.h"
#include "../math/UTIL.h"

#include "../../objects/Object.h"

// Geometrical primitives.
// Points are Vector3 (Vector4 with w = 1). Methods are declared from more "complex" forms into more "simple" forms. 
// So to test intersection between line and plane, you must do plane.intersection(line) instead of the other way around.

class Line {
	public:

	Vector3 point;
	Vector3 slope;

	Line();

	Line(Vector3 p, Vector3 s);

	Line(const Line& other);

	Vector3 get(float t);

	// Distances

	float squaredDistanceFrom(Vector3& other);

	float squaredDistanceFrom(Line& other);

	float distanceFrom(Vector3& other);
	float distanceFrom(Line& other);

	// Intersections

	bool intersect(Vector3& other);
	bool intersect(Line& other);

	// Utils

	std::string to_string();
};

// A plane is defined as the set of points S = {x} where (x - P) @ N = 0. 
// In short, the set of points that, the vector to some point P is normal to the well, normal vector N.
class Plane : public Object {
	public:
	Vector3 p;
	Vector3 n;

	Plane();

	Plane(Vector3 r, Vector3 s);

	Plane(const Plane& other);

	// Standard Form Planes

	// Ax + By + Cz + D = 0 or N @ x + D = 0. D/|N| is the signed distance from origin. If N is normal then D is simply signed distance.
	Vector4 standardForm();

	// Ax + By + Cz + D = 0
	// intercepts: x_i = -D/A_i
	Plane(const Vector4& s);

	Plane(float a, float b, float c, float d);

	// Get the projection of a vector into the plane

	Vector3 proj(Vector3 other);

	// Distances

	// signed distance from origin. This is the D value in the standard form, assuming n is normalized
	float distanceFromOrigin();

	// Distance from Point
	float distanceFrom(const Vector3& q);

	float distanceFrom(const Vector4& q);

	// Intersections

	bool contains(const Vector3& p);

	float intersectionTime(Line& L);

	Vector3 intersection(Line& L);


	
	// 3 planes (gives one point of the intersection)
	Vector3 intersection(Plane& A, Plane& B);

	// 2 planes (gives one line of the intersection)
	Line intersection(Plane& A);

	// Transform a plane

	Plane transform_(Transform T);

	// Re-initialize this plane
	void reinit(Vector4 s);
	
	void Trans(Transform T) override;

	void ForceTrans(Transform T) override;

	// Utils

	std::string to_string();
};

#endif 