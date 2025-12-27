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

	Vector2i();

	Vector2i(int r, int s);

	Vector2i(const Vector2i& other);

	std::string to_string();
};

class Vector2 {
	public:
	float x;
	float y;

	Vector2();
	Vector2(float r, float s);

	Vector2(const Vector2& other);

	float get(int a);

	void set(int a, float f);

	// Operations

	Vector2 add(const Vector2& other);

	Vector2 inv();

	Vector2 sub(const Vector2& other);

	Vector2 mul(float other);
	Vector2 div(float other);

	// Vector specific operations

	// dot product
	float dot(const Vector2& other);

	// squared length, then length, then unit vector
	float normsquared();

	float length();

	Vector2 normalized();

	// relative to another vector: cosine of the angle, then angle, then projection, then orthogonal
	float cosine(Vector2& other);

	float angle(Vector2& other);

	Vector2 proj(Vector2& other);

	Vector2 ortho(Vector2& other);

	// rotations

	Vector2 rot(float theta);

	// Overloading

	Vector2 operator+(const Vector2& other);

	Vector2 operator-(const Vector2& other);
	float operator*(const Vector2& other);
	Vector2 operator*(const float other);
	Vector2 operator/(const float other);
	bool operator==(const Vector2& other);
	bool operator!=(const Vector2& other);
	bool operator<(const Vector2& other) const;

	// UTIL

	std::string to_string();
};

class Vector3 {
	public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float r, float s, float t);

	Vector3(const Vector3& other);

	float get(int r);

	void set(int r, float f);

	void override(float a, float b, float c);

	// Operations

	Vector3 add(const Vector3& other);

	Vector3 inv();

	Vector3 sub(const Vector3& other);

	Vector3 mul(float other);
	Vector3 div(float other);

	// Vector specific operations

	// dot product
	float dot(const Vector3& other);

	// squared length, then length, then unit vector
	float normsquared();

	float length();

	Vector3 normalized();

	// relative to another vector: cosine of the angle, then angle, then projection, then orthogonal
	float cosine(Vector3& other);

	float angle(Vector3& other);

	Vector3 proj(Vector3& other);

	Vector3 ortho(Vector3& other);

	// cross products and rotations

	Vector3 cross(const Vector3& other);

	Vector3 normal(const Vector3& other);

	// Overloading

	Vector3 operator+(const Vector3& other);
	Vector3 operator-(const Vector3& other);
	float operator*(const Vector3& other);
	Vector3 operator*(const float other);
	Vector3 operator^(const Vector3& other);
	Vector3 operator/(const float other);
	bool operator==(const Vector3& other);
	bool operator!=(const Vector3& other);

	bool operator<(const Vector3& other) const;

	// UTIL

	std::string to_string();
};

class Vector4 {
	public:
	float x;
	float y;
	float z;
	float w;

	Vector4();
	Vector4(float r, float s, float t, float u);

	Vector4(const Vector4& other);

	float get(int r);

	void set(int r, float f);

	// Operations

	Vector4 add(const Vector4& other);

	Vector4 inv();

	Vector4 sub(const Vector4& other);

	Vector4 mul(float other);
	Vector4 div(float other);

	// Vector specific operations

	// dot product
	float dot(const Vector4& other);

	// squared length, then length, then unit vector
	float normsquared();

	float length();

	Vector4 normalized();

	// relative to another vector: cosine of the angle, then angle, then projection, then orthogonal
	float cosine(Vector4& other);

	float angle(Vector4& other);

	Vector4 proj(Vector4& other);

	Vector4 ortho(Vector4& other);

	// Overloading

	Vector4 operator+(const Vector4& other);
	Vector4 operator-(const Vector4& other);
	float operator*(const Vector4& other);
	Vector4 operator*(const float other);
	Vector4 operator/(const float other);
	bool operator==(const Vector4& other);
	bool operator!=(const Vector4& other);

	// UTIL

	std::string to_string();
};

// CONSTANTS

const Vector2 NILVEC2 = Vector2(NAN, NAN);
const Vector3 NILVEC3 = Vector3(NAN, NAN, NAN);
const Vector4 NILVEC4 = Vector4(NAN, NAN, NAN, NAN);

const Vector3 VEC3_ZERO = Vector3(0, 0, 0);

// FUNCTIONS

// Lerp

static Vector2 lerp(Vector2& a, Vector2& b, float p) {
	return (a * (1 - p)) + (b * p);
}

static Vector3 lerp(Vector3& a, Vector3& b, float p) {
	return (a * (1 - p)) + (b * p);
}

static Vector4 lerp(Vector4& a, Vector4& b, float p) {
	return (a * (1 - p)) + (b * p);
}

// Conversions

static Vector2 vec2(const Vector3& u) {
	return Vector2(u.x, u.y);
}

static Vector2 vec2(const Vector4& u) {
	return Vector2(u.x, u.y);
}

static Vector3 vec3(const Vector2& u) {
	return Vector3(u.x, u.y, 0);
}

static Vector3 vec3(const Vector4& u) {
	return Vector3(u.x, u.y, u.z);
}

static Vector4 vec4(const Vector2& u) {
	return Vector4(u.x, u.y, 0, 0);
}

static Vector4 vec4(const Vector3& u) {
	return Vector4(u.x, u.y, u.z, 0);
}



// From point (w coordinate 1)

static Vector4 fromPoint(Vector3& u) {
	return Vector4(u.x, u.y, u.z, 1);
}

#endif