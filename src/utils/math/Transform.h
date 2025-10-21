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
		basis = Matrix3::eye();
	}

	Transform(Vector3 loc) {
		origin = (loc);
		basis = Matrix3::eye();
	}

	Transform(Matrix3 rot) {
		basis = (rot);
	}

	Transform(Vector3 loc, Matrix3 rot) {
		origin = (loc);
		basis = (rot);
	}

	Transform(Matrix4 m) {
		for (int i = 0; i < 3; i++) {
			origin.set(i, m.get(i, 3));
			for (int j = 0; j < 3; j++) basis.set(i, j, m.get(i, j));
		}
	}

	Transform(const Transform& other) {
		origin = (other.origin);
		basis = (other.basis);
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

	inline Transform mul(Transform other) {
		return Transform(matrix().matmul(other.matrix()));
	}

	inline Vector4 mul(Vector4 other) {
		return matrix() * (other);
	}

	inline Vector3 mul(Vector3 other, float p = 1) {
		Vector4 v = vec4(other);
		v.w = p;

		auto res = matrix() * v;
		return vec3(res);
	}

	// inverse transpose

	inline Transform NormalTransform() {
		return Transform(matrix().inv().trans());
	}

	// Overload

	inline Transform operator*(Transform other) {
		return mul(other);
	}

	inline Vector3 operator*(Vector3 other) {
		return mul(other);
	}

	inline Vector4 operator*(Vector4 other) {
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

inline Transform RotationAroundPoint(Vector3 p, Vector3 a, float t) {
	Transform back(p * -1);
	Transform rot(Vector3(), Rotation3(a, t));
	Transform forward(p);

	return forward * rot * back;
}

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