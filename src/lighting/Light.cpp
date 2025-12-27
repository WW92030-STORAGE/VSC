#include "../../include/utils/Math.h"
#include "../../include/lighting/Light.h"

	Light::Light() {

	}
	Light::Light(Vector3 i) {
		intensity = Vector3(i);
	}