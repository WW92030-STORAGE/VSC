#ifndef CAMERA_EXT
#define CAMERA_EXT

// #include <iostream>
#include <string>


#include "../utils/geometry/Primitives.h"
#include "../utils/math/Transform.h"
#include "../utils/math/UTIL.h"

class Camera : public Object {
	public:
	Transform transform;

	float alpha; // FOV
	float epsilon; // Distance to the projection plane

	float n, f; // Distance to the near and far planes

	Plane N, F, U, L, D, R;

	Matrix4 frustum;

	// Get axes

	inline Vector3 right() {
		return Vector3(transform.basis.xAxis);
	}

	inline Vector3 up() {
		return Vector3(transform.basis.yAxis);
	}

	inline Vector3 look() {
		return Vector3(transform.basis.zAxis);
	}


	// Initializes a camera at (0, 0, 0) and facing (0, 0, -1) with a given FOV
	// The Camera Space is defined here where X is to the right, Y is up, and Z outwards/backwards.
	// In other words, imagine the camera in a Minecraft world, facing NORTH (-Z).




	// Initialize the planes
	inline void init() {
		float ss = sqrtf(1 + epsilon * epsilon);
		N = Plane(0, 0, -1, -n);
		F = Plane(0, 0, 1, f);

		U = Plane(-1, -epsilon / ss, -1 / ss, 0);
		L = Plane(epsilon / ss, 0, -1 / ss, 0);
		D = Plane(0, epsilon / ss, -1 / ss, 0);
		R = Plane(0, -epsilon / ss, -1 / ss, 0);

		frustum = Frustum();

		transform = Transform();
	}

	Camera() {
		alpha = 90;
		
		n = 0.5;
		f = 64;

		epsilon = 1.0 / tanf(alpha * 0.5);

		init();
	}

	Camera(float a) {
		alpha = a;
		
		n = 0.5;
		f = 64;

		epsilon = 1.0 / tanf(alpha * 0.5);

		init();
	}

	Camera(float a, float N, float F) {
		alpha = a;
		n = N;
		f = F;

		epsilon = 1.0 / tanf(alpha * 0.5);

		init();
	}

	Camera(Camera& other) {
		alpha = other.alpha;
		n = other.n;
		f = other.f;
		epsilon = 1.0 / tanf(alpha * 0.5);
		init();
	}

	// From a projection matrix. Start with clip space planes and then reverse the frustum matrix to get the end result.
	Camera(Matrix4 m) {
		frustum = Matrix4(m);

		auto m1 = m.getRow(0);
		auto m2 = m.getRow(1);
		auto m3 = m.getRow(2);
		auto m4 = m.getRow(3);

		N = Plane(m4 + m3);
		F = Plane(m4 - m3);
		U = Plane(m4 - m2);
		L = Plane(m4 + m1);
		D = Plane(m4 + m2);
		R = Plane(m4 - m1);

		n = -((m4 + m3).w);
		f = (m4 - m3).w;

		// D.y = epsilon / (1 + epsilon * epsilon)
		// D.y + D.y * epsilon * epsilon = epsilon
		// D.y * epsilon * epsilon - epsilon + D.y = 0

		float d = D.standardForm().y;
		epsilon = quadratic(d, -1, d).y;

		// epsilon = 1.0 / tanf(alpha * 0.5)
		// 1.0 / epsilon = tanf(alpha * 0.5)
		// atanf(1.0 / epsilon) = alpha * 0.5

		alpha = 2.0 * atanf(1.0 / epsilon);
	}

	// Transform me

	inline void Trans(Transform t) {
		transform = t * transform;
	}

	// Measurements

	// x = +/- (retval) and y = +/- (retval) are the borders of the near plane.
	// similar triangles with the projection plane: 1/epsilon = (ret)/n --> ret = n/epsilon
	inline float getNearPlaneBorders() {
		if (BASE::fzero(epsilon)) return 0;
		return n / epsilon;
	}

	// Perspective and Depth

