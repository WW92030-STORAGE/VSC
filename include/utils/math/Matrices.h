#ifndef MATRICES_EXT
#define MATRICES_EXT

#include <string>
#include <cstdlib>


#include <utility>

#include "Vectors.h"
#include "UTIL.h"
#include "BASE.h"

// Matrix methods

class Matrix2 {
	public:
	Vector2 xAxis;
	Vector2 yAxis;

	Matrix2();

	Matrix2(Vector2 r, Vector2 s);

	Matrix2(const Matrix2& other);

	// Row major order: input one ROW at a time.
	Matrix2(float r, float s, float t, float u);

	// Identity matrix
	static Matrix2 eye();

	// get a (row, column)
	float get(int a, int b);

	Vector2 getRow(int a);
	// Operations

	Matrix2 add(const Matrix2& other);
	Matrix2 sub(const Matrix2& other);
	Matrix2 mul(float other);
	Matrix2 div(float other);

	// Matrix specific operations

	float det();

	Matrix2 trans();

	Matrix2 inv();

	Vector2 vecmul(const Vector2& other);

	Matrix2 matmul(const Matrix2& other);

	// Solves the system Ax = b
	// If there exist multiple solutions one is given.
	Vector2 solve(Vector2 b);

	Vector2 eigenvalues();

	Matrix2 eigenvectors();
	// overload

	Matrix2 operator+(const Matrix2& other);

	Matrix2 operator-(const Matrix2& other);

	Matrix2 operator*(float other);
	Vector2 operator*(const Vector2& other);
	Matrix2 operator*(const Matrix2& other);
	Matrix2 operator/(float other);
	bool operator==(const Matrix2 other);

	bool operator!=(const Matrix2 other);

	// Utility

	std::string to_string();
};

// 3x3 matrices

class Matrix3 {
	public:
	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;

	Matrix3();

	Matrix3(Vector3 r, Vector3 s, Vector3 t);

	Matrix3(const Matrix3& other);

	// Row major order: input one ROW at a time.
	Matrix3(float a, float b, float c, float d, float e, float f, float g, float h, float i);

	// Identity matrix
	static Matrix3 eye();

	static Matrix3 eye_scaled(float f);

	static Matrix3 diagonal(float a, float b, float c);

	// get a (row, column)
	float get(int a, int b);

	void set(int a, int b, float v);

	Vector3 getCol(int a);

	void setCol(int a, Vector3 v);

	Vector3 getRow(int a);

	void setRow(int a, Vector3 v);

	// Operations

	Matrix3 add(const Matrix3& other);
	Matrix3 sub(const Matrix3& other);
	Matrix3 mul(float other);
	Matrix3 div(float other);

	// Matrix specific operations

	float det();

	Matrix3 trans();

	// Inverse of a 3x3 matrix.
	Matrix3 inv();

	Vector3 vecmul(const Vector3& other);
	Matrix3 matmul(const Matrix3& other);

	// Specialized solve method using Cramer's rule because gprof says this gets called a lot
	Vector3 solve(Vector3 b);

	// Solve a linear system using Elimination and backsub (legacy). 
	// If the system has multiple solutions any of them are given. If no solution exists the NIL vector is returned.
	// To use this elsewhere: copy, set N to the dimension, then replace each Matrix3 and Vector3 with MatrixN and VectorN.
	Vector3 solve2(Vector3 b);

	// TODO - Eigenvalues and Eigenvectors when necessary

	// overload

	Matrix3 operator+(const Matrix3& other);

	Matrix3 operator-(const Matrix3& other);

	Matrix3 operator*(float other);
	Vector3 operator*(const Vector3& other);
	Matrix3 operator*(const Matrix3& other);
	Matrix3 operator/(float other);
	bool operator==(const Matrix3 other);

	bool operator!=(const Matrix3 other);

	// Utility

	std::string to_string();

	std::string sprintf();
};

// 4x4 matrices

class Matrix4 {
	public:
	Vector4 xAxis;
	Vector4 yAxis;
	Vector4 zAxis;
	Vector4 wAxis;

	Matrix4();

	Matrix4(Vector4 r, Vector4 s, Vector4 t, Vector4 u);

	Matrix4(const Matrix4& other);

	// Row major order: input one ROW at a time.
	Matrix4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p);

	// Identity matrix
	static Matrix4 eye();

	// get a (row, column)
	float get(int a, int b);

	void set(int a, int b, float v);

	Vector4 getRow(int a);

	void setRow(int a, Vector4 v);

	// Operations

	Matrix4 add(const Matrix4& other);
	Matrix4 sub(const Matrix4& other);
	Matrix4 mul(float other);
	Matrix4 div(float other);

	// Matrix specific operations

	float det();

	Matrix4 trans() ;

	// Inverse of a 4x4 matrix.
	Matrix4 inv();

	Vector4 vecmul(const Vector4& other);

	Matrix4 matmul(const Matrix4& other);

	// Solve a linear system using Elimination and backsub. 
	// If the system has multiple solutions any of them are given. If no solution exists the NIL vector is returned.
	// To use this elsewhere: copy, set N to the dimension, then replace each Matrix3 and Vector3 with MatrixN and VectorN.
	Vector4 solve(Vector4 b) ;

	// TODO - Eigenvalues and Eigenvectors when necessary

	// overload

	Matrix4 operator+(const Matrix4& other);

	Matrix4 operator-(const Matrix4& other);

	Matrix4 operator*(float other);
	Vector4 operator*(const Vector4& other);
	Matrix4 operator*(const Matrix4& other);
	Matrix4 operator/(float other);
	bool operator==(const Matrix4 other);

	bool operator!=(const Matrix4 other);

	// Utility

	std::string to_string();

	std::string sprintf();
	
};

// Arbitrary size matrices (Largely unused unless extremely needed)

class Matrix {
	public:
	int N, M;
	float** entries;

	Matrix();

	~Matrix();
};

// Constants and Functions

const Matrix2 NILMAT2 = Matrix2(NAN, NAN, NAN, NAN);

const Matrix3 NILMAT3 = Matrix3(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN);

// Rotation matriecs

static Matrix2 Rotation2(float angle) {
	float c = cosf(angle);
	float s = sinf(angle);
	return Matrix2(c, -s, s, c);
}

static Matrix3 Rotation3(Vector3 a, float angle) {
	if (a.normsquared() == 0) return Matrix3::eye();
	a = a.normalized();

	float c = cosf(angle);
	float s = sinf(angle);

	Vector3 xx(c + (1 - c) * a.x * a.x, (1 - c) * a.x * a.y + s * a.z, (1 - c) * a.x * a.z - s * a.y);
	Vector3 yy((1 - c) * a.x * a.y - s * a.z, c + (1 - c) * a.y * a.y, (1 - c) * a.y * a.z + s * a.x);
	Vector3 zz((1 - c) * a.x * a.z + s * a.y, (1 - c) * a.y * a.z - s * a.x, c + (1 - c) * a.z * a.z);

	return Matrix3(xx, yy, zz);
}

#endif