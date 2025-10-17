#ifndef FRAGMENT_EXT
#define FRAGMENT_EXT

#include <string>

#include "../math/Vectors.h"
#include "../../material/Material.h"

/*

Fragments represent pixel data which can be used in shaders. Fragments can hold information such as position, normal, uv, and color.

*/

class Fragment {
	public:
	Vector4 ndc;
	Vector3 normal;
	Vector2 uv = NILVEC2;
	uint32_t color = 0;
	uint32_t albedo = 0;
	Vector2 screenUV = NILVEC2;
	Vector3 wspos;

	Fragment() {
		ndc.z = INF;
		ndc.w = INF;
		normal.z = 1;
	}

	Fragment(Vector4 V, Vector3 N, uint32_t C) {
		ndc = (V);
		normal = (N);
		color = C;
	}

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U) {
		ndc = (V);
		normal = (N);
		color = C;
		uv = (U);
	}

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A) {
		ndc = (V);
		normal = (N);
		color = C;
		uv = (U);
		albedo = A;
	}

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A, Vector2 suv) {
		ndc = (V);
		normal = (N);
		color = C;
		uv = (U);
		albedo = A;
		screenUV = (suv);
	}

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A, Vector2 suv, Vector3 wsp) {
		ndc = (V);
		normal = (N);
		color = C;
		uv = (U);
		albedo = A;
		screenUV = (suv);
		wspos = (wsp);
	}

	Fragment(const Fragment& o) {
		ndc = (o.ndc);
		normal = (o.normal);
		color = o.color;
		uv = (o.uv);
		albedo = o.albedo;
		screenUV = (o.screenUV);
		wspos = (o.wspos);
	}

	inline bool operator==(Fragment& o) {
		return (ndc == o.ndc) && (normal == o.normal) && (color == o.color) && (uv == o.uv) && (albedo == o.albedo) && (screenUV == o.screenUV) && (wspos == o.wspos);
	}

	inline bool operator!=(Fragment& o) {
		return !( (ndc == o.ndc) && (normal == o.normal) && (color == o.color) && (uv == o.uv) && (albedo == o.albedo) && (screenUV == o.screenUV) && (wspos == o.wspos));
	}

	// UTIL

	inline std::string to_string() {
		return "Fragment[ndc=" + ndc.to_string() + ", n=" + normal.to_string() + ", c=" + std::to_string(color) + ", uv=" + uv.to_string() + ", albedo=" + std::to_string(albedo) + ", suv=" + screenUV.to_string() + ", wspos=" + wspos.to_string() + "]";
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