#ifndef FRAGMENT_EXT
#define FRAGMENT_EXT

#include <string>

#include "../math/Vectors.h"
#include "../../material/Material.h"

class Fragment {
	public:
	Vector4 ndc;
	Vector3 normal;
	Vector2 uv = NILVEC2;
	uint32_t color = 0;
	uint32_t albedo = 0;

	Fragment() {
		ndc = Vector4(0, 0, FLT_MAX, FLT_MAX);
		normal = Vector3(0, 0, 1);
	}

	Fragment(Vector4 V, Vector3 N, uint32_t C) {
		ndc = Vector4(V);
		normal = Vector3(N);
		color = C;
	}

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U) {
		ndc = Vector4(V);
		normal = Vector3(N);
		color = C;
		uv = Vector2(U);
	}

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A) {
		ndc = Vector4(V);
		normal = Vector3(N);
		color = C;
		uv = Vector2(U);
		albedo = A;
	}

	Fragment(const Fragment& o) {
		ndc = Vector4(o.ndc);
		normal = Vector3(o.normal);
		color = o.color;
		uv = Vector2(o.uv);
		albedo = o.albedo;
	}

	inline bool operator==(Fragment& o) {
		return (ndc == o.ndc) && (normal == o.normal) && (color == o.color) && (uv == o.uv) && (albedo == o.albedo);
	}

	inline bool operator!=(Fragment& o) {
		return !( (ndc == o.ndc) && (normal == o.normal) && (color == o.color) && (uv == o.uv) && (albedo == o.albedo));
	}

	// UTIL

	inline std::string to_string() {
		return "Fragment[ndc=" + ndc.to_string() + ", n=" + normal.to_string() + ", c=" + std::to_string(color) + ", uv=" + uv.to_string() + "]";
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