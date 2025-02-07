#ifndef OBJECT_EXT
#define OBJECT_EXT

#include "../utils/math/Transform.h"

// Abstract class representing a 3 dimensional object.

class Object {
	public:

	Transform transform;

	Object() {

	}

	// MUST OVERRIDE ME

	// Transform the object but also affect the transform itself. This is so you can keep track of the position.

	virtual void Trans(Transform t) {
		transform = t * transform;
	}

	// Transform the object but not affect the transform. This is to modify teh object without affecting the coordinate system.

	virtual void ForceTrans(Transform t) {

	}
};

#endif