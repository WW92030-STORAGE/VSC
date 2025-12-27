#include "../../include/lighting/DirectionalLight.h"

#include "../../include/utils/Math.h"

	DirectionalLight::DirectionalLight() {
		TYPE = DIRECTIONAL;
	}
	
	DirectionalLight::DirectionalLight(Vector3& v) {
		TYPE = DIRECTIONAL;
		if (!isZero(v)) dir = v.normalized();
	}

	DirectionalLight::DirectionalLight(Vector3& v, Vector3 i) {
		TYPE = DIRECTIONAL;
		intensity = Vector3(i);
		if (!isZero(v)) dir = v.normalized();
	}

	void DirectionalLight::Trans(Transform t) {
		dir = vec3(t.matrix() * vec4(dir)).normalized();
	}
