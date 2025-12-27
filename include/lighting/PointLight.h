#ifndef PT_LIGHT_EXT
#define PT_LIGHT_EXT

#include "../utils/Math.h"
#include "Light.h"

class PointLight : public Light {
	public:

	PointLight();

	PointLight(Vector3 i);

	PointLight(float a);

	PointLight(Vector3 i, float a);
};

#endif