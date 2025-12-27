#include <string>
#include "../../../include/utils/Math.h"

#include "../../../include/objects/Object.h"




#include "../../../include/utils/geometry/Primitives.h"

// Geometrical primitives.
// Points are Vector3 (Vector4 with w = 1). Methods are declared from more "complex" forms into more "simple" forms. 
// So to test intersection between line and plane, you must do plane.intersection(line) instead of the other way around.

// Line

	Line::Line() {
		point = Vector3(0, 0, 0);
		slope = Vector3(1, 0, 0);
	}

	Line::Line(Vector3 p, Vector3 s) {
		point = (p);
		slope = (s).normalized();
	}

	Line::Line(const Line& other) {
		point = (other.point);
		slope = (other.slope);
	}

	Vector3 Line::get(float t) {
		return point + (slope * t);
	}

	// Distances

	float Line::squaredDistanceFrom(Vector3& other) {
		Vector3 topoint = other - point;
		return topoint.ortho(slope).normsquared();
	}

	float Line::squaredDistanceFrom(Line& other) {
		if (other.slope == slope) return squaredDistanceFrom(other.point);

		float coeff = pow(slope * other.slope, 2) - slope.normsquared() * other.slope.normsquared();

		Matrix2 A(-(other.slope.normsquared()), slope * other.slope, -(slope * other.slope), slope.normsquared());
		Vector2 x((other.point - point) * slope, (other.point - point) * other.slope);

		Vector2 T = (A * x) / coeff;

		float F = get(T.x).normsquared() + other.get(T.y).normsquared() - 2 * get(T.x).length() * other.get(T.y).length();

		return F;
	}

	float Line::distanceFrom(Vector3& other) {
		return sqrtf(squaredDistanceFrom(other));
	}
	float Line::distanceFrom(Line& other) {
		return sqrtf(squaredDistanceFrom(other));
	}

	// Intersections

	bool Line::intersect(Vector3& other) {
		return BASE::fzero(squaredDistanceFrom(other));
	}
	bool Line::intersect(Line& other) {
		return BASE::fzero(squaredDistanceFrom(other));
	}

	// Utils

	std::string Line::to_string() {
		return "[" + point.to_string() + " : " + slope.to_string() + "]";
	}