	inline Vector2 rasterize(Vector3 p) {
		if (p.z > -n) return NILVEC2; // Invisible because too in front
		if (p.z < -f) return NILVEC2; // Invisible because too far back

		// Draw the line from (0, 0, 0) to (x, y, z)
		// The projection plane exists at distance epsilon from the camera.
		// so scale by epsilon / |p.z|

		return Vector2(p.x, p.y) * epsilon / fabs(p.z);
	}

	// To rasterize a line, each pixel is sampled as a ray travelling through points (x, y, -e) where (x, y) are equally spaced points on the projection plane.
	// However this does not translate well onto the actual line itself, as the ray does not mark out equally spaced points on hte line.

	// For a point (x, y, z < -n) on the line, its rasterization is p = (p.x, p.y, p.z). 
	// Let the line be defined by two planes: ax + bz = c, Ay + Bz = C where c and C are nonzero so the lkine does not pass thru the (0, y, 0) line.
	// And using p.x/x = -e/z and p.y/y = -e/z to get x = -p.x z/e, y = -p.y z/e.

	// Substitute: (b - a p.x / e)z = c, (B - A p.y / e)z = C.
	// Or: 1/z = (-a p.x / ce) + (b / c) = (-A p.y / Ce) + (B / C)

	// Let (x1, y1, z1) and (x2, y2, z2) be the endpoints of the line, and (p1.x, p1.y, -e) and (p2.x, p2.y, -e) be the rasterizations.
	// To linearly interpolate we find a third point (p3.x, p3.y, -e) = ... (1 - t)(p1) + t(p2). We seek the corresponding point (x3, y3, z3) in the line.

	// Some manipulation gives us that if p3 = (1 - t)p1 + t(p2) then the relation of the z coordinates is 1/z3 = (1 - t)(1/z1) + t(1/z2).

	// How do we do this? Define another interp function to handle this, so the z = 1/(RHS)

	// Get the z value
	inline float perserpz(Vector3 p, Vector3 q, float t) {
		if (BASE::fequal(p.z, q.z)) return p.z;
		if (BASE::fzero(p.z) || BASE::fzero(q.z)) return 0;
		float zrep = (1 - t) / p.z + t / q.z;

		if (BASE::fzero(zrep)) return 0;
		float z = 1 / zrep;

		return z;
	}

	// Returns the value of t in the projection plane corresponding to the given t value in the line.
	inline float perserp(Vector3 p, Vector3 q, float t) {
		if (BASE::fequal(p.z, q.z)) return t;
		if (BASE::fzero(p.z) || BASE::fzero(q.z)) return 0;
		float zrep = (1 - t) / p.z + t / q.z;

		if (BASE::fzero(zrep)) return 0;
		float z = 1 / zrep;

		float tt = (z - p.z) / (q.z - p.z);
		return tt;
	}

	// Interpolates on a vertex: If p, q are the endpoints with scalar attributes b1, b2, this interpolates the projection-wise t interspersed b value.
	// In other words, finds the b3 value for the point p3 between p and q.

	inline float verp(Vector3 p, Vector3 q, float b1, float b2, float t) {
		if (BASE::fzero(p.z) || BASE::fzero(q.z)) return BASE::lerp(b1, b2, t);

		float res = (1 - t) * b1 / p.z + t * b2 / q.z;
		return perserpz(p, q, t) * res;
	}

	// Now all of this is fine and dandy but we'd rather just use one transform to do it all. 
	// This transform is not preserving of angles or distances, but rather maps Euclidean (cam) space (view frustum) to the clipping space, projected into the viewport.
	// This means that instead of light beams converging at the position of the camera, the light beams now become parallel, going in the direction of the near plane.
	// The transform uses the w coordinate to store the original z coordinate. 

	// Begin with a point P = (P.x, P.y, P.z, 1). The projected point onto the near plane is p = (p.x, p.y, p.z = -n, p.w) = (-n * P.x/P.z, -n * P.y/P.z, -n, p.w).
	// Now let's further establish the HOMOGENOUS CLIP SPACE as having coordinates bounded by the borders x, y, z = +/-1.
	// We seek a transform from the view frustum (a six sided 3d shape) to the clip space (also a six sided 3d shape).
	// Let U, L, D, R be the upper, left, down, and right borders of the near plane (here actually just +/- n/e). Also let W = R - L and H = U - D be the dimensions.
	// So from our projected point p = (-nP.x/P.z, -nP.y/P.z, -n. p.w) we obtain coordinates for the clipped point p':
	// p'.x = x' = (p.x - L) * 2 / W + 1
	// p'.y = y' = (p.y - D) * 2 / H + 1

