#ifndef AABB_EXT
#define AABB_EXT

#include "Ray.h"
#include "../utils/Math.h"

#include <string>
#include <cmath>
#include <cfloat>
#include <algorithm>

class AABB {
	public:
	Vector3 inferior;
	Vector3 superior;

	Vector3 MARGIN = Vector3(1, 1, 1) * 0.02;

	AABB();

	AABB(Vector3 a, Vector3 b);

	AABB(Triangle3 t);

	AABB(const AABB& other);

	AABB merge(AABB other);

	bool operator<(AABB& other);

	bool operator==(AABB& other);

	std::string to_string();

	float volume();

	float area();

	float IntersectRay(Ray r);
};

#endif