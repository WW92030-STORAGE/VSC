#ifndef TRANSFORM_EXT
#define TRANSFORM_EXT


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

	Transform();

	Transform(Vector3 loc);

	Transform(Matrix3 rot);

	Transform(Vector3 loc, Matrix3 rot);

	Transform(Matrix4 m);

	Transform(const Transform& other);

	Matrix4 matrix();

	Transform inv();

	Transform mul(Transform other);

	Vector4 mul(Vector4 other);

	Vector3 mul(Vector3 other, float p = 1);

	// inverse transpose

	Transform NormalTransform();

	// Overload

	Transform operator*(Transform other);

	Vector3 operator*(Vector3 other);

	Vector4 operator*(Vector4 other);

	// Utility

	std::string to_string();

	std::string sprintf();
};

static Transform Rotation(Vector3 a, float t) {
	return Transform(Vector3(), Rotation3(a, t));
}

static Transform RotationAroundPoint(Vector3 p, Vector3 a, float t) {
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
static Transform NormalTransform(Transform M) {
	return Transform(M.matrix().inv().trans());
}

#endif