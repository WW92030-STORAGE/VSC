#ifndef MATH_UTIL2_EXT
#define MATH_UTIL2_EXT

// Math methods that use all custom defined structures

// WARNING - None of these work right now and must be fixed.

#include "Vectors.h"
#include "BASE.h"
#include "Matrices.h"
#include <vector>

// Advanced techniques

static Matrix3 vec3out(Vector3& a, Vector3& b) { // a * T(b)
	return Matrix3(a * b.x, a * b.y, a * b.z);
}

// Generate a Givens rotation for the Matrix3
static std::pair<float, float> GivensRot3(Matrix3& A, int p, int q) {
	if (!BASE::fzero(A.get(p, q))) {
		float r = (A.get(q, q) - A.get(p, p)) / (2.0 * A.get(p, q));
		float t;
		float s1rr = sqrtf(1.0 + r * r);
		if (r >= 0.0) t = 1.0 / (r + s1rr);
		else t = -1.0 / (s1rr - r);

		float c = 1.0 / sqrtf(1.0 + t * t);
		return {c, t * c};
	}

	return {1, 0};
}

// based on Real Time Collision Detection
// Eigenvalues and eigenvectors of a 3x3 real symmetric matrix using Jacobi iteration
static std::pair<Vector3, Matrix3> eigen3_sym(Matrix3 A) {
	constexpr int MAX_ITERS = 64;

	Matrix3 v = Matrix3::eye();

	int i, j, p, q;
	float po, c, s;

	Matrix3 J, b, t;

	for (int iter = 0; iter < MAX_ITERS; iter++) {
		p = 0;
		q = 1;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (i == j) continue;
				if (fabs(A.get(i, j)) > fabs(A.get(p, q))) {
					p = i;
					q = j;
				}
			}
		}

		auto cs = GivensRot3(A, p, q);
		J = Matrix3::eye();
		J.set(p, p, cs.first);
		J.set(p, q, cs.second);
		J.set(q, p, -cs.second);
		J.set(q, q, cs.first);

		v = v * J;
		A = (J.trans() * A) * J;

		float off = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < i; j++) {
				if (i == j) continue;
				off += A.get(i, j) * A.get(i, j);
			}
		}
		if (iter > 2 && off >= po)break;
		po = off; 
	}

	return {Vector3(A.xAxis.x, A.yAxis.y, A.zAxis.z), v};
}

static Matrix3 PCA(std::vector<Vector3>& vv) {
	int n = vv.size();
	float n_inv = 1.0 / n;
	Vector3 mu;
	for (auto v : vv) mu = mu + v;

	mu = mu * n_inv;

	// Covariance matrix
	Matrix3 cov;
	for (auto v : vv) {
		Vector3 pm = v - mu;
		cov = cov + vec3out(pm, pm);
	}
	cov = cov * n_inv;


	return Matrix3();
}


#endif