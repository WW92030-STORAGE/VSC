#ifndef FRAGMENT_EXT
#define FRAGMENT_EXT

#include <string>

#include "../Math.h"
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

	Fragment();

	Fragment(Vector4 V, Vector3 N, uint32_t C);

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U);

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A);

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A, Vector2 suv);

	Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A, Vector2 suv, Vector3 wsp);

	Fragment(const Fragment& o);

	bool operator==(Fragment& o);

	bool operator!=(Fragment& o);

	// UTIL

	std::string to_string();
};

class ReducedFrag {
	public:
	float depth;
	uint32_t color;

	ReducedFrag();

	ReducedFrag(float F, uint32_t C);

	ReducedFrag(const ReducedFrag& o);

	bool operator==(ReducedFrag& o);

	bool operator!=(ReducedFrag& o);

	// UTIL

	std::string to_string();
};

const Fragment NILFRAG = Fragment(NILVEC4, NILVEC3, 0);


#endif