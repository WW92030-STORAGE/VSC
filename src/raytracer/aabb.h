#ifndef AABB_EXT
#define AABB_EXT

#include "Ray.h"
#include "../utils/Math.h"

#include <string>
#include <cmath>
#include <cfloat>

class AABB {
	public:
	Vector3 inferior;
	Vector3 superior;

	AABB(Vector3 a, Vector3 b) {
		inferior = min(a, b);
		superior = max(a, b);
	}

	AABB(AABB& other) {
		inferior = Vector3(other.inferior);
		superior = Vector3(other.superior);
	}

	inline AABB merge(AABB other) {
		return AABB(min(inferior, other.inferior), max(superior, other.superior));
	}

	inline bool operator<(AABB& other) {
		if (!(inferior == other.inferior)) return inferior < other.inferior;
		return superior < other.superior;
	}

	inline bool operator==(AABB& other) {
		return inferior == other.inferior && superior == other.superior;
	}
	inline std::string to_string() {
		return "AABB[" + inferior.to_string() + ", " + superior.to_string() + "]";
	}
};

#endif