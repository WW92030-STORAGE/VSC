# utils/math/Transform

`class Transform`

## Description

Transform class, representing a coordinate system.

## Fields

- `origin: Vector3`
- `basis: Matrix3`

## Constructors

- `Transform()` = Origin (0, 0, 0) and identity basis.
- `Transform(Vector3 loc)`
- `Transform(Matrix3 rot)`
- `Transform(Vector3 loc, Matrix3 rot)`
- `Transform(Matrix4 m)` = From a transform matrix.

## Functions

- `Matrix4 matrix()` = Converts to transform matrix.
- `Transform inv()` = Inverse transform.
- `Transform mul(Transform& other)` = Composite as `(this * other)`.
- `Transform NormalTransform()` = Equivalent transform of a normal vector.
- `Transform operator*(Transform& other)`
- `std::string to_string()`
- `std::string sprintf()`

# Static Instances

- `Transform RotationAroundPoint(Vector3 p, Vector3 a, float t)` = Rotate around the point `p` with axis `a` and angle `t`.
- `Transform NormalTransform(Transform M)` = Equivalent transform for normal vector.