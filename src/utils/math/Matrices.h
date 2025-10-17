#ifndef MATRICES_EXT
#define MATRICES_EXT

// #include <iostream>
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

	Matrix2() {
		xAxis = Vector2();
		yAxis = Vector2();
	}

	Matrix2(Vector2 r, Vector2 s) {
		xAxis = Vector2(r);
		yAxis = Vector2(s);
	}

	Matrix2(const Matrix2& other) {
		xAxis = Vector2(other.xAxis);
		yAxis = Vector2(other.yAxis);
	}

	// Row major order: input one ROW at a time.
	Matrix2(float r, float s, float t, float u) {
		xAxis = Vector2(r, t);
		yAxis = Vector2(s, u);
	}

	// Identity matrix
	inline static Matrix2 eye() {
		return Matrix2(1, 0, 0, 1);
	}

	// get a (row, column)
	inline float get(int a, int b) {
		if (b == 0) {
			return a ? xAxis.y : xAxis.x;
		}
		else return a ? yAxis.y : yAxis.x;
	}

	inline Vector2 getRow(int a) {
		if (a == 0) return Vector2(xAxis.x, yAxis.x);
		return Vector2(xAxis.y, yAxis.y);
	}

	// Operations

	inline Matrix2 add(const Matrix2& other) {
		return Matrix2(xAxis + other.xAxis, yAxis + other.yAxis);
	}
	inline Matrix2 sub(const Matrix2& other) {
		return Matrix2(xAxis - other.xAxis, yAxis - other.yAxis);
	}
	inline Matrix2 mul(float other) {
		return Matrix2(xAxis * other, yAxis * other);
	}
	inline Matrix2 div(float other) {
		if (BASE::fzero(other)) return eye();
		return Matrix2(xAxis / other, yAxis / other);
	}

	// Matrix specific operations

	inline float det() {
		return xAxis.x * yAxis.y - xAxis.y * yAxis.x;
	}

	inline Matrix2 trans() {
		return Matrix2(xAxis.x, xAxis.y, yAxis.x, yAxis.y);
	}

	inline Matrix2 inv() {
		float d = det();
		if (BASE::fzero(d)) return eye();
		return Matrix2(yAxis.y, -yAxis.x, -xAxis.y, xAxis.x).div(d); // sowwy 3:
	}

	inline Vector2 vecmul(const Vector2& other) {
		return Vector2(getRow(0).dot(other), getRow(1).dot(other));
	}

	inline Matrix2 matmul(const Matrix2& other) {
		return Matrix2(vecmul(other.xAxis), vecmul(other.yAxis));
	}

	// Solves the system Ax = b
	// If there exist multiple solutions one is given.
	Vector2 solve(Vector2 b) {
		// const bool DEBUG = false;


		Matrix2 A = Matrix2(*this);
		// if (DEBUG) std::cout << "SOLVING " << A.to_string() << "x = " << b.to_string() << std::endl;
		// check the first column
		if (!BASE::fzero(A.xAxis.x) && !BASE::fzero(A.xAxis.y)); // top left is nonzero, bottom left is zero: do nothing
		// if the top value is less in magnitude than the bottom value
		else if (fabs(A.xAxis.x) < fabs(A.yAxis.y)) {
			std::swap(A.xAxis.x, A.xAxis.y);
			std::swap(A.yAxis.x, A.yAxis.y);
			std::swap(b.x, b.y);
		}

		// Take care of the bottom value by reducing it to zero. This guarantees the bottom left value is zero.
		if (!BASE::fzero(A.xAxis.x)) {
			float coeff = A.xAxis.y / A.xAxis.x;
			A.xAxis.y = 0;
			A.yAxis.y -= A.yAxis.x * coeff;
			b.y -= b.x * coeff;
		}
		// if (DEBUG) std::cout << "AFTER CLEARING ROW1 " << A.to_string() << std::endl;

		// if the [1][1] value is nonzero use it to reduce the [0][1]

		if (!BASE::fzero(A.yAxis.y)) {
			float coeff = A.yAxis.x / A.yAxis.y;
			A.yAxis.x = 0;
			b.x -= b.y * coeff;
		}

		// if (DEBUG) std::cout << "AFTER CLEARING ROW2 " << A.to_string() << std::endl;

		// So now the off diagonal entries are guaranteed to be zero if possible.

		// Shunt the coefficients down so you get the solutions

		// WARNING

		// If there exist multiple solutions you will get one of them
		// if some row is zero then this might just happen
		// But if both rows are zero then all vector solutions are solutions unless b is nonzero reduced

		Vector2 res = NILVEC2;

		if (isZero(A.xAxis) && isZero(A.yAxis)) {
			if (isZero(b)) res = Vector2(1, 1);
		}

		// Row 1 is zero (y values in the vectors. This means the Y component of the solution is variable)
		else if (BASE::fzero(A.xAxis.y) && BASE::fzero(A.yAxis.y)) {
			if (BASE::fzero(b.y)) res = Vector2((b.x - A.yAxis.x) / A.xAxis.x, 1); // Multiple solutions
			// A[0][0]x + A[0][1]y = B[0]
		}
		// Row 0 is zero (x axis in the vectors. This means the X component is variable)
		else if (BASE::fzero(A.xAxis.x) && BASE::fzero(A.yAxis.x)) {
			if (BASE::fzero(b.x)) res = Vector2(1, (b.y - A.xAxis.y) / A.yAxis.y); // Multiple solutions
			// A[1][0]x + A[1][1]y = B[1]
		}

		// Otherwise the matrix is now diagonal so we can simply shunt the coefficients down.

		else res = Vector2(b.x / A.xAxis.x, b.y / A.yAxis.y);

		// if (DEBUG) std::cout << "SOLUTION " << res.to_string() << std::endl;

		return res;
	}

	Vector2 eigenvalues() {
		// Let the values be (a, b; c, d)
		// A - xI = (a - x, b; c, d - x)
		// det = (a - x)(d - x) - bc = (ad - bc) - (a + d)x + x^2

		float b = -(xAxis.x + yAxis.y);
		float c = det();

		float disc = b * b - 4 * c;
		if (disc < 0) return NILVEC2; // complex eigenvalues do later

		// https://people.csail.mit.edu/bkph/articles/Quadratics.pdf
		return quadratic(1, b, c);
	}

	Matrix2 eigenvectors() {
		Vector2 vals = eigenvalues();
		if (vals == NILVEC2) return Matrix2(NAN, NAN, NAN, NAN);

		Vector2 s = (sub(eye() * vals.x)).solve(Vector2(0, 0));
		Vector2 t = (sub(eye() * vals.y)).solve(Vector2(0, 0));
		return Matrix2(s, t);

	}

	// overload

	inline Matrix2 operator+(const Matrix2& other) {
		return add(other);
	}

	inline Matrix2 operator-(const Matrix2& other) {
		return sub(other);
	}

	inline Matrix2 operator*(float other) {
		return mul(other);
	}
	inline Vector2 operator*(const Vector2& other) {
		return vecmul(other);
	}
	inline Matrix2 operator*(const Matrix2& other) {
		return matmul(other);
	}
	inline Matrix2 operator/(float other) {
		return div(other);
	}
	inline bool operator==(const Matrix2 other) {
		return xAxis == other.xAxis && yAxis == other.yAxis;
	}

	inline bool operator!=(const Matrix2 other) {
		return !(xAxis == other.xAxis && yAxis == other.yAxis);
	}

	// Utility

	inline std::string to_string() {
		std::string res = "[" + std::to_string(xAxis.x) + ", " + std::to_string(yAxis.x) + "; ";
		res = res + std::to_string(xAxis.y) + ", " + std::to_string(yAxis.y) + "]";
		return res;
	}
};

