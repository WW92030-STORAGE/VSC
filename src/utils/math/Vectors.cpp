#include <string>
#include <cmath>
#include <cfloat>
#include <climits>

#include "../../../include/utils/math/BASE.h"



#include "../../../include/utils/math/Vectors.h"

// Vector methods

// Vector of 2 integers, used for triangle rasterization

	Vector2i::Vector2i() {
		x = 0;
		y = 0;
	}

	Vector2i::Vector2i(int r, int s) {
		x = r;
		y = s;
	}

	Vector2i::Vector2i(const Vector2i& other) {
		x = other.x;
		y = other.y;
	}

	std::string Vector2i::to_string() {
		return "Vector2i[" + std::to_string(x) + ", " + std::to_string(y) + "]";
	}

	Vector2::Vector2() {
		x = 0;
		y = 0;
	}
	Vector2::Vector2(float r, float s) {
		x = r;
		y = s;
	}

	Vector2::Vector2(const Vector2& other) {
		x = other.x;
		y = other.y;
	}

	float Vector2::get(int a) {
		if (a == 0) return x;
		return y;
	}

	void Vector2::set(int a, float f) {
		if (a == 0) x = f;
		else y = f;
	}

	// Operations

	Vector2 Vector2::add(const Vector2& other) {
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::inv() {
		return Vector2(-x, -y);
	}

	Vector2 Vector2::sub(const Vector2& other) {
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 Vector2::mul(float other) {
		return Vector2(x * other, y * other);
	}
	Vector2 Vector2::div(float other) {
		if (BASE::fzero(other)) return Vector2(0, 0);
		float f = 1.0 / other;
		return Vector2(x * f, y * f);
	}

	// Vector specific operations

	// dot product
	float Vector2::dot(const Vector2& other) {
		return x * other.x + y * other.y;
	}

	// squared length, then length, then unit vector
	float Vector2::normsquared() {
		return x * x + y * y;
	}

	float Vector2::length() {
		return sqrtf(normsquared());
	}

	Vector2 Vector2::normalized() {
		float len = length();
		if (BASE::fzero(len)) return Vector2(1, 0);
		float l_inv = 1.0 / len;
		return Vector2(x * l_inv, y * l_inv);
	}

	// relative to another vector: cosine of the angle, then angle, then projection, then orthogonal
	float Vector2::cosine(Vector2& other) {
		if (BASE::fzero(length()) || BASE::fzero(other.length())) return 1;
		return dot(other) / (length() * other.length());
	}

	float Vector2::angle(Vector2& other) {
		return acosf(cosine(other));
	}

	Vector2 Vector2::proj(Vector2& other) {
		if (BASE::fzero(other.length())) return Vector2(0, 0);
		return other * (dot(other) / (other.normsquared()));
	}

	Vector2 Vector2::ortho(Vector2& other) {
		return Vector2(x, y) - proj(other);
	}

	// rotations

	Vector2 Vector2::rot(float theta) {
		float c = cosf(theta);
		float s = sinf(theta);
		return Vector2(c * x - s * y, s * x + c * y);
	}

	// Overloading

	Vector2 Vector2::operator+(const Vector2& other) {
		return add(other);
	} 
	Vector2 Vector2::operator-(const Vector2& other) {
		return sub(other);
	} 
	float Vector2::operator*(const Vector2& other) {
		return dot(other);
	} 
	Vector2 Vector2::operator*(const float other) {
		return mul(other);
	} 
	Vector2 Vector2::operator/(const float other) {
		return div(other);
	} 
	bool Vector2::operator==(const Vector2& other) {
		return BASE::fequal(x, other.x) && BASE::fequal(y, other.y);
	} 
	bool Vector2::operator!=(const Vector2& other) {
		return !(BASE::fequal(x, other.x) && BASE::fequal(y, other.y));
	} 
	bool Vector2::operator<(const Vector2& other) const {
		if (!BASE::fequal(x, other.x)) return x < other.x;
		return y < other.y;
	} 

	// UTIL

	std::string Vector2::to_string() {
		return "Vector2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

	Vector3::Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3::Vector3(float r, float s, float t) {
		x = r;
		y = s;
		z = t;
	}

	Vector3::Vector3(const Vector3& other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}

	float Vector3::get(int r) {
		if (r == 0) return x;
		if (r == 1) return y;
		return z;
	}

	void Vector3::set(int r, float f) {
		if (r == 0) x = f;
		else if (r == 1) y = f;
		else z = f;
	}

	void Vector3::override(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}

	// Operations

	Vector3 Vector3::add(const Vector3& other) {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 Vector3::inv() {
		return Vector3(-x, -y, -z);
	}

	Vector3 Vector3::sub(const Vector3& other) {
		return Vector3(x - other.x, y - other.y, z -  other.z);
	}

	Vector3 Vector3::mul(float other) {
		return Vector3(x * other, y * other, z * other);
	}
	Vector3 Vector3::div(float other) {
		if (BASE::fzero(other)) return Vector3(0, 0, 0);
		float f = 1.0 / other;
		return Vector3(x * f, y * f, z * f);
	}

	// Vector specific operations

	// dot product
	float Vector3::dot(const Vector3& other) {
		return x * other.x + y * other.y + z * other.z;
	}

	// squared length, then length, then unit vector
	float Vector3::normsquared() {
		return x * x + y * y + z * z;
	}

	float Vector3::length() {
		return sqrtf(normsquared());
	}

	Vector3 Vector3::normalized() {
		float len = length();
		if (BASE::fzero(len)) return Vector3(1, 0, 0);
		float l_inv = 1.0 / len;
		return Vector3(x * l_inv, y * l_inv, z * l_inv);
	}

	// relative to another vector: cosine of the angle, then angle, then projection, then orthogonal
	float Vector3::cosine(Vector3& other) {
		if (BASE::fzero(length()) || BASE::fzero(other.length())) return 1;
		return dot(other) / (length() * other.length());
	}

	float Vector3::angle(Vector3& other) {
		return acosf(cosine(other));
	}

	Vector3 Vector3::proj(Vector3& other) {
		if (BASE::fzero(other.length())) return Vector3(0, 0, 0);
		return other * (dot(other) / other.normsquared());
	}

	Vector3 Vector3::ortho(Vector3& other) {
		return Vector3(x, y, z) - proj(other);
	}

	// cross products and rotations

	Vector3 Vector3::cross(const Vector3& other) {
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	Vector3 Vector3::normal(const Vector3& other) {
		return cross(other).normalized();
	}

	// Overloading

	Vector3 Vector3::operator+(const Vector3& other) {
		return add(other);
	} 
	Vector3 Vector3::operator-(const Vector3& other) {
		return sub(other);
	} 
	float Vector3::operator*(const Vector3& other) {
		return dot(other);
	} 
	Vector3 Vector3::operator*(const float other) {
		return mul(other);
	} 
	Vector3 Vector3::operator^(const Vector3& other) {
		return cross(other);
	} 
	Vector3 Vector3::operator/(const float other) {
		return div(other);
	} 
	bool Vector3::operator==(const Vector3& other) {
		return BASE::fequal(x, other.x) && BASE::fequal(y, other.y) && BASE::fequal(z, other.z);
	}
	bool Vector3::operator!=(const Vector3& other) {
		return !(BASE::fequal(x, other.x) && BASE::fequal(y, other.y) && BASE::fequal(z, other.z));
	}

	bool Vector3::operator<(const Vector3& other) const {
		if (!BASE::fequal(x, other.x)) return x < other.x;
		if (!BASE::fequal(y, other.y)) return y < other.y;
		return z < other.z;
	}

	// UTIL

	std::string Vector3::to_string() {
		return "Vector3(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}

	Vector4::Vector4() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	Vector4::Vector4(float r, float s, float t, float u) {
		x = r;
		y = s;
		z = t;
		w = u;
	}

	Vector4::Vector4(const Vector4& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	float Vector4::get(int r) {
		if (r == 0) return x;
		if (r == 1) return y;
		if (r == 2) return z;
		return w;
	}

	void Vector4::set(int r, float f) {
		if (r == 0) x = f;
		else if (r == 1) y = f;
		else if (r == 2) z = f;
		else w = f;
	}

	// Operations

	Vector4 Vector4::add(const Vector4& other) {
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vector4 Vector4::inv() {
		return Vector4(-x, -y, -z, -w);
	}

	Vector4 Vector4::sub(const Vector4& other) {
		return Vector4(x - other.x, y - other.y, z -  other.z, w - other.w);
	}

	Vector4 Vector4::mul(float other) {
		return Vector4(x * other, y * other, z * other, w * other);
	}
	Vector4 Vector4::div(float other) {
		if (BASE::fzero(other)) return Vector4(0, 0, 0, 0);
		float f = 1.0 / other;
		return Vector4(x * f, y * f, z * f, w * f);
	}

	// Vector specific operations

	// dot product
	float Vector4::dot(const Vector4& other) {
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	// squared length, then length, then unit vector
	float Vector4::normsquared() {
		return x * x + y * y + z * z + w * w;
	}

	float Vector4::length() {
		return sqrtf(normsquared());
	}

	Vector4 Vector4::normalized() {
		float len = length();
		if (BASE::fzero(len)) return Vector4(1, 0, 0, 0);
		float l_inv = 1.0 / len;
		return Vector4(x * l_inv, y * l_inv, z * l_inv, w * l_inv);
	}

	// relative to another vector: cosine of the angle, then angle, then projection, then orthogonal
	float Vector4::cosine(Vector4& other) {
		if (BASE::fzero(length()) || BASE::fzero(other.length())) return 1;
		return dot(other) / (length() * other.length());
	}

	float Vector4::angle(Vector4& other) {
		return acosf(cosine(other));
	}

	Vector4 Vector4::proj(Vector4& other) {
		if (BASE::fzero(other.length())) return Vector4(0, 0, 0, 0);
		return other * (dot(other) / other.normsquared());
	}

	Vector4 Vector4::ortho(Vector4& other) {
		return Vector4(x, y, z, w) - proj(other);
	}

	// Overloading

	Vector4 Vector4::operator+(const Vector4& other) {
		return add(other);
	} 
	Vector4 Vector4::operator-(const Vector4& other) {
		return sub(other);
	} 
	float Vector4::operator*(const Vector4& other) {
		return dot(other);
	} 
	Vector4 Vector4::operator*(const float other) {
		return mul(other);
	} 
	Vector4 Vector4::operator/(const float other) {
		return div(other);
	} 
	bool Vector4::operator==(const Vector4& other) {
		return BASE::fequal(x, other.x) && BASE::fequal(y, other.y) && BASE::fequal(z, other.z) && BASE::fequal(w, other.w);
	}
	bool Vector4::operator!=(const Vector4& other) {
		return !(BASE::fequal(x, other.x) && BASE::fequal(y, other.y) && BASE::fequal(z, other.z) && BASE::fequal(w, other.w));
	}

	// UTIL

	std::string Vector4::to_string() {
		return "Vector4(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
	}