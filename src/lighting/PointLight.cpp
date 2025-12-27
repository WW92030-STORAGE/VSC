#include "../../include/utils/Math.h"
#include "../../include/lighting/Light.h"

#include "../../include/lighting/PointLight.h"


	PointLight::PointLight() {
		attenuation = 1;
		TYPE = POINT;
	}

	PointLight::PointLight(Vector3 i) {
		attenuation = 1;
		TYPE = POINT;
		intensity = Vector3(i);
	}

	PointLight::PointLight(float a) {
		attenuation = a;
		TYPE = POINT;
	}

	PointLight::PointLight(Vector3 i, float a) {
		TYPE = POINT;
		intensity = Vector3(i);
		attenuation = a;
	}