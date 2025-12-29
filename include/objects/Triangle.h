#ifndef TRIANGLE_EXT
#define TRIANGLE_EXT

/*

``Count up the sides. One, two, three.
That is a triangle that you see.''

-Barney and Friends song

Triangles are formed of three points and represent the plane the three points go through. Triangles can occur in 2d and 3d.
3d triangles form into groups to form meshes and manifolds, while 2d (and Fragment) triangles are more useful for uv mapping and textures.

*/

#include <string>

#include "../utils/math/Vectors.h"
#include "../utils/math/Matrices.h"
#include "../utils/math/Transform.h"
#include "../utils/math/Quaternion.h"

#include "../utils/geometry/Fragment.h"
#include "../utils/geometry/Primitives.h"

class Triangle2 {
	public:
	Vector2 p[3];

	Triangle2();

	Triangle2(Vector2 a, Vector2 b, Vector2 c);

	Triangle2(Vector2 x[3]);
	Triangle2(const Triangle2& other);

	// Orient this triangle so the vertices are in counterclockwise order and the normal has positive z. 2D TRIANGLE ONLY

	void orient();

	// Is a point inside this triangle?
	bool inside(Vector2 P);

	// UTIL

	std::string to_string();
};

class Triangle3 : public Object {
	public:
	Vector3 p[3];
	Vector2 uv[3];
	Vector3 N;

	Triangle3();

	Triangle3(Vector3 a, Vector3 b, Vector3 c);

	Triangle3(Vector3& a, Vector3& b, Vector3& c, Vector2& q, Vector2& r, Vector2 s);

	Triangle3(Vector3 x[3]);

	Triangle3(Vector3 x[3], Vector2 uu[3]);

	Triangle3(const Triangle3& other);

	void flipNormal();

	// The normal is off teh direction based on the counterclockwise orientation of the points. 
	// So it goes on the side of the plane that, when viewing the triangle, p0, p1, p2 are in counterclockwise order.
	Vector3 normal();

	// Is a point inside the infinite prism with cross section this triangle and direction the normal?
	bool inside(Vector3 s);

	float intersectionTime(Line L);

	Vector3 intersection(Line& L);

	void Trans(Transform t) override;

	void ForceTrans(Transform t) override ;

	Vector3 centroid();

	// Comparison -- sort by centroid z position, so that the "largest" triangles have the lowest z.

	bool operator<(const Triangle3& other) const;

	// UTIL

	std::string to_string();
	
	// Barycentric coordinates of a point. **Undefined behavior for points outside the triangle plane

	float area();

	Vector3 bary(Vector3 pos);

	// Interpolate on the vertices

	float interp(Vector3 pos, float f0, float f1, float f2);

	template <typename T>
	T interp(Vector3 pos, T a, T b, T c);

	template <typename T>
	T interp_given_bary(Vector3 pos, T a, T b, T c);
};


	template <typename T>
	T Triangle3::interp(Vector3 pos, T a, T b, T c) {
		Vector3 r = NormSum(bary(pos));
		return a * r.x + b * r.y + c * r.z;
	}

	template <typename T>
	T Triangle3::interp_given_bary(Vector3 r, T a, T b, T c) {
		return a * r.x + b * r.y + c * r.z;
	}


// This is a triangle of FRAGMENTS. It is not the same as a Triangle3.
// This triangle exists in the clip space, while Triangle3 exists in Euclidean space.
class TriangleF {
	public:
	Fragment p[3];
	Vector3 edges[3]; // Edge functions, from p[i] to p[i + 1]
	Vector3 N;
	Vector3 ON;

	BaseMaterial* material;

	bool oriented;

	Vector3 mutualize(Vector3 v);

	void setup_edges();

	void init();

	TriangleF();

	TriangleF(Fragment a, Fragment b, Fragment c, Vector3 n);

	TriangleF(Fragment x[3], Vector3 n);

	TriangleF(const TriangleF& other);

	// Orient this triangle so the vertices are in counterclockwise order and the normal has positive z.

	void orient() ;

	bool inside(Vector2 P) ;

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
	Vector3 bary(int x, int y);

	Vector3 bary2(int x, int y);

	// Interpolate on the vertices

	float interp(int x, int y, float f0, float f1, float f2);

	template <typename T>
	T interp(int x, int y, T a, T b, T c);

	template <typename T>
	T interp_given_bary(Vector3 r, T a, T b, T c, bool ns = false);

	// Comparison -- sort by centroid z position, so that the "largest" triangles have the lowest z.

	bool operator<(const TriangleF& other) const;

	Vector4 centroid();

	// UTIL

	std::string to_string();
};

	template <typename T>
	T TriangleF::interp(int x, int y, T a, T b, T c) {
		Vector3 r = (bary(x, y));
		// return a + (b - a) * r.y + (c - a) * r.z;
		return a * r.x + b * r.y + c * r.z;
	}

	template <typename T>
	T TriangleF::interp_given_bary(Vector3 r, T a, T b, T c, bool ns) {
		if (ns) r = NormSum(r);
		// return a + (b - a) * r.y + (c - a) * r.z;
		return a * r.x + b * r.y + c * r.z;
	}

const Triangle2 NILTRI2 = Triangle2(NILVEC2, NILVEC2, NILVEC2);
const Triangle3 NILTRI3 = Triangle3(NILVEC3, NILVEC3, NILVEC3);
const TriangleF NILTRIF = TriangleF(NILFRAG, NILFRAG, NILFRAG, NILVEC3);

#endif