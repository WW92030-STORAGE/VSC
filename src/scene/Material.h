#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include "../utils/math/BASE.h"
#include "../utils/math/Vectors.h"
#include "Color.h"

struct BaseMaterial {
	public:
	Vector3 baseColor;
	float specular;

	BaseMaterial() {
		baseColor = Vector3();
		specular = 0;
	}

	BaseMaterial(Vector3 v, float f) {
		baseColor = Vector3(v);
		specular = f;
	}

	BaseMaterial(uint32_t u, float f) {
		baseColor = rgb(u);
		specular = f;
	}

	BaseMaterial(const BaseMaterial& other) {
		baseColor = Vector3(other.baseColor);
		specular = other.specular;
	}

	bool operator==(BaseMaterial& other) {
		return (baseColor == other.baseColor) && BASE::fequal(specular, other.specular);
	}

	inline std::string to_string() {
		return "BaseMaterial[" + baseColor.to_string() + ", " + std::to_string(specular) + "]";
	}
};

BaseMaterial BASEMAT_WHITE(Vector3(1, 1, 1), 0);

#endif