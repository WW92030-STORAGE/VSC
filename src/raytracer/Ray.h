#ifndef RAY_EXT
#define RAY_EXT

#include <string>

#include "../utils/Math.h"
#include "../utils/geometry/Primitives.h"
#include "../material/Material.h"
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
		point = (p);
		slope = Vector3(s).normalized();
	}

	Ray(const Ray& other) {
		point = (other.point);
		slope = (other.slope);
	}

	Vector3 get(float t) {
		return point + slope * t;
	}

	Line toLine() {
		return Line(point, slope);
	}

	inline float intersectTriangle(Triangle3 t) {
		return t.intersectionTime(toLine());
	}

	inline std::string to_string() {
		return "Ray[" + point.to_string() + ", " + slope.to_string() + "]";
	}
};

class IntersectionPoint {
	public:
	BaseMaterial* material;
	float time;
	Vector3 N;
	Vector2 uv;

	IntersectionPoint() {
		material = nullptr;
		time = INF;
		N = Vector3(1, 0, 0);
		uv = Vector2(0 ,0);
	}

	IntersectionPoint(BaseMaterial* bm, float t, Vector3 n, Vector2 u) {
		material = bm;
		time = t;
		N = (n);
		uv = (u);
	}

	inline bool valid() {
		return (!BASE::fequal(INF, time) && N != NILVEC3);
	}

	~IntersectionPoint() {
	}

	std::string to_string() {
		std::string res = "IntersectionPoint[" + std::to_string(time) + ", " + N.to_string() + ", " + uv.to_string() + ", ";
		if (material) res = res + material->to_string();
		return res + "]";
	}
};

#endif