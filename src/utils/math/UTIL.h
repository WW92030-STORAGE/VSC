#ifndef MATH_UTIL_EXT
#define MATH_UTIL_EXT

// Math methods that use discrete structures

#include "Vectors.h"
#include "BASE.h"

// Solves the quadratic equation ax^2 + bx + c = 0
// https://people.csail.mit.edu/bkph/articles/Quadratics.pdf
inline Vector2 quadratic(float a, float b, float c) {
	if (a == 0) {
		if (b == 0) return NILVEC2;
		return Vector2(-c/b, -c/b);
	}

	float disc = b * b - 4 * a * c;
	if (disc < 0) return NILVEC2; // Complex roots do later

	disc = sqrtf(disc);

	float r = NAN;
	float s = NAN;
	if (b >= 0) {
			r = 0.5 * (-b - disc) / a;
			s = -2 * c / (b + disc);
		}
		else {
			r = 2 * c / (disc - b);
			s = 0.5 * (disc - b) / a;
		}
	if (r > s) return Vector2(s, r);
	return Vector2(r, s);
}

// Are these things zero?
inline bool isZero(Vector2 v) {
	return BASE::fzero(v.x) && BASE::fzero(v.y);
}

inline bool isZero(Vector3 v) {
	return BASE::fzero(v.x) && BASE::fzero(v.y) && BASE::fzero(v.z);
}

inline bool isZero(Vector4 v) {
	return BASE::fzero(v.x) && BASE::fzero(v.y) && BASE::fzero(v.z) && BASE::fzero(v.w);
}
#endif