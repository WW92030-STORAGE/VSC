#ifndef LIGHT_EXT
#define LIGHT_EXT

#include "../utils/math/Vectors.h"
#include "../objects/Object.h"

enum LightType {
	NIL,
	POINT,
	DIRECTIONAL
};

class Light : public Object {
	public:
	Vector3 intensity = Vector3(0.5, 0.5, 0.5);
	float attenuation = 0;

	LightType TYPE = NIL;
	

	Light();
	Light(Vector3 i);
};

#endif