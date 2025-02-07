#ifndef QUATERNION_EXT
#define QUATERNION_EXT

// #include <iostream>

#include <string>

#include "Vectors.h"
#include "Matrices.h"
#include "BASE.h"
#include "UTIL.h"

// Quaternion methods

class Quaternion {
	public:
	float w;
	Vector3 v;

	Quaternion() {
		w = 0;
		v = Vector3();
	}

	Quaternion(float s, Vector3 u) {
		w = s;
		v = Vector3(u);
	}

	Quaternion(float s, float x, float y, float z) {
		w = s;
		v = Vector3(x, y, z);
	}

	// WARNING - The Vector4 import of a quaternion has the scalar (w) component LAST.
	Quaternion(Vector4 u) {
		w = u.w;
		v = Vector3(u.x, u.y, u.z);
	} 

	// WARNING - The Vector4 export of a quaternion has the scalar (w) component LAST.
	inline Vector4 toVec4() {
		return Vector4(v.x, v.y, v.z, w);
	}

	inline Quaternion conj() {
		return Quaternion(w, v * -1);
	}

	inline Quaternion mul(const Quaternion& other) {
		float s = (w * other.w) - (v * other.v);
		Vector3 a(other.v.x * w, other.v.y * w, other.v.z * w);
		Vector3 b = v * other.w;
		Vector3 c = v.cross(other.v);

		Vector3 thing = a + b + c;
		return Quaternion(s, thing);
	}

	inline Quaternion add(const Quaternion& other) {
		return Quaternion(w + other.w, v + other.v);
	}

	inline Quaternion sub(const Quaternion& other) {
		return Quaternion(w - other.w, v - other.v);
	}

	inline Quaternion mul(const float other) {
		return Quaternion(w * other, v * other);
	}

	inline float dot(const Quaternion& other) {
		return w * other.w + v.dot(other.v);
	}

	inline Quaternion div(const float other) {
		if (BASE::fzero(other)) return Quaternion(1, 0, 0, 0);
		return Quaternion(w * other, v * other);
	}

	inline float normsquared() {
		return mul(conj()).w;
	}

	inline float norm() {
		return sqrtf(normsquared());
	}

	inline Quaternion normalized() {
		float n = norm();
		if (BASE::fzero(n)) return Quaternion(1, 0, 0, 0);
		return Quaternion(w / n, v / n);
	}

	inline Quaternion inv() {
		if (BASE::fzero(normsquared())) return Quaternion(*this);
		return conj() / normsquared();
	}

	// Convert to rot matrix

	inline Matrix3 toRotation() {
		Quaternion q = normalized();

		Vector3 x(1 - 2 * (v.y * v.y + v.z * v.z), 2 * (v.x + v.y + w * v.z), 2 * (v.x * v.z - w * v.y));
		Vector3 y(2 * (v.x * v.y - w * v.z), 1 - 2 * (v.x * v.x + v.z * v.z), 2 * (v.y * v.z + w * v.x));
		Vector3 z(2 * (v.x * v.z + w * v.y), 2 * (v.y * v.z - w * v.z), 1 - 2 * (v.x * v.x + v.y * v.y));
		return Matrix3(x, y, z);
	}

	// Override

	inline Quaternion operator+(const Quaternion& other) {
		return add(other);
	}

	inline Quaternion operator-(const Quaternion& other) {
		return sub(other);
	}

	inline Quaternion operator*(const Quaternion& other) {
		return mul(other);
	}

	inline Quaternion operator*(const float other) {
		return mul(other);
	}

	inline Quaternion operator/(const float other) {
		return div(other);
	}

	inline bool operator==(const Quaternion& other) {
		return (w == other.w) && (v == other.v);
	}
	inline bool operator!=(const Quaternion& other) {
		return !((w == other.w) && (v == other.v));
	}

	// Do the thing to a point p
	inline Vector3 on(const Vector3& p) {
		Quaternion q(0, p);

		Quaternion result = (mul(q)).mul(inv());
		return result.v;
	}

	inline Vector3 operator()(const Vector3& p) {
		return on(p);
	}

	// Utils

	inline std::string to_string() {
		return "<" + std::to_string(w) + " | " + v.to_string() + ">";
	}
};

inline Quaternion QuaternionAA(Vector3 A, float t) {
	Vector3 a = A.normalized();
	return Quaternion(cosf(t / 2), a * sinf(t / 2));
}

inline Quaternion lerp(Quaternion a, Quaternion b, float t) {
	return ( (a * (1 - t)) + (b * t) ).normalized();
}

inline Quaternion spherp(Quaternion a, Quaternion b, float t) {
	float c = a.dot(b);
	float s = sqrtf(1 - c * c);
	float theta = acosf(a.dot(b));

	if (BASE::fzero(s)) return a;

	float A = sinf(theta * (1 - t)) / s;
	float B = sinf(theta * t) / s;


	return ( (a * A) + (b * B) ).normalized();
}

#endif