// Plane

	Plane::Plane() {
		p = Vector3(0, 0, 0);
		n = Vector3(1, 0, 0);
	}

	Plane::Plane(Vector3 r, Vector3 s) {
		p = (r);
		n = (s).normalized();
	}

	Plane::Plane(const Plane& other) {
		p = (other.p);
		n = (other.n);
	}

	// Standard Form Planes

	// Ax + By + Cz + D = 0 or N @ x + D = 0. D/|N| is the signed distance from origin. If N is normal then D is simply signed distance.
	Vector4 Plane::standardForm() {
		return Vector4(n.x, n.y, n.z, -(p * n));
	}

	// Ax + By + Cz + D = 0
	// intercepts: x_i = -D/A_i
	Plane::Plane(const Vector4& s) {
		n = Vector3(s.x, s.y, s.z);

		if (!BASE::fzero(s.x)) p = Vector3(-s.w / s.x, 0, 0);
		else if (!BASE::fzero(s.y)) p = Vector3(0, -s.w / s.y, 0);
		else if (!BASE::fzero(s.z)) p = Vector3(0, 0, -s.w / s.z);
		else {
			n = Vector3(1, 0, 0);
		}

		n = n.normalized();
	}

	Plane::Plane(float a, float b, float c, float d) {
		n = Vector3(a, b, c);

		if (!BASE::fzero(a)) p = Vector3(-d / a, 0, 0);
		else if (!BASE::fzero(b)) p = Vector3(0, -d / b, 0);
		else if (!BASE::fzero(c)) p = Vector3(0, 0, -d / c);
		else {
			p = Vector3();
			n = Vector3(1, 0, 0);
		}

		n = n.normalized();
	}

	// Get the projection of a vector into the plane

	Vector3 Plane::proj(Vector3 other) {
		return other.ortho(n);
	}

	// Distances

	// signed distance from origin. This is the D value in the standard form, assuming n is normalized
	float Plane::distanceFromOrigin() {
		return -(n * p);
	}

	// Distance from Point
	float Plane::distanceFrom(const Vector3& q) {
		return distanceFromOrigin() + (n.normalized() * q);
	}

	float Plane::distanceFrom(const Vector4& q) {
		if (!BASE::fequal(q.w, 1)) return distanceFrom(vec3(q));
		return standardForm() * q;
	}

	// Intersections

	bool Plane::contains(const Vector3& p) {
		return BASE::fzero(distanceFrom(p));
	}

	float Plane::intersectionTime(Line& L) {
		float ndotl = n * L.slope;
		if (BASE::fzero(ndotl)) {
			if (contains(L.point)) return 0;
			return NAN;
		}

		return ( (p - L.point) * n) / ndotl;
	}

	Vector3 Plane::intersection(Line& L) {
		if (BASE::fzero(n * L.slope)) {
			if (contains(L.point)) return L.point;
			return NILVEC3;
		}

		float T = intersectionTime(L);
		return L.get(T);
	}


	
	// 3 planes (gives one point of the intersection)
	Vector3 Plane::intersection(Plane& A, Plane& B) {
		Matrix3 M(n.x, n.y, n.z, A.n.x, A.n.y, A.n.z, B.n.x, B.n.y, B.n.z);
		Vector3 b(-distanceFromOrigin(), -A.distanceFromOrigin(), -B.distanceFromOrigin()); 

		return M.solve(b);
	}

	// 2 planes (gives one line of the intersection)
	Line Plane::intersection(Plane& A) {
		if (BASE::fzero(n * A.n)) { // Parallel planes
			if (BASE::fzero(distanceFrom(A.p))) {
				Vector3 test = proj(Vector3(1, 0, 0));
				if (!isZero(test)) return Line(p, test);
				test = proj(Vector3(0, 1, 0));
				if (!isZero(test)) return Line(p, test);
				test = proj(Vector3(0, 0, 1));
				if (!isZero(test)) return Line(p, test);
			}
		}

		Vector3 slop = n.cross(A.n);

		Vector4 sf = standardForm();
		Vector4 tf = A.standardForm();

		Matrix3 system(sf.x, sf.y, sf.z, tf.x, tf.y, tf.z, 0, 0, 0);
		Vector3 b(-sf.w, -tf.w, 0);

		Vector3 x = system.solve(b);

		return Line(x, slop);
	}

	// Transform a plane

	Plane Plane::transform_(Transform T) {
		Matrix4 U = T.matrix().inv().trans();
		return Plane(U * standardForm());
	}

	// Re-initialize this plane
	void Plane::reinit(Vector4 s) {
		n = Vector3(s.x, s.y, s.z);

		if (!BASE::fzero(s.x)) p = Vector3(-s.w / s.x, 0, 0);
		else if (!BASE::fzero(s.y)) p = Vector3(0, -s.w / s.y, 0);
		else if (!BASE::fzero(s.z)) p = Vector3(0, 0, -s.w / s.z);
		else {
			p = Vector3();
			n = Vector3(1, 0, 0);
		}

		n = n.normalized();
	}
	
	void Plane::Trans(Transform T) {
		transform = transform * T;
		Matrix4 U = T.matrix().inv().trans();
		reinit(U * standardForm());
	}

	void Plane::ForceTrans(Transform T) {
		Matrix4 U = T.matrix().inv().trans();
		reinit(U * standardForm());
	}

	// Utils

	std::string Plane::to_string() {
		return "[" + p.to_string() + " : " + n.to_string() + " | " + std::to_string(distanceFromOrigin()) + "]";
	}