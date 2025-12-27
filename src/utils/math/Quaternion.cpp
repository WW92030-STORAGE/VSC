#include <string>

#include "../../../include/utils/math/Vectors.h"
#include "../../../include/utils/math/Matrices.h"
#include "../../../include/utils/math/BASE.h"
#include "../../../include/utils/math/UTIL.h"


#include "../../../include/utils/math/Quaternion.h"

// Quaternion methods



	Quaternion::Quaternion() {
		w = 0;
	}

	Quaternion::Quaternion(float s, Vector3 u) {
		w = s;
		v = (u);
	}

	Quaternion::Quaternion(float s, float x, float y, float z) {
		w = s;
		v = Vector3(x, y, z);
	}

	Quaternion Quaternion::eye() {
		return Quaternion(1, 0, 0, 0);
	}

	// WARNING - The Vector4 import of a quaternion has the scalar (w) component LAST.
	Quaternion::Quaternion(Vector4 u) {
		w = u.w;
		v = Vector3(u.x, u.y, u.z);
	} 

	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	Quaternion::Quaternion(Matrix3 m) {
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
	Vector4 Quaternion::toVec4() {
		return Vector4(v.x, v.y, v.z, w);
	}

	Quaternion Quaternion::conj() {
		return Quaternion(w, v * -1);
	}

	Quaternion Quaternion::mul(const Quaternion& other) {
		float s = (w * other.w) - (v * other.v);
		Vector3 a(other.v.x * w, other.v.y * w, other.v.z * w);
		Vector3 b = v * other.w;
		Vector3 c = v.cross(other.v);

		Vector3 thing = a + b + c;
		return Quaternion(s, thing);
	}

	Quaternion Quaternion::add(const Quaternion& other) {
		return Quaternion(w + other.w, v + other.v);
	}

	Quaternion Quaternion::sub(const Quaternion& other) {
		return Quaternion(w - other.w, v - other.v);
	}

	Quaternion Quaternion::mul(const float other) {
		return Quaternion(w * other, v * other);
	}

	float Quaternion::dot(const Quaternion& other) {
		return w * other.w + v.dot(other.v);
	}

	Quaternion Quaternion::div(const float other) {
		if (BASE::fzero(other)) return Quaternion(1, 0, 0, 0);
		return Quaternion(w * other, v * other);
	}

	float Quaternion::normsquared() {
		return mul(conj()).w;
	}

	float Quaternion::norm() {
		return sqrtf(normsquared());
	}

	Quaternion Quaternion::normalized() {
		float n = norm();
		if (BASE::fzero(n)) return Quaternion(1, 0, 0, 0);
		float ninv = 1.0 / n;
		return Quaternion(w * ninv, v * ninv);
	}

	Quaternion Quaternion::inv() {
		float nsq = normsquared();
		if (BASE::fzero(nsq)) return Quaternion(*this);
		float nsinv = 1.0 / nsq;
		return conj() * nsinv;
	}

	// Add a scaled vector
	Quaternion Quaternion::addScaled(Vector3 v, float scale) {
		Quaternion q(0, v * scale);
		q = q * (*this);
		return (*this) + q * 0.5;
	}

	// Convert to rot matrix

	Matrix3 Quaternion::toRotation() {
		Quaternion q = normalized();
		Vector3 x(1 - 2 * (v.y * v.y + v.z * v.z), 2 * (v.x * v.y + w * v.z), 2 * (v.x * v.z - w * v.y));
		Vector3 y(2 * (v.x * v.y - w * v.z), 1 - 2 * (v.x * v.x + v.z * v.z), 2 * (v.y * v.z + w * v.x));
		Vector3 z(2 * (v.x * v.z + w * v.y), 2 * (v.y * v.z - w * v.x), 1 - 2 * (v.x * v.x + v.y * v.y));
		return Matrix3(x, y, z);
	}

	// Override

	Quaternion Quaternion::operator+(const Quaternion& other) {
		return add(other);
	}

	Quaternion Quaternion::operator-(const Quaternion& other) {
		return sub(other);
	}

	Quaternion Quaternion::operator*(const Quaternion& other) {
		return mul(other);
	}

	Quaternion Quaternion::operator*(const float other) {
		return mul(other);
	}

	Quaternion Quaternion::operator/(const float other) {
		return div(other);
	}

	bool Quaternion::operator==(const Quaternion& other) {
		return (w == other.w) && (v == other.v);
	}
	bool Quaternion::operator!=(const Quaternion& other) {
		return !((w == other.w) && (v == other.v));
	}

	// Do the thing to a point p
	Vector3 Quaternion::on(const Vector3& p) {
		Quaternion q(0, p);

		Quaternion result = (mul(q)).mul(inv());
		return result.v;
	}

	Vector3 Quaternion::operator()(const Vector3& p) {
		return on(p);
	}

	// Utils

	std::string Quaternion::to_string() {
		return "<" + std::to_string(w) + " | " + v.to_string() + ">";
	}

	std::string Quaternion::sprintf() {
		return to_string();
	}
