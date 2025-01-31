#ifndef DIR_LIGHT_EXT
#define DIR_LIGHT_EXT

#include "../utils/math/UTIL.h"
#include "../utils/math/Vectors.h"
#include "Light.h"

class PointLight : public Light {
	public:
	float attenuation = 0.5;

	PointLight() {

	}

	PointLight(Vector3 i) {
		intensity = Vector3(i);
	}

	PointLight(Vector3 i, float a) {
		intensity = Vector3(i);
		attenuation = a;
	}
};

#endif