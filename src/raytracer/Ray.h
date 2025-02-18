#ifndef RAY_EXT
#define RAY_EXT

#include "../utils/Math.h"
#include "../utils/geometry/Primitives.h"
#include "../scene/Material.h"
#include "../objects/Mesh.h"
#include "../objects/Object.h"
#include "../objects/Triangle.h"

// WARNING - This is in development and will only resume after texturing is done.

class Ray {
	public:
	Vector3 point;
	Vector3 slope;

	Ray() {
		point = Vector3();
		slope = Vector3(1, 0, 0);
	}

	Ray(Vector3 p, Vector3 s) {
		point = Vector3(p);
		slope = Vector3(s).normalized();
	}

	Ray(Ray& other) {
		point = Vector3(other.point);
		slope = Vector3(other.slope).normalized();
	}

	Line toLine() {
		return Line(point, slope);
	}

	inline float intersectTriangle(Triangle3 t) {
		return t.intersectionTime(toLine());
	}


};

class IntersectionPoint {
	public:
	BaseMaterial material;
	Vector3 N;
	Object* obj;


};

#endif