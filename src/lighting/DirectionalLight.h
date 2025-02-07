#ifndef DIR_LIGHT_EXT
#define DIR_LIGHT_EXT

#include "../utils/math/UTIL.h"
#include "../utils/math/Vectors.h"
#include "Light.h"

class DirectionalLight : public Light {
	public:
	Vector3 dir = Vector3(0, 0, -1);

	DirectionalLight() {
		TYPE = DIRECTIONAL;
	}
	
	DirectionalLight(Vector3& v) {
		TYPE = DIRECTIONAL;
		if (!isZero(v)) dir = v.normalized();
	}

	DirectionalLight(Vector3& v, Vector3 i) {
		TYPE = DIRECTIONAL;
		intensity = Vector3(i);
		if (!isZero(v)) dir = v.normalized();
	}

	inline void Trans(Transform t) {
		dir = vec3(t.matrix() * vec4(dir)).normalized();
	}
};

#endif