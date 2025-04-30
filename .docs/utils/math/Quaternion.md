# utils/math/Quaternion

`class Quaternion`

## Description

Quaternion class for rotations.

## Notes

- Most examples use Transforms. There exist methods to convert quaternions to matrices and back.

## Fields

- `w: float` = Real component.
- `v: Vector3` = Imaginary component.

## Constructors

- `Quaternion()` = Zero quaternion.
- `Quaternion(float s, Vector3 u)`
- `Quaternion(float s, float x, float y, float z)` = Real `s` and Imaginary `v = [x, y, z]`
- `Quaternion(Vector4 u)` = Real `u.w` imaginary `[u.x, u.y, u.z]`

## Functions

- `Vector4 toVec4()` = Returns `[v.x, v.y, v.z, w]`
- `Quaternion conj()`
- `Quaternion mul(const Quaternion& other)`
- `Quaternion add(const Quaternion& other)`
- `Quaternion sub(const Quaternion& other)`
- `Quaternion mul(const float other)`
- `float dot(const Quaternion& other)`
- `Quaternion div(const float other)`
- `float normsquared()` = Sum of squares of components.
- `float norm()`
- `Quaternion normalized()`
- `Quaternion inv()`
- `Matrix3 toRotation()` = Convert to rotation matrix.
- `Quaternion operator+(const Quaternion& other)`
- `Quaternion operator+(const Quaternion& other)`
- `Quaternion operator*(const Quaternion& other)`
- `Quaternion operator*(const float other)`
- `Quaternion operator/(const float other)`
- `bool operator==(const Quaternion& other)`
- `bool operator!=(const Quaternion& other)`
- `Vector3 on(const Vector3& p)` = Perform a rotation on point `p`.
- `Vector3 operator()(const Vector3& p)` = `on(p)`.
- `std::string to_string()`

# Static Instances

- `Quaternion QuaternionAA(Vector3 A, float t)` = Generate a quaternion for an axis `A` and angle `t`.
- `Quaternion lerp(Quaternion a, Quaternion b, float t)` = Naive linear interpolation and normalization.
- `Quaternion spherp(Quaternion a, Quaternion b, float t)` = Spherical interpolation.
- `Quaternion slerp(Quaternion a, Quaternion b, float t)` = Spherical interpolation.