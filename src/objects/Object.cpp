#include <string>
#include "../../include/utils/Math.h"

#include "../../include/objects/Object.h"

// Abstract class representing a 3 dimensional object.

	Object::Object() {

	}

	Object::Object(const Object& other) {
		transform = other.transform;
	}

	// MUST OVERRIDE ME

	// Transform the object but also affect the transform itself. This is so you can keep track of the position.

	void Object::Trans(Transform t) {
		transform = t * transform;
	}

	// Transform the object but not affect the transform. This is to modify teh object without affecting the coordinate system.

	void Object::ForceTrans(Transform t) {

	}

	std::string Object::to_string() {
		return "Object[" + transform.to_string() + "]";
	}