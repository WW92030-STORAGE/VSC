#ifndef LIGHT_EXT
#define LIGHT_EXT

#include "../utils/math/Vectors.h"

class Light : public Object {
	public:
	Vector3 intensity = Vector3(1, 1, 1);

	Light() {

	}
	Light(Vector3 i) {
		intensity = Vector3(i);
	}
};

#endif