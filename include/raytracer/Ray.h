#ifndef RAY_EXT
#define RAY_EXT

#include <string>

#include "../utils/Math.h"
#include "../utils/geometry/Primitives.h"
#include "../material/Material.h"
#include "../objects/Mesh.h"
#include "../objects/Object.h"
#include "../objects/Triangle.h"

class Ray {
	public:
	Vector3 point;
	Vector3 slope;

	Ray();

	Ray(Vector3 p, Vector3 s);

	Ray(const Ray& other);

	Vector3 get(float t);

	Line toLine();

	float intersectTriangle(Triangle3 t);

	std::string to_string();
};

class IntersectionPoint {
	public:
	BaseMaterial* material;
	float time;
	Vector3 N;
	Vector2 uv;

	IntersectionPoint();

	IntersectionPoint(BaseMaterial* bm, float t, Vector3 n, Vector2 u);

	bool valid();

	~IntersectionPoint();

	std::string to_string();
};

#endif