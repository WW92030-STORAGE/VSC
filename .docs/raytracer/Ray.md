# raytracer/Ray

`class Ray`

## Description

Ray with an origin and a direction.

## Fields

- `point, slope: Vector3` = (Ray origin/reference point), (Normalized ray direction) i.e. `r(t) = point + slope * t`

## Constructors

- `Ray()` = Default `point = (0, 0, 0), slope = (1, 0, 0)`
- `Ray(Vector3 p, Vector3 s)`

## Functions

- `Vector3 get(float t)` = Gets `point + slope * t`
- `Line toLine()` = Returns an infinite line corresponding to the ray.
- `float intersectTriangle(Triangle3 t)` = Time when the ray intersects the triangle.
- `std::string to_string()` = String representation.

---

# raytracer/Ray/IntersectionPoint

`class IntersectionPoint`

## Description

Data about intersection points.

## Fields

- `material: BaseMaterial*` = Material of the point
- `time: float` = Time of the intersection
- `N: Vector3` = Normal of the point
- `uv: Vector2` = uv of the point

## Constructors

- `IntersectionPoint(BaseMaterial* bm, float t, Vector3 n, Vector2 u)`

## Functions

- `bool valid()` = Is the point valid? (Time is not `INF` and `N` is not NIL).