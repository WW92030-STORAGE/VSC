#ifndef QUATERNION_EXT
#define QUATERNION_EXT


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

	Quaternion();

	Quaternion(float s, Vector3 u);

	Quaternion(float s, float x, float y, float z);

	static Quaternion eye();

	// WARNING - The Vector4 import of a quaternion has the scalar (w) component LAST.
	Quaternion(Vector4 u);

	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	Quaternion(Matrix3 m);

	// WARNING - The Vector4 export of a quaternion has the scalar (w) component LAST.
	Vector4 toVec4();

	Quaternion conj();

	Quaternion mul(const Quaternion& other);

	Quaternion add(const Quaternion& other);

	Quaternion sub(const Quaternion& other);

	Quaternion mul(const float other);

	float dot(const Quaternion& other);

	Quaternion div(const float other);

	float normsquared();

	float norm();

	Quaternion normalized();

	Quaternion inv();

	// Add a scaled vector
	Quaternion addScaled(Vector3 v, float scale);

	// Convert to rot matrix

	Matrix3 toRotation();

	// Override

	Quaternion operator+(const Quaternion& other);

	Quaternion operator-(const Quaternion& other);

	Quaternion operator*(const Quaternion& other);

	Quaternion operator*(const float other);

	Quaternion operator/(const float other);

	bool operator==(const Quaternion& other);
	bool operator!=(const Quaternion& other);

	// Do the thing to a point p
	Vector3 on(const Vector3& p);

	Vector3 operator()(const Vector3& p);

	// Utils

	std::string to_string();

	std::string sprintf();
};

static Quaternion QuaternionAA(Vector3 A, float t) {
	Vector3 a = A.normalized();
	return Quaternion(cosf(t * 0.5), a * sinf(t * 0.5));
}

static Quaternion lerp(Quaternion a, Quaternion b, float t) {
	a = a.normalized();
	b = b.normalized();
	return ( (a * (1 - t)) + (b * t) ).normalized();
}

static Quaternion spherp(Quaternion a, Quaternion b, float t) {
	a = a.normalized();
	b = b.normalized();
	float c = a.dot(b);

	if (c < 0) {
		a = a * -1;
		c = a.dot(b);
	}
	float s = sqrtf(1 - c * c);
	float theta = acosf(a.dot(b));

	if (BASE::fzero(s)) return a;

	float sinv = 1.0 / s;

	float A = sinf(theta * (1 - t)) * sinv;
	float B = sinf(theta * t) * sinv;


	return ( (a * A) + (b * B) ).normalized();
}

static Quaternion slerp(Quaternion a, Quaternion b, float t) {
	return spherp(a, b, t);
}

#endif