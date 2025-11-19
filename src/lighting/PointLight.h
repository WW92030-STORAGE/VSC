#ifndef PT_LIGHT_EXT
#define PT_LIGHT_EXT

#include "../utils/math/UTIL.h"
#include "../utils/math/Vectors.h"
#include "Light.h"

class PointLight : public Light {
	public:

	PointLight() {
		attenuation = 1;
		TYPE = POINT;
	}

	PointLight(Vector3 i) {
		attenuation = 1;
		TYPE = POINT;
		intensity = Vector3(i);
	}

	PointLight(float a) {
		attenuation = a;
		TYPE = POINT;
	}

	PointLight(Vector3 i, float a) {
		TYPE = POINT;
		intensity = Vector3(i);
		attenuation = a;
	}
};

#endif