#include "../../include/raytracer/Ray.h"
#include "../../include/utils/Math.h"

#include <string>
#include <cmath>
#include <cfloat>
#include <algorithm>



#include "../../include/raytracer/aabb.h"

	AABB::AABB() {
		
	}

	AABB::AABB(Vector3 a, Vector3 b) {
		inferior = min(a, b);
		superior = max(a, b);
	}

	AABB::AABB(Triangle3 t) {
		inferior = min(min(t.p[0], t.p[1]), t.p[2]) - MARGIN;
		superior = max(max(t.p[0], t.p[1]), t.p[2]) + MARGIN;
	}

	AABB::AABB(const AABB& other) {
		inferior = Vector3(other.inferior);
		superior = Vector3(other.superior);
	}

	AABB AABB::merge(AABB other) {
		return AABB(min(inferior, other.inferior) - MARGIN, max(superior, other.superior) + MARGIN);
	}

	bool AABB::operator<(AABB& other) {
		if (!(inferior == other.inferior)) return inferior < other.inferior;
		return superior < other.superior;
	}

	bool AABB::operator==(AABB& other) {
		return inferior == other.inferior && superior == other.superior;
	}
	std::string AABB::to_string() {
		return "AABB[" + inferior.to_string() + ", " + superior.to_string() + "]";
	}

	float AABB::volume() {
		return (superior.x - inferior.x) * (superior.y - inferior.y) * (superior.z - inferior.z);
	}

	float AABB::area() {
		float dx = superior.x - inferior.x;
		float dy = superior.y - inferior.y;
		float dz = superior.z - inferior.z;

		float res = (dx * dy + dy * dz + dz * dx); 
		return res + res;
	}

	float AABB::IntersectRay(Ray r) {
		// Consider the infinite planes containing the six sides, and then the three pairs of planes in the three directions.
		// Clearly the portion of the ray inside the box must be inside the slabs formed by these pairs.
		// So to find the intersection time interval we take the maximum of the intersections with the closest planes in each pai
		// And the minimum of the intersections of the far planes in each pair.
		// If the ray is parallel to a pair we just compare origin coordinates to see if it is inside.

		float ENTER = -1 * INF;
		float EXIT = INF;

		auto point = Vector3(r.point);
		auto slope = Vector3(r.slope);

		// For each direction (x, y, z = ???) i.e. normal vector:
		for (int d = 0; d < 3; d++) {
			// First we check if the ray is parallel to the slab
			float minP = inferior.get(d);
			float maxP = superior.get(d);
			if (BASE::fzero(slope.get(d))) {
				// if the ray is parallel but outside the slab it will never intersect the box.
				if (point.get(d) < minP || point.get(d) > maxP) return INF;
				continue;
			}

			// The ray is oblique to the planes and will thus intersect them.
			// The ray has form r(t) = o + vt. The plane has form P[d] = T.
			// Thus component wise we simply get o[d] + v[d] * t = P[d]
			// t = (p[d] - o[d]) / v[d]

			float a = (superior.get(d) - point.get(d)) / slope.get(d);
			float b = (inferior.get(d) - point.get(d)) / slope.get(d);
			if (a > b) std::swap(a, b);

			ENTER = std::max(ENTER, a);
			EXIT = std::min(EXIT, b);
		}

		if (ENTER > EXIT) return INF;
		if (EXIT < 0 && ENTER < 0) return INF;
		if (ENTER < 0) return EXIT;
		return ENTER;
	}