// 3x3 matrices

class Matrix3 {
	public:
	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;

	Matrix3() {
		xAxis = Vector3();
		yAxis = Vector3();
		zAxis = Vector3();
	}

	Matrix3(Vector3 r, Vector3 s, Vector3 t) {
		xAxis = Vector3(r);
		yAxis = Vector3(s);
		zAxis = Vector3(t);
	}

	Matrix3(const Matrix3& other) {
		xAxis = Vector3(other.xAxis);
		yAxis = Vector3(other.yAxis);
		zAxis = Vector3(other.zAxis);
	}

	// Row major order: input one ROW at a time.
	Matrix3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
		xAxis = Vector3(a, d, g);
		yAxis = Vector3(b, e, h);
		zAxis = Vector3(c, f, i);
	}

	// Identity matrix
	inline static Matrix3 eye() {
		return Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);
	}

	// get a (row, column)
	float get(int a, int b) {
		if (b == 0) {
			if (a == 0) return xAxis.x;
			if (a == 1) return xAxis.y;
			return xAxis.z;
		}
		if (b == 1) {
			if (a == 0) return yAxis.x;
			if (a == 1) return yAxis.y;
			return yAxis.z;
		}
		else {
			if (a == 0) return zAxis.x;
			if (a == 1) return zAxis.y;
			return zAxis.z;
		}
	}

	void set(int a, int b, float v) {
		if (b == 0) {
			if (a == 0) xAxis.x = v;
			else if (a == 1) xAxis.y = v;
			else xAxis.z = v;
		}
		if (b == 1) {
			if (a == 0) yAxis.x = v;
			else if (a == 1) yAxis.y = v;
			else yAxis.z = v;
		}
		else {
			if (a == 0) zAxis.x = v;
			else if (a == 1) zAxis.y = v;
			else zAxis.z = v;
		}
	}

	Vector3 getCol(int a) {
		if (a == 0) return Vector3(xAxis);
		if (a == 1) return Vector3(yAxis);
		return Vector3(zAxis);
	}

	void setCol(int a, Vector3 v) {
		if (a == 0) xAxis = Vector3(v);
		else if (a == 1) yAxis = Vector3(v);
		else zAxis = Vector3(v);
	}

	Vector3 getRow(int a) {
		if (a == 0) return Vector3(xAxis.x, yAxis.x, zAxis.x);
		if (a == 1) return Vector3(xAxis.y, yAxis.y, zAxis.y);
		return Vector3(xAxis.z, yAxis.z, zAxis.z);
	}

	void setRow(int a, Vector3 v) {
		set(a, 0, v.x);
		set(a, 1, v.y);
		set(a, 2, v.z);
	}

	// Operations

	inline Matrix3 add(const Matrix3& other) {
		return Matrix3(xAxis + other.xAxis, yAxis + other.yAxis, zAxis + other.zAxis);
	}
	inline Matrix3 sub(const Matrix3& other) {
		return Matrix3(xAxis - other.xAxis, yAxis - other.yAxis, zAxis - other.zAxis);
	}
	inline Matrix3 mul(float other) {
		return Matrix3(xAxis * other, yAxis * other, zAxis * other);
	}
	inline Matrix3 div(float other) {
		if (other == 0) return eye();
		return Matrix3(xAxis / other, yAxis / other, zAxis / other);
	}

	// Matrix specific operations

	inline float det() {
		float p1 = xAxis.x * (yAxis.y * zAxis.z - yAxis.z * zAxis.y);
		float p2 = yAxis.x * (zAxis.y * xAxis.z - zAxis.z * xAxis.y);
		float p3 = zAxis.x * (xAxis.y * yAxis.z - xAxis.z * yAxis.y);
		return p1 + p2 + p3;
	}

	inline Matrix3 trans() {
		return Matrix3(xAxis.x, xAxis.y, xAxis.z, yAxis.x, yAxis.y, yAxis.z, zAxis.x, zAxis.y, zAxis.z);
	}

	// Inverse of a 3x3 matrix.
	Matrix3 inv() {
		const int N = 3;
		Matrix3 M(*this);
		float d = det();
		if (BASE::fzero(d)) return eye();
		
		// gaussian elimination to get the output matrix

		Matrix3 res = eye();

		// std::cout << "LEFT\n" << M.sprintf() << " | RIGHT\n" << res.sprintf() << std::endl;

		for (int j = 0; j < N; j++) {
			int i = j;
			float ss = 0;
			for (int ii = j; ii < N; ii++) {
				if (fabs(M.get(ii, j)) > ss) {
					ss = fabs(M.get(ii, j));
					i = ii;
				}
			}
			if (BASE::fzero(ss)) return eye();
			if (i != j) {
				Vector3 temp = M.getRow(i);
				M.setRow(i, M.getRow(j));
				M.setRow(j, temp);
				
				temp = res.getRow(i);
				res.setRow(i, res.getRow(j));
				res.setRow(j, temp);
			}

			float diag = M.get(j, j);
			M.setRow(j, M.getRow(j) / diag);
			res.setRow(j, res.getRow(j) / diag);
			for (int r = 0; r < N; r++) {
				if (r == j) continue;
				float coeff = -M.get(r, j);
				M.setRow(r, M.getRow(r) + M.getRow(j) * coeff);
				res.setRow(r, res.getRow(r) + res.getRow(j) * coeff);
			}
		}
		// std::cout << "LEFT\n" << M.sprintf() << " | RIGHT\n" << res.sprintf() << std::endl;
		return res;
	}

	inline Vector3 vecmul(const Vector3& other) {
		return Vector3(getRow(0).dot(other), getRow(1).dot(other), getRow(2).dot(other));
	}

	inline Matrix3 matmul(const Matrix3& other) {
		return Matrix3(vecmul(other.xAxis), vecmul(other.yAxis), vecmul(other.zAxis));
	}

	// Specialized solve method using Cramer's rule because gprof says this gets called a lot
	Vector3 solve(Vector3 b) {
		float deter = det();
		if (BASE::fzero(deter)) return solve2(b);

		Matrix3 A(*this);
		Vector3 res(0, 0, 0);

		for (int i = 0; i < 3; i++) {
			Vector3 temp = A.getCol(i);
			A.setCol(i, b);
			res.set(i, A.det() / deter);
			A.setCol(i, temp);
		}

		return res;
	}

	// Solve a linear system using Elimination and backsub (legacy). 
	// If the system has multiple solutions any of them are given. If no solution exists the NIL vector is returned.
	// To use this elsewhere: copy, set N to the dimension, then replace each Matrix3 and Vector3 with MatrixN and VectorN.
	Vector3 solve2(Vector3 b) {
		// const bool DEBUG = false;
		Matrix3 A(*this);
		int N = 3;

		int i = 0;
		int j = 0;
		for (j = 0; j < N; j++) {
			// if (DEBUG) std::cout << "BEFORE COL " << j << " = \n" << A.sprintf() << "\n";
			int k = -1;
			float minim = 0;
			for (int ii = i; ii < N; ii++) {
				if (fabs(A.get(ii, j)) > fabs(minim)) {
					minim = fabs(A.get(ii, j));
					k = ii;
				}
			}
			if (k < 0) continue;
			if (k != i) {
				Vector3 temp = A.getRow(k);
				A.setRow(k, A.getRow(i));
				A.setRow(i, temp);

				float tem = b.get(k);
				b.set(k, b.get(i));
				b.set(i, tem);
			}
			float val = A.get(i, j);
			A.setRow(i, A.getRow(i) / val);
			b.set(i, b.get(i) / val);

			for (int r = 0; r < N; r++) {
				if (r == i) continue;
				float coeff = -A.get(r, j);
				A.setRow(r, A.getRow(r) + A.getRow(i) * coeff);
				b.set(r, b.get(r) + b.get(i) * coeff);
			}

			i++;
		}

		// The end result is an upper triangular matrix. You can then backsub variables starting from the end.

		// Finish backsubbing

		Vector3 res(b);

		// if (DEBUG) std::cout << "END " << A.sprintf() << "\n" << res.to_string() << "\n";

		bool* frvr = (bool*)(malloc(sizeof(bool) * N));
		for (int c = 0; c < N; c++) {
			bool fff = true;
			for (int r = N - 1; r >= 0; r--) {
				if (!BASE::fzero(A.get(r, c))) {
					if (c <= 0 || BASE::fzero(A.get(r, c - 1))) {
						fff = false;
					}
					break;
				}
			}
			frvr[c] = fff;
			if (fff) {
				if (BASE::fzero(b.get(c))) res.set(c, 1);
				else return NILVEC3;
			}
		}

		// Row rank = column rank
		// Backsubstitute non-free variables using the nonzero rows
		int row = N - 1;
		for (int i = N - 1; i >= 0; i--) {
			if (frvr[i]) continue;
			while (isZero(A.getRow(row))) row--;

			float val = res.get(i);
			for (int k = i + 1; k < N; k++) {
				val -= A.get(row, k) * res.get(k);
			}
			res.set(i, val / A.get(row, i));
			row--;
		}
		free(frvr);

		return res;
	}

	// TODO - Eigenvalues and Eigenvectors when necessary

	// overload

	inline Matrix3 operator+(const Matrix3& other) {
		return add(other);
	}

	inline Matrix3 operator-(const Matrix3& other) {
		return sub(other);
	}

	inline Matrix3 operator*(float other) {
		return mul(other);
	}
	inline Vector3 operator*(const Vector3& other) {
		return vecmul(other);
	}
	inline Matrix3 operator*(const Matrix3& other) {
		return matmul(other);
	}
	inline Matrix3 operator/(float other) {
		return div(other);
	}
	inline bool operator==(const Matrix3 other) {
		return xAxis == other.xAxis && yAxis == other.yAxis && zAxis == other.zAxis;
	}

	inline bool operator!=(const Matrix3 other) {
		return !(xAxis == other.xAxis && yAxis == other.yAxis && zAxis == other.zAxis);
	}

	// Utility

	inline std::string to_string() {
		std::string res = "[" + std::to_string(xAxis.x) + ", " + std::to_string(yAxis.x) + ", " + std::to_string(zAxis.x) + "; ";
		res = res + std::to_string(xAxis.y) + ", " + std::to_string(yAxis.y) + ", " + std::to_string(zAxis.y) + "; ";
		res = res + std::to_string(xAxis.z) + ", " + std::to_string(yAxis.z) + ", " + std::to_string(zAxis.z) + "]";
		return res;
	}

	inline std::string sprintf() {
		std::string res = "[" + std::to_string(xAxis.x) + ", " + std::to_string(yAxis.x) + ", " + std::to_string(zAxis.x) + "; \n";
		res = res + std::to_string(xAxis.y) + ", " + std::to_string(yAxis.y) + ", " + std::to_string(zAxis.y) + "; \n";
		res = res + std::to_string(xAxis.z) + ", " + std::to_string(yAxis.z) + ", " + std::to_string(zAxis.z) + "]";
		return res;
	}
};

