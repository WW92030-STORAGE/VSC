#ifndef FRAGMENT_EXT
#define FRAGMENT_EXT

#include <string>

#include "../math/Vectors.h"

class Fragment {
	public:
	Vector4 ndc;
	Vector3 normal;
	uint32_t color;

	Fragment() {
		ndc = Vector4(0, 0, FLT_MAX, FLT_MAX);
		normal = Vector3(0, 0, 1);
		color = 0;
	}

	Fragment(Vector4 V, Vector3 N, uint32_t C) {
		ndc = Vector4(V);
		normal = Vector3(N);
		color = C;
	}

	Fragment(const Fragment& o) {
		ndc = Vector4(o.ndc);
		normal = Vector3(o.normal);
		color = o.color;
	}

	inline bool operator==(Fragment& o) {
		return (ndc == o.ndc) && (normal == o.normal) && (color == o.color);
	}

	inline bool operator!=(Fragment& o) {
		return !( (ndc == o.ndc) && (normal == o.normal) && (color == o.color) );
	}

	// UTIL

	inline std::string to_string() {
		return "Fragment[" + ndc.to_string() + ", " + normal.to_string() + ", " + std::to_string(color) + "]";
	}
};

Fragment NILFRAG(NILVEC4, NILVEC3, 0);


#endif