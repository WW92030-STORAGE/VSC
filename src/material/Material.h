#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include "../utils/math/BASE.h"
#include "../utils/math/Vectors.h"
#include "../scene/Color.h"

struct BaseMaterial {
	public:
	Vector3 baseColor;
	float specular;
	float reflective;
	float refraction;

	enum MaterialType {
		BASE = 0,
		IMAGE = 1
	};

	MaterialType TYPE = BASE;

	BaseMaterial() {
		baseColor = Vector3();
		specular = 0;
	}

	BaseMaterial(Vector3 v, float spec = 0, float refl = 0, float refr = 0) {
		baseColor = Vector3(v);
		specular = spec;
		reflective = refl;
		refraction = refr;
	}

	BaseMaterial(uint32_t u, float spec = 0, float refl = 0, float refr = 0) {
		baseColor = rgb(u);
		specular = spec;
		reflective = refl;
		refraction = refr;
	}

	BaseMaterial(const BaseMaterial& other) {
		baseColor = Vector3(other.baseColor);
		specular = other.specular;
		reflective = other.reflective;
		refraction = other.refraction;

		TYPE = other.TYPE;
	}

	virtual inline uint32_t getColor(Vector2 v) {
		return rgb(baseColor);
	}

	virtual inline uint32_t getColor(int x, int y) {
		return rgb(baseColor);
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