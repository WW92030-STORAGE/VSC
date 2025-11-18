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

	Quaternion() {
		w = 0;
	}

	Quaternion(float s, Vector3 u) {
		w = s;
		v = (u);
	}

	Quaternion(float s, float x, float y, float z) {
		w = s;
		v = Vector3(x, y, z);
	}

	static Quaternion eye() {
		return Quaternion(1, Vector3(0, 0, 0));
	}

	// WARNING - The Vector4 import of a quaternion has the scalar (w) component LAST.
	Quaternion(Vector4 u) {
		w = u.w;
		v = Vector3(u.x, u.y, u.z);
	} 

	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	Quaternion(Matrix3 m) {
		float T = m.get(0, 0) + m.get(1, 1) + m.get(2, 2);

		
		if (T > 0) {
			float S = sqrtf(T + 1.0) * 2.0;
			float SI = 1.0 / S;
			w = 0.25 * S;
			v = Vector3(m.get(2, 1) - m.get(1, 2), m.get(0, 2) - m.get(2, 0), m.get(1, 0) - m.get(0, 1));
			v = v * SI;
		} else if (m.get(0, 0) > m.get(1, 1) && m.get(0, 0) > m.get(2, 2)) {
			float S = 2.0 * sqrtf(1.0 + m.get(0, 0) - m.get(1, 1) - m.get(2, 2));
			float SI = 1.0 / S;
			w = (m.get(2, 1) - m.get(1, 2)) * SI;
			v.x = 0.25 * S;
      		v.y = (m.get(0, 1) + m.get(1, 0)) * SI;
      		v.z = (m.get(0, 2) + m.get(2, 0)) * SI;
		} else if (m.get(1, 1) > m.get(2, 2)) {
			float S = 2.0 * sqrtf(1.0 + m.get(1, 1) - m.get(0, 0) - m.get(2, 2));
			float SI = 1.0 / S;
			w = (m.get(0, 2) - m.get(2, 0)) * SI;
			v.x = (m.get(0, 1) + m.get(1, 0)) * SI;
			v.y = 0.25 * S;
			v.z = (m.get(1, 2) + m.get(2, 1)) * SI;
		} else {
			float S = 2.0 * sqrtf(1.0 + m.get(2, 2) - m.get(0, 0) - m.get(1, 1));
			float SI = 1.0 / S;
			w = (m.get(1, 0) - m.get(0, 1)) * SI;
			v.x = (m.get(0, 2) + m.get(2, 0)) * SI;
			v.y = (m.get(1, 2) + m.get(2, 1)) * SI;
			v.z = 0.25 * S;
		}

		float ss = w * w + v.normsquared();
		if (!BASE::fzero(ss)) {
			w /= sqrtf(ss);
			v = v / sqrtf(ss);
		}
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
		float ninv = 1.0 / n;
		return Quaternion(w * ninv, v * ninv);
	}

	inline Quaternion inv() {
		float nsq = normsquared();
		if (BASE::fzero(nsq)) return Quaternion(*this);
		float nsinv = 1.0 / nsq;
		return conj() * nsinv;
	}

	// Convert to rot matrix

	inline Matrix3 toRotation() {
		Quaternion q = normalized();
		Vector3 x(1 - 2 * (v.y * v.y + v.z * v.z), 2 * (v.x * v.y + w * v.z), 2 * (v.x * v.z - w * v.y));
		Vector3 y(2 * (v.x * v.y - w * v.z), 1 - 2 * (v.x * v.x + v.z * v.z), 2 * (v.y * v.z + w * v.x));
		Vector3 z(2 * (v.x * v.z + w * v.y), 2 * (v.y * v.z - w * v.x), 1 - 2 * (v.x * v.x + v.y * v.y));
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

	inline std::string sprintf() {
		return to_string();
	}
};

inline Quaternion QuaternionAA(Vector3 A, float t) {
	Vector3 a = A.normalized();
	return Quaternion(cosf(t * 0.5), a * sinf(t * 0.5));
}

inline Quaternion lerp(Quaternion a, Quaternion b, float t) {
	a = a.normalized();
	b = b.normalized();
	return ( (a * (1 - t)) + (b * t) ).normalized();
}

Quaternion spherp(Quaternion a, Quaternion b, float t) {
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

inline Quaternion slerp(Quaternion a, Quaternion b, float t) {
	return spherp(a, b, t);
}

#endif