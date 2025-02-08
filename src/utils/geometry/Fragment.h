#ifndef FRAGMENT_EXT
#define FRAGMENT_EXT

#include <string>

#include "../math/Vectors.h"
#include "../../scene/Material.h"

class Fragment {
	public:
	Vector4 ndc;
	Vector3 normal;
	uint32_t color;

	Fragment() {
		ndc = Vector4(0, 0, FLT_MAX, FLT_MAX);
		normal = Vector3(0, 0, 1);
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

class ReducedFrag {
	public:
	float depth;
	uint32_t color;

	ReducedFrag() {
		depth = FLT_MAX;
		color = 0;
	}

	ReducedFrag(float F, uint32_t C) {
		depth = F;
		color = C;
	}

	ReducedFrag(const ReducedFrag& o) {
		depth = o.depth;
		color = o.color;
	}

	inline bool operator==(ReducedFrag& o) {
		return BASE::fequal(depth, o.depth) && (color == o.color);
	}

	inline bool operator!=(ReducedFrag& o) {
		return !( BASE::fequal(depth, o.depth) && (color == o.color) );
	}

	// UTIL

	inline std::string to_string() {
		return "ReducedFrag[" + std::to_string(depth) + " " + std::to_string(color) + "]";
	}
};

Fragment NILFRAG(NILVEC4, NILVEC3, 0);


#endif