#ifndef TRANSFORM_EXT
#define TRANSFORM_EXT

// #include <iostream>

#include <string>

#include "Vectors.h"
#include "Matrices.h"
#include "BASE.h"
#include "UTIL.h"

// 3D Transforms

class Transform {
	public:
	Vector3 origin; // location, the affine component
	Matrix3 basis; // rotation, the linear component

	Transform() {
		origin = Vector3();
		basis = Matrix3::eye();
	}

	Transform(Vector3 loc) {
		origin = Vector3(loc);
		basis = Matrix3::eye();
	}

	Transform(Vector3 loc, Matrix3 rot) {
		origin = Vector3(loc);
		basis = Matrix3(rot);
	}

	Transform(Matrix4 m) {
		origin = Vector3();
		basis = Matrix3();
		for (int i = 0; i < 3; i++) {
			origin.set(i, m.get(i, 3));
			for (int j = 0; j < 3; j++) basis.set(i, j, m.get(i, j));
		}
	}

	Transform(const Transform& other) {
		origin = Vector3(other.origin);
		basis = Matrix3(other.basis);
	}

	inline Matrix4 matrix() {
		Matrix4 res;
		for (int i = 0; i < 3; i++) {
			res.set(i, 3, origin.get(i));
			for (int j = 0; j < 3; j++) res.set(i, j, basis.get(i, j));
		}
		res.set(3, 3, 1);
		return res;
	}

	inline Transform inv() {
		return Transform(matrix().inv());
	}

	inline Transform mul(Transform& other) {
		return Transform(matrix().matmul(other.matrix()));
	}

	// inverse transpose

	inline Transform NormalTransform() {
		return Transform(matrix().inv().trans());
	}

	// Overload

	inline Transform operator*(Transform& other) {
		return mul(other);
	}

	// Utility

	inline std::string to_string() {
		std::string res = origin.to_string() + " | " + basis.to_string();
		return res;
	}

	inline std::string sprintf() {
		std::string res = "" + origin.to_string() + "\n" + basis.sprintf();
		return res;
	}
};

// Obtain a transform for a normal vector N, assuming the tangent vector T goes through the M transform
/*

N @ T = 0
Let T' = MT
We want N' @ T' = 0
Let G be the corresponding transform for N

N' @ T' = (GN) @ (MT) = 0
= (GN)^T @ (MT)
= N^T @ G^T @ M @ T

Given N^T @ T = dot(N, T) = 0
this means (G^T @ M) = I and so G = (M^-1)^T
*/
inline Transform NormalTransform(Transform M) {
	return Transform(M.matrix().inv().trans());
}

#endif