// 4x4 matrices

class Matrix4 {
	public:
	Vector4 xAxis;
	Vector4 yAxis;
	Vector4 zAxis;
	Vector4 wAxis;

	Matrix4() {
		xAxis = Vector4();
		yAxis = Vector4();
		zAxis = Vector4();
		wAxis = Vector4();
	}

	Matrix4(Vector4 r, Vector4 s, Vector4 t, Vector4 u) {
		xAxis = Vector4(r);
		yAxis = Vector4(s);
		zAxis = Vector4(t);
		wAxis = Vector4(u);
	}

	Matrix4(const Matrix4& other) {
		xAxis = Vector4(other.xAxis);
		yAxis = Vector4(other.yAxis);
		zAxis = Vector4(other.zAxis);
		wAxis = Vector4(other.wAxis);
	}

	// Row major order: input one ROW at a time.
	Matrix4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p) {
		xAxis = Vector4(a, e, i, m);
		yAxis = Vector4(b, f, j, n);
		zAxis = Vector4(c, g, k, o);
		wAxis = Vector4(d, h, l, p);
	}

	// Identity matrix
	inline static Matrix4 eye() {
		Matrix4 res;
		for (int i = 0; i < 4; i++) res.set(i, i, 1);
		return res;
	}

	// get a (row, column)
	float get(int a, int b) {
		if (b == 0) {
			if (a == 0) return xAxis.x;
			if (a == 1) return xAxis.y;
			if (a == 2) return xAxis.z;
			return xAxis.w;
		}
		else if (b == 1) {
			if (a == 0) return yAxis.x;
			if (a == 1) return yAxis.y;
			if (a == 2) return yAxis.z;
			return yAxis.w;
		}
		else if (b == 2) {
			if (a == 0) return zAxis.x;
			if (a == 1) return zAxis.y;
			if (a == 2) return zAxis.z;
			return zAxis.w;
		}
		else {
			if (a == 0) return wAxis.x;
			if (a == 1) return wAxis.y;
			if (a == 2) return wAxis.z;
			return wAxis.w;
		}
	}

	void set(int a, int b, float v) {
		if (b == 0) {
			if (a == 0) xAxis.x = v;
			else if (a == 1) xAxis.y = v;
			else if (a == 2) xAxis.z = v;
			else xAxis.w = v;
		}
		else if (b == 1) {
			if (a == 0) yAxis.x = v;
			else if (a == 1) yAxis.y = v;
			else if (a == 2) yAxis.z = v;
			else yAxis.w = v;
		}
		else if (b == 2) {
			if (a == 0) zAxis.x = v;
			else if (a == 1) zAxis.y = v;
			else if (a == 2) zAxis.z = v;
			else zAxis.w = v;
		}
		else {
			if (a == 0) wAxis.x = v;
			else if (a == 1) wAxis.y = v;
			else if (a == 2) wAxis.z = v;
			else wAxis.w = v;
		}
	}

	Vector4 getRow(int a) {
		if (a == 0) return Vector4(xAxis.x, yAxis.x, zAxis.x, wAxis.x);
		if (a == 1) return Vector4(xAxis.y, yAxis.y, zAxis.y, wAxis.y);
		if (a == 2) return Vector4(xAxis.z, yAxis.z, zAxis.z, wAxis.z);
		return Vector4(xAxis.w, yAxis.w, zAxis.w, wAxis.w);
	}

	void setRow(int a, Vector4 v) {
		set(a, 0, v.x);
		set(a, 1, v.y);
		set(a, 2, v.z);
		set(a, 3, v.w);
	}

	// Operations

	inline Matrix4 add(const Matrix4& other) {
		return Matrix4(xAxis + other.xAxis, yAxis + other.yAxis, zAxis + other.zAxis, wAxis + other.wAxis);
	}
	inline Matrix4 sub(const Matrix4& other) {
		return Matrix4(xAxis - other.xAxis, yAxis - other.yAxis, zAxis - other.zAxis, wAxis - other.wAxis);
	}
	inline Matrix4 mul(float other) {
		return Matrix4(xAxis * other, yAxis * other, zAxis * other, wAxis * other);
	}
	inline Matrix4 div(float other) {
		if (other == 0) return eye();
		return Matrix4(xAxis / other, yAxis / other, zAxis / other, wAxis / other);
	}

	// Matrix specific operations

	float det() {
		Matrix4 A(*this);
		const int N = 4;
		// const bool DEBUG = false;

		float res = 1;

		int i = 0;
		int j = 0;
		for (j = 0; j < N; j++) {
			// if (DEBUG) std::cout << "BEFORE COL " << j << " = \n" << A.sprintf() << "\n";
			int k = -1;
			float minim = 0;
			for (int ii = i; ii < N; ii++) {
				if (fabs(A.get(ii, j)) > fabs(minim)) {
					minim = fabs(A.get(ii, j));
					k = ii;
				}
			}
			if (k < 0) continue;
			if (k != i) {
				Vector4 temp = A.getRow(k);
				A.setRow(k, A.getRow(i));
				A.setRow(i, temp);

				res = -res;
			}
			float val = A.get(i, j);
			A.setRow(i, A.getRow(i) / val);
			res *= val;

			for (int r = 0; r < N; r++) {
				if (r == i) continue;
				float coeff = -A.get(r, j);
				A.setRow(r, A.getRow(r) + A.getRow(i) * coeff);
			}

			i++;
		}

		for (int i = 0; i < N; i++) res *= A.get(i, i);
		return res;
	}

	Matrix4 trans() {
		Matrix4 res;
		const int N = 4;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) res.set(i, j, get(j, i));
		}
		return res;
	}

	// Inverse of a 4x4 matrix.
	Matrix4 inv() {
		const int N = 4;
		Matrix4 M(*this);
		float d = det();
		if (BASE::fzero(d)) return eye();
		
		// gaussian elimination to get the output matrix

		Matrix4 res = eye();

		// std::cout << "LEFT\n" << M.sprintf() << " | RIGHT\n" << res.sprintf() << std::endl;

		for (int j = 0; j < N; j++) { // Loop through the columns j from 0 to N - 1
			int i = j;
			float ss = 0;
			for (int ii = j; ii < N; ii++) { // Find the row i >= j where |M(i, j)| has the largest absolute value
				if (fabs(M.get(ii, j)) > ss) {
					ss = fabs(M.get(ii, j));
					i = ii;
				}
			}
			if (BASE::fzero(ss)) return eye(); // If no nonzero element exists in this range then it is not invertible.
			if (i != j) { // If i != j then swap rows i, j to place a nonzero element on the diagonal.
				Vector4 temp = M.getRow(i);
				M.setRow(i, M.getRow(j));
				M.setRow(j, temp);
				
				temp = res.getRow(i);
				res.setRow(i, res.getRow(j));
				res.setRow(j, temp);
			}

			float diag = M.get(j, j);
			M.setRow(j, M.getRow(j) / diag);
			res.setRow(j, res.getRow(j) / diag); // Divide row j by its diagonal element.
			for (int r = 0; r < N; r++) { // For row r from 0 to N - 1...
				if (r == j) continue; // and r != j...
				float coeff = -M.get(r, j); // Add coeff * row j to row r.
				M.setRow(r, M.getRow(r) + M.getRow(j) * coeff);
				res.setRow(r, res.getRow(r) + res.getRow(j) * coeff);
			}

			// std::cout << j << " | LEFT\n" << M.sprintf() << " | RIGHT\n" << res.sprintf() << std::endl;
		}
		// std::cout << "LEFT\n" << M.sprintf() << " | RIGHT\n" << res.sprintf() << std::endl;
		return res;
	}

	inline Vector4 vecmul(const Vector4& other) {
		Vector4 res;
		const int N = 4;
		for (int i = 0; i < N; i++) res.set(i, getRow(i).dot(other));
		return res;
	}

	inline Matrix4 matmul(const Matrix4& other) {
		return Matrix4(vecmul(other.xAxis), vecmul(other.yAxis), vecmul(other.zAxis), vecmul(other.wAxis));
	}

	// Solve a linear system using Elimination and backsub. 
	// If the system has multiple solutions any of them are given. If no solution exists the NIL vector is returned.
	// To use this elsewhere: copy, set N to the dimension, then replace each Matrix3 and Vector3 with MatrixN and VectorN.
	Vector4 solve(Vector4 b) {
		// const bool DEBUG = false;
		Matrix4 A(*this);
		int N = 4;

		int i = 0;
		int j = 0;
		for (j = 0; j < N; j++) {
			// if (DEBUG) std::cout << "BEFORE COL " << j << " = \n" << A.sprintf() << "\n";
			int k = -1;
			float minim = 0;
			for (int ii = i; ii < N; ii++) {
				if (fabs(A.get(ii, j)) > fabs(minim)) {
					minim = fabs(A.get(ii, j));
					k = ii;
				}
			}
			if (k < 0) continue;
			if (k != i) {
				Vector4 temp = A.getRow(k);
				A.setRow(k, A.getRow(i));
				A.setRow(i, temp);

				float tem = b.get(k);
				b.set(k, b.get(i));
				b.set(i, tem);
			}
			float val = A.get(i, j);
			A.setRow(i, A.getRow(i) / val);
			b.set(i, b.get(i) / val);

			for (int r = 0; r < N; r++) {
				if (r == i) continue;
				float coeff = -A.get(r, j);
				A.setRow(r, A.getRow(r) + A.getRow(i) * coeff);
				b.set(r, b.get(r) + b.get(i) * coeff);
			}

			i++;
		}

		// The end result is an upper triangular matrix. You can then backsub variables starting from the end.

		// Finish backsubbing

		Vector4 res(b);

		// if (DEBUG) std::cout << "END " << A.sprintf() << "\n" << res.to_string() << "\n";

		bool* frvr = (bool*)(malloc(sizeof(bool) * N));
		for (int c = 0; c < N; c++) {
			bool fff = true;
			for (int r = N - 1; r >= 0; r--) {
				if (!BASE::fzero(A.get(r, c))) {
					if (c <= 0 || BASE::fzero(A.get(r, c - 1))) {
						fff = false;
					}
					break;
				}
			}
			frvr[c] = fff;
			if (fff) {
				if (BASE::fzero(b.get(c))) res.set(c, 1);
				else return NILVEC4;
			}
		}

		// Row rank = column rank
		// Backsubstitute non-free variables using the nonzero rows
		int row = N - 1;
		for (int i = N - 1; i >= 0; i--) {
			if (frvr[i]) continue;
			while (isZero(A.getRow(row))) row--;

			float val = res.get(i);
			for (int k = i + 1; k < N; k++) {
				val -= A.get(row, k) * res.get(k);
			}
			res.set(i, val / A.get(row, i));
			row--;
		}
		free(frvr);

		return res;
	}

	// TODO - Eigenvalues and Eigenvectors when necessary

	// overload

	inline Matrix4 operator+(const Matrix4& other) {
		return add(other);
	}

	inline Matrix4 operator-(const Matrix4& other) {
		return sub(other);
	}

	inline Matrix4 operator*(float other) {
		return mul(other);
	}
	inline Vector4 operator*(const Vector4& other) {
		return vecmul(other);
	}
	inline Matrix4 operator*(const Matrix4& other) {
		return matmul(other);
	}
	inline Matrix4 operator/(float other) {
		return div(other);
	}
	inline bool operator==(const Matrix4 other) {
		return xAxis == other.xAxis && yAxis == other.yAxis && zAxis == other.zAxis && wAxis == other.wAxis;
	}

	inline bool operator!=(const Matrix4 other) {
		return !(xAxis == other.xAxis && yAxis == other.yAxis && zAxis == other.zAxis && wAxis == other.wAxis);
	}

	// Utility

	inline std::string to_string() {
		std::string res = "[";
		const int N = 4;
		for (int i = 0; i < N; i++) {
			if (i > 0) res = res + "; ";
			for (int j = 0; j < N; j++) {
				if (j > 0) res = res + ", ";
				res = res + std::to_string(get(i, j));
			}
		}
		res = res + "]";
		return res;
	}

	inline std::string sprintf() {
		std::string res = "[";
		const int N = 4;
		for (int i = 0; i < N; i++) {
			if (i > 0) res = res + "; \n";
			for (int j = 0; j < N; j++) {
				if (j > 0) res = res + ", ";
				res = res + std::to_string(get(i, j));
			}
		}
		res = res + "]";
		return res;
	}
	
};

// Arbitrary size matrices (Largely unused unless extremely needed)

class Matrix {
	public:
	int N, M;
	float** entries;

	Matrix() {
		N = 2;
		M = 2;
		entries = (float**)malloc(sizeof(float*) * N);
		for (int i = 0; i < N; i++) entries[i] = (float*)malloc(sizeof(float) * M);
	}

	~Matrix() {
		for (int i = 0; i < N; i++) free(entries[i]);
		free(entries);
	}
};

// Constants and Functions

Matrix2 NILMAT2 = Matrix2(NAN, NAN, NAN, NAN);

Matrix3 NILMAT3 = Matrix3(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN);

// Rotation matriecs

Matrix2 Rotation2(float angle) {
	float c = cosf(angle);
	float s = sinf(angle);
	return Matrix2(c, -s, s, c);
}

Matrix3 Rotation3(Vector3 a, float angle) {
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