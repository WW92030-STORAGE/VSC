#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

#include "../utils/math/BASE.h"
#include "../utils/math/Vectors.h"
#include "../scene/Color.h"

struct BaseMaterial {
	public:
	Vector3 baseColor = Vector3(1, 1, 1);
	float specular = 1.0;
	float reflective = 0.0;
	bool refractive = false;
	float refraction = 0.0;

	enum MaterialType {
		BASE = 0,
		IMAGE = 1
	};

	MaterialType TYPE = BASE;

	BaseMaterial();

	BaseMaterial(Vector3 v, float spec = 0, float refl = 0, float refr = 0);

	BaseMaterial(uint32_t u, float spec = 0, float refl = 0, float refr = 0);

	BaseMaterial(const BaseMaterial& other);

	virtual uint32_t getColor(Vector2 v);

	virtual uint32_t getColor(int x, int y);

	bool operator==(BaseMaterial& other);

	std::string to_string();
};

extern const BaseMaterial BASEMAT_WHITE;
extern const BaseMaterial BASEMAT_RED;
extern const BaseMaterial BASEMAT_YELLOW;
extern const BaseMaterial BASEMAT_GREEN;
extern const BaseMaterial BASEMAT_CYAN;
extern const BaseMaterial BASEMAT_BLUE;
extern const BaseMaterial BASEMAT_MAGENTA;
extern const BaseMaterial BASEMAT_BLACK;

#endif