#ifndef CAMERA_EXT
#define CAMERA_EXT

#include <string>


#include "../utils/geometry/Primitives.h"
#include "../utils/math/Transform.h"
#include "../utils/math/UTIL.h"

class Camera : public Object {
	public:

	float alpha; // FOV
	float epsilon; // Distance to the projection plane

	float n, f; // Distance to the near and far planes

	Plane N, F, U, L, D, R;  // Rubik's cube notation, except F is Far and N is Near.

	Matrix4 frustum;

	// Get axes

	Vector3 right();
	Vector3 up();

	Vector3 look();


	// Initializes a camera at (0, 0, 0) and facing (0, 0, -1) with a given FOV
	// The Camera Space is defined here where X is to the right, Y is up, and Z outwards/backwards.
	// In other words, imagine the camera in a Minecraft world, facing NORTH (-Z).




	// Initialize the planes
	void init();

	Camera();
	Camera(float a);

	Camera(float a, float N, float F);

	Camera(const Camera& other); // also invokes super

	// From a projection matrix. Start with clip space planes and then reverse the frustum matrix to get the end result.
	Camera(Matrix4 m);

	// Transform me

	void Trans(Transform t);

	// Measurements

	// x = +/- (retval) and y = +/- (retval) are the borders of the near plane.
	// similar triangles with the projection plane: 1/epsilon = (ret)/n --> ret = n/epsilon
	float getNearPlaneBorders();

	// Perspective and Depth

	Vector2 rasterize(Vector3 p);

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
	float perserpz(Vector3 p, Vector3 q, float t);

	// Returns the value of t in the projection plane corresponding to the given t value in the line.
	float perserp(Vector3 p, Vector3 q, float t);

	// Interpolates on a vertex: If p, q are the endpoints with scalar attributes b1, b2, this interpolates the projection-wise t interspersed b value.
	// In other words, finds the b3 value for the point p3 between p and q.

	float verp(Vector3 p, Vector3 q, float b1, float b2, float t);

	// !!! Information about frustum moved to ../../src/camera/Camera.cpp

	Matrix4 Frustum();
	// You can generalize this matrix to when the far plane is infinitely distant:

	Matrix4 FrustumNoBack();

	// To get to the fully normalized device coordinates we simply divide all components of the clip point by its w coordinate:
	// Be aware that the w coordinate will NOT be normalized because we seek to encode the depth information as well. 
	
	// This method first converts points to camera space, then NDC. In a way, this serves as the vertex shader to convert from world to NDC.
	Vector4 glFrustum(Vector4 p);

	Vector4 glFrustumNoBack(Vector4 p);

	// Orthographic projections are simpler: simply draw a ray in the z direction to get the projected points

	Matrix4 Ortho() ;

	Vector4 glOrtho(Vector4 p);
};


#endif