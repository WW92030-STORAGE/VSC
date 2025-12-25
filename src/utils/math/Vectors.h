#ifndef VECTORS_EXT
#define VECTORS_EXT

#include <string>
#include <cmath>
#include <cfloat>
#include <climits>

#include "BASE.h"

// Vector methods

// Vector of 2 integers, used for triangle rasterization

class Vector2i {
	public:
	int x;
	int y;

	Vector2i() {
		x = 0;
		y = 0;
	}

	Vector2i(int r, int s) {
		x = r;
		y = s;
	}

	Vector2i(const Vector2i& other) {
		x = other.x;
		y = other.y;
	}

	std::string to_string() {
		return "Vector2i[" + std::to_string(x) + ", " + std::to_string(y) + "]";
	}
};

class Vector2 {
	public:
	float x;
	float y;

	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2(float r, float s) {
		x = r;
		y = s;
	}

	Vector2(const Vector2& other) {
		x = other.x;
		y = other.y;
	}

	inline float get(int a) {
		if (a == 0) return x;
		return y;
	}

	inline void set(int a, float f) {
		if (a == 0) x = f;
		else y = f;
	}

	// Operations

	inline Vector2 add(const Vector2& other) {
		return Vector2(x + other.x, y + other.y);
	}

	inline Vector2 inv() {
		return Vector2(-x, -y);
	}

	inline Vector2 sub(const Vector2& other) {
		return Vector2(x - other.x, y - other.y);
	}

	inline Vector2 mul(float other) {
		return Vector2(x * other, y * other);
	}
	inline Vector2 div(float other) {
		if (BASE::fzero(other)) return Vector2(0, 0);
		return Vector2(x / other, y / other);
	}

	// Vector specific operations

	// dot product
	inline float dot(const Vector2& other) {
		return x * other.x + y * other.y;
	}

	// squared length, then length, then unit vector
	inline float normsquared() {
		return x * x + y * y;
	}

	inline float length() {
		return sqrtf(normsquared());
	}

	inline Vector2 normalized() {
		float len = length();
		if (BASE::fzero(len)) return Vector2(1, 0);
		return Vector2(x / len, y / len);
	}

	// relative to another vector: cosine of the angle, then angle, then projection, then orthogonal
	inline float cosine(Vector2& other) {
		if (BASE::fzero(length()) || BASE::fzero(other.length())) return 1;
		return dot(other) / (length() * other.length());
	}

	inline float angle(Vector2& other) {
		return acosf(cosine(other));
	}

	inline Vector2 proj(Vector2& other) {
		if (BASE::fzero(other.length())) return Vector2(0, 0);
		return other * (dot(other) / (other.normsquared()));
	}

	inline Vector2 ortho(Vector2& other) {
		return Vector2(x, y) - proj(other);
	}

	// rotations

	inline Vector2 rot(float theta) {
		float c = cosf(theta);
		float s = sinf(theta);
		return Vector2(c * x - s * y, s * x + c * y);
	}

	// Overloading

	inline Vector2 operator+(const Vector2& other) {
		return add(other);
	} 
	inline Vector2 operator-(const Vector2& other) {
		return sub(other);
	} 
	inline float operator*(const Vector2& other) {
		return dot(other);
	} 
	inline Vector2 operator*(const float other) {
		return mul(other);
	} 
	inline Vector2 operator/(const float other) {
		return div(other);
	} 
	inline bool operator==(const Vector2& other) {
		return BASE::fequal(x, other.x) && BASE::fequal(y, other.y);
	} 
	inline bool operator!=(const Vector2& other) {
		return !(BASE::fequal(x, other.x) && BASE::fequal(y, other.y));
	} 
	inline bool operator<(const Vector2& other) const {
		if (!BASE::fequal(x, other.x)) return x < other.x;
		return y < other.y;
	} 

	// UTIL

	inline std::string to_string() {
		return "Vector2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}
};

class Vector3 {
	public:
	float x;
	float y;
	float z;

	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(float r, float s, float t) {
		x = r;
		y = s;
		z = t;
	}

