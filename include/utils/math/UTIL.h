#ifndef MATH_UTIL_EXT
#define MATH_UTIL_EXT

// Math methods that use discrete structures

#include "Vectors.h"
#include "BASE.h"

// Solves the quadratic equation ax^2 + bx + c = 0
// https://people.csail.mit.edu/bkph/articles/Quadratics.pdf
static Vector2 quadratic(float a, float b, float c) {
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
static bool isZero(Vector2 v) {
	return BASE::fzero(v.x) && BASE::fzero(v.y);
}

static bool isZero(Vector3 v) {
	return BASE::fzero(v.x) && BASE::fzero(v.y) && BASE::fzero(v.z);
}

static bool isZero(Vector4 v) {
	return BASE::fzero(v.x) && BASE::fzero(v.y) && BASE::fzero(v.z) && BASE::fzero(v.w);
}

// L1-normalize a vector (so the sum of the elements is 1)

static Vector3 NormSum(Vector3 v) {
	float sum = v.x + v.y + v.z;
	if (BASE::fzero(sum)) return v;
	return v / sum;
}

// Elementwise min/max
static Vector3 min(Vector3 a, Vector3 b) {
	return Vector3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
}
static Vector3 max(Vector3 a, Vector3 b) {
	return Vector3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
}

// Linear Interpolation (Yes this also exists as several predefined functions for common types. But here it is in the most general form)

template <typename T>
static T lerp(T a, T b, float t) {
	return (a * (1 - t)) + (b * t);
}

// Polar and rectangular

static Vector2 rect2polar(Vector2 rect) {
	float theta = atan2(rect.y, rect.x);
	float modu = rect.length();

	return Vector2(modu, theta);
}

static Vector2 polar2rect(Vector2 polar) {
	return Vector2(cosf(polar.y), sinf(polar.y)) * polar.x;
}

#endif