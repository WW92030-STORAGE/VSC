#ifndef DIR_LIGHT_EXT
#define DIR_LIGHT_EXT

#include "../utils/Math.h"
#include "Light.h"

class DirectionalLight : public Light {
	public:
	Vector3 dir = Vector3(0, 0, -1);

	DirectionalLight();
	
	DirectionalLight(Vector3& v);
	DirectionalLight(Vector3& v, Vector3 i);

	void Trans(Transform t);
};

#endif