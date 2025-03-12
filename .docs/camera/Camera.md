# camera/Camera

`class Camera : public Object`

## Description

Camera class that the scene uses as the viewpoint and perspective.

## Notes

- The camera should never move during rasterization. Instead, inverse-transform the drawn objects to match the camera space.

## Fields

- `transform: Transform` = world transform  
- `alpha: float` = Total FOV (both sides)
- `epsilon: float` = Distance to the projection plane, which is a square of radius 1.
- `n, f: float` = Distances to the near and far planes, respectively.
- `N, F, U, L, D, R: Plane` = Placeholder objects for the view frustum planes (will be used in clipping).
- `frustum: Matrix4` = Projection matrix from camera space to un-normalized NDC.

## Constructors

Constructors setup the fields of the camera class, using an OpenGL default transform of origin (0, 0, 0) and look vector (0, 0, -1).

- `Camera()` = `alpha = 90, n = 0.5, f = 64`
- `Camera(float a)` = `alpha = a, n = 0.5, f = 64`
- `Camera(float a, float N, float F)` = `alpha = a, n = N, f = F`

## Functions

- `Vector3 right(), up(), look()` = gets the `x, y, z` normalized basis vectors of the camera space.
- `void init()` = Sets up the camera frustum planes and matrix based on `alpha, epsilon, n, f` values.
- `float getNearPlaneBorders()` = Gets the radius of the near plane (i.e. the side planes intersect at x, y = +/-(retval)).
- `Matrix4 Frustum()` = Assuming default transform, gets the Frustum projection matrix.
- `Matrix4 FrustumNoBack()` = Assuming default transform, Frustum projection matrix except this time the back plane is at infinity.
- `Vector4 glFrustum(Vector4 p)` = multiplies `p` by the Frustum matrix
- `Vector4 glFrustumNoBack(Vector4 p)` = multiplies `p` by the FrustumNoBack matrix
- `Matrix4 Ortho()` = Assuming default transform, gets the Orthogonal projection matrix.
- `Vector4 glOrtho(Vector4 p)` = multiplies `p` by the Orthogonal matrix

## Deprecated/Unused Functions

- `Vector2 rasterize()` = Rasterize a point p onto the projection plane assuming the default transform.
- `float perserpz(Vector3 p, Vector3 q, float t)` = I do not know what this does, apparently it has to do with perspective interpolation?
- `float perserp(Vector3 p, Vector3 q, float t)` = Returns the value `t-prime` corresponding to the line (p, q) projected in the projection plane, so that interpolating using the parameter `t-prime` on the projected line yields the corresponding point to if you interpolated using `t` in camera space. Again, assuming default transform.
- `float verp(Vector3 p, Vector3 q, float b1, float b2, float t)` = Perspective correct interpolation of attributes (b1, b2) on (p, q) respectively. Assumes default transform.