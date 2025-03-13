# utils/geometry/Primitives/Line

`class Line`

## Description

Line class representing a parametric line with origin and direction.

## Notes

- This is similar to the `Ray` class.

## Fields

- `point, slope: Vector3` = (Ray origin/reference point), (Normalized ray direction) i.e. `r(t) = point + slope * t`

## Constructors

- `Line()` = Default origin (0, 0, 0) direction (1, 0, 0)
- `Line(Vector3 p, Vector3 s)`

## Functions

- `Vector3 get(float t)` = Gets the point at time `t` aka. `point + slope * t`
- `float squaredDistanceFrom(Vector3& other)` = Squared minimum distance from a point
- `float squaredDistanceFrom(Line& other)` = Squared minimum distance from another line
- `float distanceFrom(Vector3& other)`
- `float distanceFrom(Line& other)`
- `bool intersect(Vector3& other)` = Intersects a point?
- `bool intersect(Line& other)` = Intersects a line?
- `std::string to_string()`

---

# utils/geometry/Primitives/Plane

`class Plane : public Object`

## Description

Plane class representing a plane with reference point and normal vector.

## Fields

- `p, n: Vector3` = (Reference point, Normal vector)

## Constructors

- `Plane()` = Default origin (0, 0, 0) normal (1, 0, 0)
- `Plane(Vector3 r, Vector3 s)` = (Origin, normal)
- `Plane(const Vector4& s)` = From standard form `(A, B, C, D) <-- (Ax + By + Cz + D = 0)`
- `Plane(float a, float b, float c, float d)`

## Functions

- `Vector4 standardForm()` = Returns a `Vector4(A, B, C, D)` where the plane in standard form is `Ax + By + Cz + D = 0`.
- `Vector3 proj(Vector3 other)` = Projects a `Vector3` to the plane.
- `float distanceFromOrigin()` = Minimum distance to `(0, 0, 0)`
- `float distanceFrom(const Vector3& q)` = Minimum distance to a point
- `float distanceFrom(const Vector4& q)`
- `bool contains(const Vector3& p)` = Contains point?
- `float intersectionTime(Line& L)` = Intersection time of a line (so `L.get(intersectionTime)` gets the intersection point).
- `Vector3 intersection(Line& L)` = Gets the intersection point.
- `Vector3 intersection(Plane& A, Plane& B)` = Gets an intersection of this plane and two others.
- `Line intersection(Plane& A)` = Gets a line intersection of this plane and another.
- `transform_(Transform T)` = Transforms the plane (you can also do `Trans` and `ForceTrans`).
- `void reinit(Vector4 s)` = Reinit the plane from a standard form without using the constructor.
- `std::string to_string()`
