/*

``Count up the sides. One, two, three.
That is a triangle that you see.''

-Barney and Friends song

Triangles are formed of three points and represent the plane the three points go through. Triangles can occur in 2d and 3d.
3d triangles form into groups to form meshes and manifolds, while 2d (and Fragment) triangles are more useful for uv mapping and textures.

*/

#include <string>

#include "../../include/utils/math/Vectors.h"
#include "../../include/utils/math/Matrices.h"
#include "../../include/utils/math/Transform.h"
#include "../../include/utils/math/Quaternion.h"

#include "../../include/utils/geometry/Fragment.h"
#include "../../include/utils/geometry/Primitives.h"

#include "../../include/objects/Triangle.h"

	Triangle2::Triangle2() {
		for (int i = 0; i < 3; i++) p[i] = Vector2(i == 1, i == 2);
	}

	Triangle2::Triangle2(Vector2 a, Vector2 b, Vector2 c) {
		p[0] = (a);
		p[1] = (b);
		p[2] = (c);
	}

	Triangle2::Triangle2(Vector2 x[3]) {
		for (int i = 0; i < 3; i++) p[i] = (x[i]);
	}

	Triangle2::Triangle2(const Triangle2& other) {
		for (int i = 0; i < 3; i++) p[i] = (other.p[i]);
	}

	// Orient this triangle so the vertices are in counterclockwise order and the normal has positive z. 2D TRIANGLE ONLY

	void Triangle2::orient() {
		Vector3 e1 = vec3(p[1] - p[0]);
		Vector3 e2 = vec3(p[2] - p[0]);

		if ((e1.cross(e2)).z < 0) {
			Vector2 temp(p[1]);
			p[1] = (p[2]);
			p[2] = temp;
		}
	}

	// Is a point inside this triangle?
	bool Triangle2::inside(Vector2 P) {
		orient();
		// Think of it like this. If a point is inside the triangle then it must be to the left of all three edges, in counterclockwise motion.
		// This means (P[(i + 1) % 3] - P[i]) ^ (P - p[i]) = (Edge) ^ (Point to vertex) has positive z.

		for (int i = 0; i < 3; i++) {
			Vector3 v = vec3(P - p[i]);
			Vector3 e = vec3(p[(i + 1) % 3] - p[i]);
			if ((e.cross(v)).z < 0) return false;
		}
		return true;
	}


	// UTIL

	std::string Triangle2::to_string() {
		std::string res = "Triangle2[";
		for (int i = 0; i < 3; i++) res = res + (i ? ", " : "") + p[i].to_string();
		res = res + "]";
		return res;
	}

	Triangle3::Triangle3() {
		for (int i = 0; i < 3; i++) {
			p[i] = Vector3(i == 0, i == 1, i == 2);
			uv[i] = Vector2();
		}
		N = normal();
	}

	Triangle3::Triangle3(Vector3 a, Vector3 b, Vector3 c) {
		p[0] = (a);
		p[1] = (b);
		p[2] = (c);
		for (int i = 0; i < 3; i++) uv[i] = Vector2();
		N = normal();
	}

	Triangle3::Triangle3(Vector3& a, Vector3& b, Vector3& c, Vector2& q, Vector2& r, Vector2 s) {
		p[0] = (a);
		p[1] = (b);
		p[2] = (c);
		uv[0] = (q);
		uv[1] = (r);
		uv[2] = (s);

		N = normal();
	}

	Triangle3::Triangle3(Vector3 x[3]) {
		for (int i = 0; i < 3; i++) p[i] = (x[i]);
		for (int i = 0; i < 3; i++) uv[i] = Vector2();
		N = normal();
	}

	Triangle3::Triangle3(Vector3 x[3], Vector2 uu[3]) {
		for (int i = 0; i < 3; i++) p[i] = (x[i]);
		for (int i = 0; i < 3; i++) uv[i] = (uu[i]);
		N = normal();
	}

	Triangle3::Triangle3(const Triangle3& other) : Object(other) {
		for (int i = 0; i < 3; i++) {
			p[i] = (other.p[i]);
			uv[i] = (other.uv[i]);
		}
		N = other.N;
	}

	void Triangle3::flipNormal() {
		Vector3 temp(p[0]);
		p[0] = Vector3(p[1]);
		p[1] = temp;
		N = N * -1;
	}

	// The normal is off teh direction based on the counterclockwise orientation of the points. 
	// So it goes on the side of the plane that, when viewing the triangle, p0, p1, p2 are in counterclockwise order.
	Vector3 Triangle3::normal() {
		auto e1 = p[1] - p[0];
		auto e2 = p[2] - p[0];
		return (e1.cross(e2)).normalized();
	}

	// Is a point inside the infinite prism with cross section this triangle and direction the normal? (Most reliable when the point is in the plane of the triangle)
	bool Triangle3::inside(Vector3 s) {
		constexpr bool USE_BARY = true;

		if (USE_BARY) {
			Vector3 b = bary(s);
			return b.x >= 0 && b.y >= 0 && b.z >= 0;
		}

		for (int i = 0; i < 3; i++) {
			Vector3 e1 = p[(i + 1) % 3] - p[i];
			Vector3 e2 = s - p[i];
			Vector3 protrusion = e1.cross(e2);
			if (protrusion * N < 0) return false;
		}
		return true;
	}

	float Triangle3::intersectionTime(Line L) {
		constexpr bool MOLLER = true;

		if (MOLLER) {
			Vector3 e1 = p[1] - p[0];
			Vector3 e2 = p[2] - p[0];

			Vector3 Lxe2 = L.slope.cross(e2);
			float det = e1.dot(Lxe2);

			if (BASE::fzero(det)) return NAN; // parallel to plane = THAT'S NO GOOD

			float det_inv = 1.0 / det;
			Vector3 s = L.point - p[0];
			float u = det_inv * s.dot(Lxe2);

			if (u < 0 || u > 1) return NAN; // First barycentric inside criteria

			Vector3 sxe1 = s.cross(e1);
			float v = det_inv * L.slope.dot(sxe1);
			if (v < 0 || u + v > 1) return NAN; // Second barycentric inside criteria

			return det_inv * e2.dot(sxe1);
		}

		Plane PP(p[0], normal());
		float f = PP.intersectionTime(L);
		if (std::isnan(f)) return NAN;
		Vector3 res = L.get(f);

		if (!inside(res)) return NAN;
		return f;
	}

	Vector3 Triangle3::intersection(Line& L) {
		float f = intersectionTime(L);
		if (std::isnan(f)) return NILVEC3;

		return L.get(f);
	}

	void Triangle3::Trans(Transform t) {
		transform = t * transform;
		Matrix4 trans = t.matrix();
		for (int j = 0; j < 3; j++) p[j] = vec3(trans * fromPoint(p[j]));	
		N = normal();
	}

	void Triangle3::ForceTrans(Transform t) {
		Matrix4 trans = t.matrix();
		for (int j = 0; j < 3; j++) p[j] = vec3(trans * fromPoint(p[j]));	
		N = normal();
	}

	Vector3 Triangle3::centroid() {
		return (p[0] + p[1] + p[2]) / 3.0;
	}

	// Comparison -- sort by centroid z position, so that the "largest" triangles have the lowest z.

	bool Triangle3::operator<(const Triangle3& other) const {
		Vector3 i;
		Vector3 j;
		for (int s = 0; s < 3; s++) {
			i.x += p[s].x;
			i.y += p[s].y;
			i.z += p[s].z;

			j.x += other.p[s].x;
			j.y += other.p[s].y;
			j.z += other.p[s].z;
		}

		if (i.z != j.z) return i.z < j.z;
		if (i.x != j.x) return i.x < j.x;
		return i.y < j.y;
	}

	// UTIL

	std::string Triangle3::to_string() {
		std::string res = "Triangle3[";
		for (int i = 0; i < 3; i++) res = res + (i ? ", " : "") + p[i].to_string();
		res = res + " | " + N.to_string() + "]";
		return res;
	}
	
	// Barycentric coordinates of a point. **Undefined behavior for points outside the triangle plane

	float Triangle3::area() {
		return 0.5 * (p[1] - p[0]).cross(p[2] - p[0]).length();
	}

	// For information on the method used here please read TriangleF::bary...
	Vector3 Triangle3::bary(Vector3 pos) {
		constexpr bool LINALG = true;

		if (LINALG) {
			Matrix2 A2(p[0].x - p[2].x, p[1].x - p[2].x, p[0].y - p[2].y, p[1].y - p[2].y);

			if (!BASE::fzero(A2.det())) {
				Vector2 res = A2.inv() * Vector2(pos.x - p[2].x, pos.y - p[2].y);
				return Vector3(res.x, res.y, 1 - res.x - res.y);
			}

			Matrix3 A(p[0], p[1], p[2]);
			return NormSum(A.solve(pos));
		}

		float ss[3];
		for (int i = 0; i < 3; i++) ss[i] = Triangle3(pos, p[(i + 1) % 3], p[(i + 2) % 3]).area();
		float sum = ss[0] + ss[1] + ss[2];
		if (!BASE::fzero(sum)) {
			for (int i = 0; i < 3; i++) ss[i] /= sum;
		}

		return Vector3(ss[0], ss[1], ss[2]);
	}	

	// Interpolate on the vertices

	float Triangle3::interp(Vector3 pos, float f0, float f1, float f2) {
		Vector3 F__F(f0, f1, f2);
		return NormSum(bary(pos)) * F__F;
	}

