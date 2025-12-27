#include <string>

#include "../../include/utils/Math.h"
#include "../../include/utils/geometry/Primitives.h"
#include "../../include/material/Material.h"
#include "../../include/objects/Mesh.h"
#include "../../include/objects/Object.h"
#include "../../include/objects/Triangle.h"



#include "../../include/raytracer/Ray.h"

// Ray

	Ray::Ray() {
		slope = Vector3(1, 0, 0);
	}

	Ray::Ray(Vector3 p, Vector3 s) {
		point = (p);
		slope = Vector3(s).normalized();
	}

	Ray::Ray(const Ray& other) {
		point = (other.point);
		slope = (other.slope);
	}

	Vector3 Ray::get(float t) {
		return point + slope * t;
	}

	Line Ray::toLine() {
		return Line(point, slope);
	}

	float Ray::intersectTriangle(Triangle3 t) {
		return t.intersectionTime(toLine());
	}

	std::string Ray::to_string() {
		return "Ray[" + point.to_string() + ", " + slope.to_string() + "]";
	}

// IntersectionPoint

	IntersectionPoint::IntersectionPoint() {
		material = nullptr;
		time = INF;
		N = Vector3(1, 0, 0);
		uv = Vector2(0 ,0);
	}

	IntersectionPoint::IntersectionPoint(BaseMaterial* bm, float t, Vector3 n, Vector2 u) {
		material = bm;
		time = t;
		N = (n);
		uv = (u);
	}

	bool IntersectionPoint::valid() {
		return (!BASE::fequal(INF, time) && N != NILVEC3);
	}

	IntersectionPoint::~IntersectionPoint() {
	}

	std::string IntersectionPoint::to_string() {
		std::string res = "IntersectionPoint[" + std::to_string(time) + ", " + N.to_string() + ", " + uv.to_string() + ", ";
		if (material) res = res + material->to_string();
		return res + "]";
	}