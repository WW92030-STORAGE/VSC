#include <string>

#include "../../include/utils/math/BASE.h"
#include "../../include/utils/math/Vectors.h"
#include "../../include/scene/Color.h"

#include "../../include/material/Material.h"

	BaseMaterial::BaseMaterial() {
		baseColor = Vector3();
		specular = 0;
	}

	BaseMaterial::BaseMaterial(Vector3 v, float spec, float refl, float refr) {
		baseColor = Vector3(v);
		specular = spec;
		reflective = refl;
		refraction = refr;

		if (refraction >= 0) refractive = true;
	}

	BaseMaterial::BaseMaterial(uint32_t u, float spec, float refl, float refr) {
		baseColor = rgb(u);
		specular = spec;
		reflective = refl;
		refraction = refr;

		if (refraction >= 0) refractive = true;
	}

	BaseMaterial::BaseMaterial(const BaseMaterial& other) {
		baseColor = Vector3(other.baseColor);
		specular = other.specular;
		reflective = other.reflective;
		refractive = other.refractive;
		refraction = other.refraction;

		TYPE = other.TYPE;
	}

	uint32_t BaseMaterial::getColor(Vector2 v) {
		return rgb(baseColor);
	}

	uint32_t BaseMaterial::getColor(int x, int y) {
		return rgb(baseColor);
	}

	bool BaseMaterial::operator==(BaseMaterial& other) {
		return (baseColor == other.baseColor) && BASE::fequal(specular, other.specular) && BASE::fequal(reflective, other.reflective), BASE::fequal(refraction, other.refraction) && (refractive == other.refractive);
	}

	std::string BaseMaterial::to_string() {
		return "BaseMaterial[" + baseColor.to_string() + ", " + std::to_string(specular) + ", " + std::to_string(reflective) + ", " + std::to_string(refraction) + "]";
	}

const BaseMaterial BASEMAT_WHITE = BaseMaterial(Vector3(1, 1, 1), 0);
const BaseMaterial BASEMAT_RED = BaseMaterial(Vector3(1, 0, 0), 0);
const BaseMaterial BASEMAT_YELLOW = BaseMaterial(Vector3(1, 1, 0), 0);
const BaseMaterial BASEMAT_GREEN = BaseMaterial(Vector3(0, 1, 0), 0);
const BaseMaterial BASEMAT_CYAN = BaseMaterial(Vector3(0, 1, 1), 0);
const BaseMaterial BASEMAT_BLUE = BaseMaterial(Vector3(0, 0, 1), 0);
const BaseMaterial BASEMAT_MAGENTA = BaseMaterial(Vector3(1, 0, 1), 0);
const BaseMaterial BASEMAT_BLACK = BaseMaterial(Vector3(0, 0, 0), 0);