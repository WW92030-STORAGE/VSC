#ifndef DIR_LIGHT_EXT
#define DIR_LIGHT_EXT

#include "../utils/math/UTIL.h"
#include "../utils/math/Vectors.h"

class DirectionalLight : public Light {
	public:
	Vector3 dir;

	DirectionalLight() {
		dir = Vector3(0, 0, -1);
	}

	DirectionalLight(Vector3& v) {
		if (isZero(v)) dir = Vector3(0, 0, -1);
		else dir = v.normalized();
	}

	inline void Trans(Transform t) {
		dir = vec3(t.matrix() * vec4(dir)).normalized();
	}
};

#endif