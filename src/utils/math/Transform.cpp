#include <string>

#include "../../../include/utils/math/Vectors.h"
#include "../../../include/utils/math/Matrices.h"
#include "../../../include/utils/math/BASE.h"
#include "../../../include/utils/math/UTIL.h"



#include "../../../include/utils/math/Transform.h"

// 3D Transforms

	Transform::Transform() {
		basis = Matrix3::eye();
	}

	Transform::Transform(Vector3 loc) {
		origin = (loc);
		basis = Matrix3::eye();
	}

	Transform::Transform(Matrix3 rot) {
		basis = (rot);
	}

	Transform::Transform(Vector3 loc, Matrix3 rot) {
		origin = (loc);
		basis = (rot);
	}

	Transform::Transform(Matrix4 m) {
		for (int i = 0; i < 3; i++) {
			origin.set(i, m.get(i, 3));
			for (int j = 0; j < 3; j++) basis.set(i, j, m.get(i, j));
		}
	}

	Transform::Transform(const Transform& other) {
		origin = (other.origin);
		basis = (other.basis);
	}

	Matrix4 Transform::matrix() {
		Matrix4 res;
		for (int i = 0; i < 3; i++) {
			res.set(i, 3, origin.get(i));
			for (int j = 0; j < 3; j++) res.set(i, j, basis.get(i, j));
		}
		res.set(3, 3, 1);
		return res;
	}

	Transform Transform::inv() {
		return Transform(matrix().inv());
	}

	Transform Transform::mul(Transform other) {
		return Transform(matrix().matmul(other.matrix()));
	}

	Vector4 Transform::mul(Vector4 other) {
		return matrix() * (other);
	}

	Vector3 Transform::mul(Vector3 other, float p) {
		Vector4 v = vec4(other);
		v.w = p;

		auto res = matrix() * v;
		return vec3(res);
	}

	// inverse transpose

	Transform Transform::NormalTransform() {
		return Transform(matrix().inv().trans());
	}

	// Overload

	Transform Transform::operator*(Transform other) {
		return mul(other);
	}

	Vector3 Transform::operator*(Vector3 other) {
		return mul(other);
	}

	Vector4 Transform::operator*(Vector4 other) {
		return mul(other);
	}

	// Utility

	std::string Transform::to_string() {
		std::string res = origin.to_string() + " | " + basis.to_string();
		return res;
	}

	std::string Transform::sprintf() {
		std::string res = "" + origin.to_string() + "\n" + basis.sprintf();
		return res;
	}