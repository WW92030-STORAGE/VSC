# objects/Triangle/Triangle2

`class Triangle2 : public Object`

## Description

Two-dimensional Triangle

## Fields

- `p: Vector2[3]` = Vertex positions.

## Constructors

- `Triangle2()`
- `Triangle2(Vector2 a, Vector2 b, Vector2 c)`
- `Triangle2(Vector2 x[3])`

## Functions

- `void orient()` = Reorders the vertices in counterclockwise order.
- `bool inside(Vector2 P)` = If `P` is inside the triangle.

---

# objects/Triangle/Triangle3

`class Triangle3 : public Object`

## Description

Three-dimensional Triangle

## Fields

- `p: Vector3[3]` = Vertex positions.
- `uv: Vector2[3]` = uvs of vertices.
- `N: Vector3` = Normal vector.

## Constructors

- `Triangle3()`
- `Triangle3(Vector3& a, Vector3& b, Vector3& c)`
- `Triangle3(Vector3& a, Vector3& b, Vector3& c, Vector2& q, Vector2& r, Vector2 s)`
- `Triangle3(Vector3 x[3])`
- `Triangle3(Vector3 x[3], Vector2 uu[3])`

## Functions

- `Vector3 normal()` = Computes the normal vector. This is done in the constructors as well.
- `bool inside(Vector3 s)` = Computes if `s` is inside the infinite triangular prism with cross section (this) and axis aligned to the normal.
- `float intersectionTime(Line L)` = Given a parametric line `L`, computes the time at which the line intersects the triangle.
- `Vector3 intersection(Line& L)` = Computes the intersection point of the line.
- `Vector3 centroid()` = Returns the centroid.
- `bool operator<(const Triangle3& other) const` = Compares by centroid z coordinate.
- `Vector3 bary(Vector3 pos)` = Barycentric coordinates of `pos` assuming it is on/near the triangle.
- `float interp(Vector3 pos, float f0, float f1, float f2)` = Barycentric interpolation of vertex attributes `(f0, f1, f2)` to position `pos`.
- `T interp(Vector3 pos, T a, T b, T c)` = Genericized interpolation.

---

# objects/Triangle/TriangleF

`class TriangleF`

## Description

Triangle of FRAGMENTS

## Fields

- `p: Fragment[3]` = Vertex positions.
- `N: Vector3` = Normal vector.
- `ON: Vector3` = Original Normal (as in in world space)
- `material: BaseMaterial*` = Material to use

## Constructors

- `TriangleF()`
- `TriangleF(Fragment& a, Fragment& b, Fragment& c, Vector3 n)`
- `TriangleF(Fragment x[3], Vector3 n)`
- `TriangleF(const TriangleF& other)`

## Functions

- `void orient()` = Orient so the normal has positive z.
- `bool inside(Vector2 P)` = Is `P` expressed as Fragment coordinates inside the triangle?
- `Vector3 bary(int x, int y)` = Barycentric coordinates of `(x, y)` as Fragment coords.
- `float interp(int x, int y, float f0, float f1, float f2)` = Barycentric interpolation of vertex attributes `(f0, f1, f2)` to position `(x, y)`.
- `T interp(int x, int y, T a, T b, T c)` = Genericized interpolation.
- `T interp_given_bary(Vector3 r, T a, T b, T c, bool ns = false)` = Genericized interpolation given already barycentric coordinates.
- `Vector4 centroid()` = Returns the centroid.
- `bool operator<(const Triangle3& other) const` = Compares by centroid z coordinate.

# Static Instances

`Triangle2 NILTRI2(NILVEC2, NILVEC2, NILVEC2)`
`Triangle3 NILTRI3(NILVEC3, NILVEC3, NILVEC3)`
`TriangleF NILTRIF(NILFRAG, NILFRAG, NILFRAG, NILVEC3)`