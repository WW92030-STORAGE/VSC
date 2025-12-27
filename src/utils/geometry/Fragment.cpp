#include <string>

#include "../../../include/utils/Math.h"
#include "../../../include/material/Material.h"


#include "../../../include/utils/geometry/Fragment.h"

/*

Fragments represent pixel data which can be used in shaders. Fragments can hold information such as position, normal, uv, and color.

*/

	Fragment::Fragment() {
		ndc.z = INF;
		ndc.w = INF;
		normal.z = 1;
	}

	Fragment::Fragment(Vector4 V, Vector3 N, uint32_t C) {
		ndc = (V);
		normal = (N);
		color = C;
	}

	Fragment::Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U) {
		ndc = (V);
		normal = (N);
		color = C;
		uv = (U);
	}

	Fragment::Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A) {
		ndc = (V);
		normal = (N);
		color = C;
		uv = (U);
		albedo = A;
	}

	Fragment::Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A, Vector2 suv) {
		ndc = (V);
		normal = (N);
		color = C;
		uv = (U);
		albedo = A;
		screenUV = (suv);
	}

	Fragment::Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A, Vector2 suv, Vector3 wsp) {
		ndc = (V);
		normal = (N);
		color = C;
		uv = (U);
		albedo = A;
		screenUV = (suv);
		wspos = (wsp);
	}

	Fragment::Fragment(const Fragment& o) {
		ndc = (o.ndc);
		normal = (o.normal);
		color = o.color;
		uv = (o.uv);
		albedo = o.albedo;
		screenUV = (o.screenUV);
		wspos = (o.wspos);
	}

	bool Fragment::operator==(Fragment& o) {
		return (ndc == o.ndc) && (normal == o.normal) && (color == o.color) && (uv == o.uv) && (albedo == o.albedo) && (screenUV == o.screenUV) && (wspos == o.wspos);
	}

	bool Fragment::operator!=(Fragment& o) {
		return !( (ndc == o.ndc) && (normal == o.normal) && (color == o.color) && (uv == o.uv) && (albedo == o.albedo) && (screenUV == o.screenUV) && (wspos == o.wspos));
	}

	// UTIL

	std::string Fragment::to_string() {
		return "Fragment[ndc=" + ndc.to_string() + ", n=" + normal.to_string() + ", c=" + std::to_string(color) + ", uv=" + uv.to_string() + ", albedo=" + std::to_string(albedo) + ", suv=" + screenUV.to_string() + ", wspos=" + wspos.to_string() + "]";
	}


// ReducedFrag

	ReducedFrag::ReducedFrag() {
		depth = FLT_MAX;
		color = 0;
	}

	ReducedFrag::ReducedFrag(float F, uint32_t C) {
		depth = F;
		color = C;
	}

	ReducedFrag::ReducedFrag(const ReducedFrag& o) {
		depth = o.depth;
		color = o.color;
	}

	bool ReducedFrag::operator==(ReducedFrag& o) {
		return BASE::fequal(depth, o.depth) && (color == o.color);
	}

	bool ReducedFrag::operator!=(ReducedFrag& o) {
		return !( BASE::fequal(depth, o.depth) && (color == o.color) );
	}

	// UTIL

	std::string ReducedFrag::to_string() {
		return "ReducedFrag[" + std::to_string(depth) + " " + std::to_string(color) + "]";
	}