	// But of course, p.x, p.y were derived from values from the original point P. So we obtain:
	// x' = (2 * n / W) * (-P.x / P.z) - (R + L) / W
	// y' = (2 * n / H) * (-P.y / P.z) - (U + D) / H

	// z coordinates interpolate via reciprocal, so construct the function p'.z = z' = A/p.z + B
	// Using the initial values z'(n) = -1, z'(f) = 1 gives z' = [ (2nf) / (f - n) ](-1/p.z) + [f + n] / [f - n]

	// Isolating a common denominator and further manipulation gives us the following transform:

	inline Matrix4 Frustum() {
		float R = n / epsilon;
		float L = -n / epsilon;
		float U = n / epsilon;
		float D = -n / epsilon;

		float RLS = R + L;
		float RLD = R - L;
		float UDS = U + D;
		float UDD = U - D;
		float FNS = f + n;
		float FND = f - n;

		Vector4 xx(2 * n / RLD, 0, 0, 0);
		Vector4 yy(0, 2 * n / UDD, 0, 0);
		Vector4 zz(RLS / RLD, UDS / UDD, -1 * FNS / FND, -1);
		Vector4 ww(0, 0, -2 * n * f / FND, 0);

		return Matrix4(xx, yy, zz, ww);

		// The projected point p' is given as MP where, again, P = (x, y, z, 1) = (point, location).
		// It contains the coordinates of clip space, and the w component which is the negative of the original z (You can see this via the sole -1 in the last row).
	}

	// You can generalize this matrix to when the far plane is infinitely distant:

	inline Matrix4 FrustumNoBack() {
		float R = n / epsilon;
		float L = -n / epsilon;
		float U = n / epsilon;
		float D = -n / epsilon;

		float RLS = R + L;
		float RLD = R - L;
		float UDS = U + D;
		float UDD = U - D;

		Vector4 xx(2 * n / RLD, 0, 0, 0);
		Vector4 yy(0, 2 * n / UDD, 0, 0);
		Vector4 zz(RLS / RLD, UDS / UDD, -1, -1);
		Vector4 ww(0, 0, -2 * n, 0);

		return Matrix4(xx, yy, zz, ww);
	}

	// To get to the fully normalized device coordinates we simply divide all components of the clip point by its w coordinate:

	inline Vector4 glFrustum(Vector4 p) {
		Vector4 res = Frustum() * transform.inv().matrix() * p;
		if (BASE::fzero(res.w)) return res;
		float w = res.w;
		res = res / res.w;
		res.w = w;
		return res;
	}

	inline Vector4 glFrustumNoBack(Vector4 p) {
		Vector4 res = FrustumNoBack() * p;
		if (BASE::fzero(res.w)) return res;
		float w = res.w;
		res = res / w;
		res.w = w;
		return res;
	}

	// Orthographic projections are simpler: simply draw a ray in the z direction to get the projected points

	inline Matrix4 Ortho() {
		float R = n / epsilon;
		float L = -n / epsilon;
		float U = n / epsilon;
		float D = -n / epsilon;

		float RLS = R + L;
		float RLD = R - L;
		float UDS = U + D;
		float UDD = U - D;
		float FNS = f + n;
		float FND = f - n;

		Vector4 xx(2 / RLD, 0, 0, 0);
		Vector4 yy(0, 2 / UDD, 0, 0);
		Vector4 zz(0, 0, -2 / FND, 0);
		Vector4 ww(-RLS / RLD, -UDS / UDD, -FNS / FND, 1);

		return Matrix4(xx, yy, zz, ww);
	}

	inline Vector4 glOrtho(Vector4 p) {
		Vector4 res = Ortho() * p;
		return res;
	}
};


#endif