	Vector3(const Vector3& other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}

	inline float get(int r) {
		if (r == 0) return x;
		if (r == 1) return y;
		return z;
	}

	inline void set(int r, float f) {
		if (r == 0) x = f;
		else if (r == 1) y = f;
		else z = f;
	}

	inline void override(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}

	// Operations

	inline Vector3 add(const Vector3& other) {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	inline Vector3 inv() {
		return Vector3(-x, -y, -z);
	}

	inline Vector3 sub(const Vector3& other) {
		return Vector3(x - other.x, y - other.y, z -  other.z);
	}

	inline Vector3 mul(float other) {
		return Vector3(x * other, y * other, z * other);
	}
	inline Vector3 div(float other) {
		if (BASE::fzero(other)) return Vector3(0, 0, 0);
		return Vector3(x / other, y / other, z / other);
	}

	// Vector specific operations

	// dot product
	inline float dot(const Vector3& other) {
		return x * other.x + y * other.y + z * other.z;
	}

	// squared length, then length, then unit vector
	inline float normsquared() {
		return x * x + y * y + z * z;
	}

	inline float length() {
		return sqrtf(normsquared());
	}

	inline Vector3 normalized() {
		float len = length();
		if (BASE::fzero(len)) return Vector3(1, 0, 0);
		return Vector3(x / len, y / len, z / len);
	}

	// relative to another vector: cosine of the angle, then angle, then projection, then orthogonal
	inline float cosine(Vector3& other) {
		if (BASE::fzero(length()) || BASE::fzero(other.length())) return 1;
		return dot(other) / (length() * other.length());
	}

	inline float angle(Vector3& other) {
		return acosf(cosine(other));
	}

	inline Vector3 proj(Vector3& other) {
		if (BASE::fzero(other.length())) return Vector3(0, 0, 0);
		return other * (dot(other) / other.normsquared());
	}

	inline Vector3 ortho(Vector3& other) {
		return Vector3(x, y, z) - proj(other);
	}

	// cross products and rotations

	inline Vector3 cross(const Vector3& other) {
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	inline Vector3 normal(const Vector3& other) {
		return cross(other).normalized();
	}

	// Overloading

	inline Vector3 operator+(const Vector3& other) {
		return add(other);
	} 
	inline Vector3 operator-(const Vector3& other) {
		return sub(other);
	} 
	inline float operator*(const Vector3& other) {
		return dot(other);
	} 
	inline Vector3 operator*(const float other) {
		return mul(other);
	} 
	inline Vector3 operator^(const Vector3& other) {
		return cross(other);
	} 
	inline Vector3 operator/(const float other) {
		return div(other);
	} 
	inline bool operator==(const Vector3& other) {
		return BASE::fequal(x, other.x) && BASE::fequal(y, other.y) && BASE::fequal(z, other.z);
	}
	inline bool operator!=(const Vector3& other) {
		return !(BASE::fequal(x, other.x) && BASE::fequal(y, other.y) && BASE::fequal(z, other.z));
	}

	inline bool operator<(const Vector3& other) const {
		if (!BASE::fequal(x, other.x)) return x < other.x;
		if (!BASE::fequal(y, other.y)) return y < other.y;
		return z < other.z;
	}

	// UTIL

	inline std::string to_string() {
		return "Vector3(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}
};

class Vector4 {
	public:
	float x;
	float y;
	float z;
	float w;

	Vector4() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	Vector4(float r, float s, float t, float u) {
		x = r;
		y = s;
		z = t;
		w = u;
	}

	Vector4(const Vector4& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	inline float get(int r) {
		if (r == 0) return x;
		if (r == 1) return y;
		if (r == 2) return z;
		return w;
	}

	inline void set(int r, float f) {
		if (r == 0) x = f;
		else if (r == 1) y = f;
		else if (r == 2) z = f;
		else w = f;
	}

	// Operations

	inline Vector4 add(const Vector4& other) {
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	inline Vector4 inv() {
		return Vector4(-x, -y, -z, -w);
	}

	inline Vector4 sub(const Vector4& other) {
		return Vector4(x - other.x, y - other.y, z -  other.z, w - other.w);
	}

	inline Vector4 mul(float other) {
		return Vector4(x * other, y * other, z * other, w * other);
	}
	inline Vector4 div(float other) {
		if (BASE::fzero(other)) return Vector4(0, 0, 0, 0);
		return Vector4(x / other, y / other, z / other, w / other);
	}

	// Vector specific operations

	// dot product
	inline float dot(const Vector4& other) {
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	// squared length, then length, then unit vector
	inline float normsquared() {
		return x * x + y * y + z * z + w * w;
	}

	inline float length() {
		return sqrtf(normsquared());
	}

	inline Vector4 normalized() {
		float len = length();
		if (BASE::fzero(len)) return Vector4(1, 0, 0, 0);
		return Vector4(x / len, y / len, z / len, w / len);
	}

	// relative to another vector: cosine of the angle, then angle, then projection, then orthogonal
	inline float cosine(Vector4& other) {
		if (BASE::fzero(length()) || BASE::fzero(other.length())) return 1;
		return dot(other) / (length() * other.length());
	}

	inline float angle(Vector4& other) {
		return acosf(cosine(other));
	}

	inline Vector4 proj(Vector4& other) {
		if (BASE::fzero(other.length())) return Vector4(0, 0, 0, 0);
		return other * (dot(other) / other.normsquared());
	}

	inline Vector4 ortho(Vector4& other) {
		return Vector4(x, y, z, w) - proj(other);
	}

	// Overloading

	inline Vector4 operator+(const Vector4& other) {
		return add(other);
	} 
	inline Vector4 operator-(const Vector4& other) {
		return sub(other);
	} 
	inline float operator*(const Vector4& other) {
		return dot(other);
	} 
	inline Vector4 operator*(const float other) {
		return mul(other);
	} 
	inline Vector4 operator/(const float other) {
		return div(other);
	} 
	inline bool operator==(const Vector4& other) {
		return BASE::fequal(x, other.x) && BASE::fequal(y, other.y) && BASE::fequal(z, other.z) && BASE::fequal(w, other.w);
	}
	inline bool operator!=(const Vector4& other) {
		return !(BASE::fequal(x, other.x) && BASE::fequal(y, other.y) && BASE::fequal(z, other.z) && BASE::fequal(w, other.w));
	}

	// UTIL

	inline std::string to_string() {
		return "Vector4(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
	}
};

// CONSTANTS

Vector2 NILVEC2 = Vector2(NAN, NAN);
Vector3 NILVEC3 = Vector3(NAN, NAN, NAN);
Vector4 NILVEC4 = Vector4(NAN, NAN, NAN, NAN);

Vector3 VEC3_ZERO = Vector3(0, 0, 0);

// FUNCTIONS

// Lerp

inline Vector2 lerp(Vector2& a, Vector2& b, float p) {
	return (a * (1 - p)) + (b * p);
}

inline Vector3 lerp(Vector3& a, Vector3& b, float p) {
	return (a * (1 - p)) + (b * p);
}

inline Vector4 lerp(Vector4& a, Vector4& b, float p) {
	return (a * (1 - p)) + (b * p);
}

// Conversions

inline Vector2 vec2(const Vector3& u) {
	return Vector2(u.x, u.y);
}

inline Vector2 vec2(const Vector4& u) {
	return Vector2(u.x, u.y);
}

inline Vector3 vec3(const Vector2& u) {
	return Vector3(u.x, u.y, 0);
}

inline Vector3 vec3(const Vector4& u) {
	return Vector3(u.x, u.y, u.z);
}

inline Vector4 vec4(const Vector2& u) {
	return Vector4(u.x, u.y, 0, 0);
}

inline Vector4 vec4(const Vector3& u) {
	return Vector4(u.x, u.y, u.z, 0);
}



// From point (w coordinate 1)

inline Vector4 fromPoint(Vector3& u) {
	return Vector4(u.x, u.y, u.z, 1);
}

#endif