// This is a triangle of FRAGMENTS. It is not the same as a Triangle3.
// This triangle exists in the clip space, while Triangle3 exists in Euclidean space.

	Vector3 TriangleF::mutualize(Vector3 v) {
		float mabs = std::max(fabs(v.x), std::max(fabs(v.y), fabs(v.z)));
		if (BASE::fzero(mabs)) return v;
		mabs = 1.0 / mabs;
		return v * mabs;
	}

	void TriangleF::setup_edges() {
		/*
		
		Given a point (x, y) and an edge (x1, y1) --> (x2, y2), we can compute side of (x, y) using cross product:
		Let e = (xe, ye) = (x2 - x1, y2 - y1) and v = (xv, yv) = (x - x1, y - y1). (x, y) is left side of the edge if and only if (xe, ye) cross (xv, yv) is positive:

		(xe, ye) cross (xv, yv) = [xe, yv] - [xv, ye] where [a, b] = a * b for readability

		[x2 - x1, y - y1] - [x - x1, y2 - y1] = [x2, y] - [x1, y] - [x2, y1] + [x1, y1] - [x, y2] + [x1, y2] + [x, y1] - [x1, y1]
		= ([x2, y] - [x1, y]) + ([x, y1] - [x, y2]) + ([x1, y2] - [x2, y1])
		
		We can also scale up and down these coefficients so we divide by the strongest magnitude for stability.

		*/
		for (int i = 0; i < 3; i++) {
			int next = (i + 1) % 3;

			edges[i] = mutualize(Vector3(p[i].ndc.y - p[next].ndc.y, p[next].ndc.x - p[i].ndc.x, p[i].ndc.x * p[next].ndc.y - p[next].ndc.x * p[i].ndc.y));
		}
	}

	void TriangleF::init() {
		auto e1 = (p[1].ndc - p[0].ndc);
		auto e2 = (p[2].ndc - p[0].ndc);
		oriented = e1.x * e2.y - e1.y * e2.x >= 0;
	}

	TriangleF::TriangleF() {
		for (int i = 0; i < 3; i++) p[i] = Fragment(Vector4(i == 0, i == 1, i == 2, 1), Vector3(), 0);
		N = Vector3(1, 1, 1).normalized();
		ON = Vector3(1, 1, 1).normalized();
		init();
	}

	TriangleF::TriangleF(Fragment a, Fragment b, Fragment c, Vector3 n) {
		p[0] = (a);
		p[1] = (b);
		p[2] = (c);
		N = (n);
		ON = (n);
		init();
	}

	TriangleF::TriangleF(Fragment x[3], Vector3 n) {
		for (int i = 0; i < 3; i++) p[i] = (x[i]);
		N = (n);
		ON = (n);
		init();
	}

	TriangleF::TriangleF(const TriangleF& other) {
		for (int i = 0; i < 3; i++) p[i] = (other.p[i]);
		N = (other.N);
		ON = (other.ON);

		material = other.material;
		oriented = other.oriented;

		for (int i = 0; i < 3; i++) edges[i] = other.edges[i];
	}

	// Orient this triangle so the vertices are in counterclockwise order and the normal has positive z.

	void TriangleF::orient() {
		Vector3 e1 = vec3(p[1].ndc - p[0].ndc);
		Vector3 e2 = vec3(p[2].ndc - p[0].ndc);

		e1.z = 0;
		e2.z = 0;

		if ((e1.cross(e2)).z < 0) {
			Fragment temp(p[1]);
			p[1] = Fragment(p[2]);
			p[2] = temp;
		}

		oriented = true;
	}

	bool TriangleF::inside(Vector2 P) {		
		for (int i = 0; i < 3; i++) {
			/*
			Vector3 v = vec3(P - vec2(p[i].ndc));
			Vector3 e = vec3(vec2(p[(i + 1) % 3].ndc - p[i].ndc));
			if ((e.cross(v)).z < 0) return false;
			*/

			// auto v = P - vec2(p[i].ndc);
			// auto e = p[(i + 1) % 3].ndc - p[i].ndc;
			// if (e.x * v.y - e.y * v.x < 0) return false;

			if (P.x * edges[i].x + P.y * edges[i].y + edges[i].z < 0) return false;
		}
		return true;
	}

	// Barycentric coordinates for the (x, y)
	/*
	
	For a triangle with Vector2 vertices p[i] = (x[i], y[i])
	and a given point (x, y) then the barycentric coordinates are S = [s0, s1, s2]
	where (x, y) = s0 p0 + s1 p1 + s2 p2

	This expands to the equations
	x = s0 x0 + s1 x1 + s2 x2
	y = s0 y0 + s1 y1 + s2 y2
	1 = s0    + s1    + s2 // normalization criterion

	Alternatively, if: 
	x = L0 x0 + L1 x1 + L2 x2
	y = L0 y0 + L1 y1 + L2 y2
	then knowing L2 = 1 - L0 - L1 we get
	L0(x0 - x2) + L1(x1 - x2) + x2 - x = 0
	L0(y0 - y2) + L1(y1 - y2) + y2 - y = 0

	or rather:

	[[x0 - x2, x1 - x2], [y0 - y2, y1 - y2]] [L0, L1] = [x - x2, y - y2]

	solve from there
	
	*/
	Vector3 TriangleF::bary(int x, int y) {
		Matrix2 A(p[0].ndc.x - p[2].ndc.x, p[1].ndc.x - p[2].ndc.x, p[0].ndc.y - p[2].ndc.y, p[1].ndc.y - p[2].ndc.y);

		if (BASE::fzero(A.det())) return bary2(x, y);

		Vector2 res = A.inv() * Vector2(x - p[2].ndc.x, y - p[2].ndc.y);

		return Vector3(res.x, res.y, 1 - res.x - res.y);
	}

	Vector3 TriangleF::bary2(int x, int y) {
		Vector3 xxx[3];
		for (int i = 0; i < 3; i++) xxx[i] = Vector3(p[i].ndc.x, p[i].ndc.y, 1);
		Matrix3 A(xxx[0], xxx[1], xxx[2]);

		Vector3 b(x, y, 1);

		return A.solve(b);
	}

	// Interpolate on the vertices

	float TriangleF::interp(int x, int y, float f0, float f1, float f2) {
		Vector3 r = (bary(x, y));
		return f0 + r.y * (f1 - f0) + r.z * (f2 - f0);

		// Vector3 F__F(f0, f1, f2);
		// return r * F__F;
	}

	// Comparison -- sort by centroid z position, so that the "largest" triangles have the lowest z.

	bool TriangleF::operator<(const TriangleF& other) const {
		Vector3 i;
		Vector3 j;
		for (int s = 0; s < 3; s++) {
			i.x += p[s].ndc.x;
			i.y += p[s].ndc.y;
			i.z += p[s].ndc.z;

			j.x += other.p[s].ndc.x;
			j.y += other.p[s].ndc.y;
			j.z += other.p[s].ndc.z;
		}

		if (i.z != j.z) return i.z > j.z;
		if (i.x != j.x) return i.x < j.x;
		return i.y < j.y;
	}

	Vector4 TriangleF::centroid() {
		Vector4 res;
		for (int i = 0; i < 3; i++) res = res + p[i].ndc;
		res = res / 3;
		res.w = 1;
		return res;

	}

	// UTIL

	std::string TriangleF::to_string() {
		std::string res = "TriangleF[";
		for (int i = 0; i < 3; i++) res = res + (i ? ", " : "") + p[i].to_string();
		res = res + " | " + N.to_string() + "]";
		return res;
	}