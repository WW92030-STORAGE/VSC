# utils/math/Vectors/Vector2

`class Vector2`

## Description

Two-dimensional vector.

## Notes

Elements are zero-indexed.

## Fields

- `x, y: float`

## Constructors

- `Vector2()` = (0, 0)
- `Vector2(float r, float s)`

## Functions

- `float get(int a)` = Get element at index `a`.
- `void set(int a, float f)` = Set element at index `a` to value `f`.
- `Vector2 add(const Vector2& other)`
- `Vector2 inv()`
- `Vector2 sub(const Vector2& other)`
- `Vector2 mul(float other)`
- `Vector2 div(float other)`
- `float dot(const Vector2& other)`
- `float normsquared()`
- `float length()`
- `Vector2 normalized()` = Unit vector.
- `float cosine(Vector2& other)` = Cosine of the angle.
- `float angle(Vector2& other)` = Angle measure.
- `Vector2 proj(Vector2& other)` = Projects this onto the other.
- `Vector2 ortho(Vector2& other)` = Orthogonal component relative to the other.
- `Vector2 rot(float theta)` = Rotates the vector.
- `Vector2 operator+(const Vector2& other)`
- `Vector2 operator-(const Vector2& other)`
- `float operator*(const Vector2& other)`
- `Vector2 operator*(const float other)`
- `Vector2 operator/(const float other)`
- `bool operator==(const Vector2& other)`
- `bool operator!=(const Vector2& other)`
- `std::string to_string()`

---

# utils/math/Vectors/Vector2i

A similar class to `Vector2` except with integer fields and only constructors. This is mainly used for triangle rasterization.

---

# utils/math/Vectors/Vector3

`class Vector3`

## Description

Three-dimensional vector.

## Notes

Elements are zero-indexed.

## Fields

- `x, y, z: float`

## Constructors

- `Vector3()` = (0, 0)
- `Vector3(float r, float s, float t)`

## Functions

- `float get(int a)` = Get element at index `a`.
- `void set(int a, float f)` = Set element at index `a` to value `f`.
- `Vector3 add(const Vector3& other)`
- `Vector3 inv()`
- `Vector3 sub(const Vector3& other)`
- `Vector3 mul(float other)`
- `Vector3 div(float other)`
- `float dot(const Vector3& other)`
- `float normsquared()`
- `float length()`
- `Vector3 normalized()` = Unit vector.
- `float cosine(Vector3& other)` = Cosine of the angle.
- `float angle(Vector3& other)` = Angle measure.
- `Vector3 proj(Vector3& other)` = Projects this onto the other.
- `Vector3 ortho(Vector3& other)` = Orthogonal component relative to the other.
- `Vector3 cross(const Vector3& other)` = Cross product.
- `Vector3 normal(const Vector3& other)` = Normalized normal vector.
- `Vector3 operator+(const Vector3& other)`
- `Vector3 operator-(const Vector3& other)`
- `float operator*(const Vector3& other)`
- `Vector3 operator*(const float other)`
- `Vector3 operator/(const float other)`
- `bool operator==(const Vector3& other)`
- `bool operator!=(const Vector3& other)`
- `bool operator<(const Vector3& other) const` = Lexicographic.
- `std::string to_string()`

---

# utils/math/Vectors/Vector4

`class Vector4`

## Description

Four-dimensional vector.

## Notes

Elements are zero-indexed.

## Fields

- `x, y, z, w: float`

## Constructors

- `Vector4()` = (0, 0)
- `Vector4(float r, float s, float t, float u)`

## Functions

- `float get(int a)` = Get element at index `a`.
- `void set(int a, float f)` = Set element at index `a` to value `f`.
- `Vector4 add(const Vector3& other)`
- `Vector4 inv()`
- `Vector4 sub(const Vector3& other)`
- `Vector4 mul(float other)`
- `Vector4 div(float other)`
- `float dot(const Vector4& other)`
- `float normsquared()`
- `float length()`
- `Vector4 normalized()` = Unit vector.
- `float cosine(Vector4& other)` = Cosine of the angle.
- `float angle(Vector4& other)` = Angle measure.
- `Vector4 proj(Vector4& other)` = Projects this onto the other.
- `Vector4 ortho(Vector4& other)` = Orthogonal component relative to the other.
- `Vector4 operator+(const Vector4& other)`
- `Vector4 operator-(const Vector4& other)`
- `float operator*(const Vector4& other)`
- `Vector4 operator*(const float other)`
- `Vector4 operator/(const float other)`
- `bool operator==(const Vector4& other)`
- `bool operator!=(const Vector4& other)`
- `std::string to_string()`

---

# Static Instances

- `Vector2 NILVEC2 = Vector2(NAN, NAN)`
- `Vector3 NILVEC3 = Vector3(NAN, NAN, NAN)`
- `Vector4 NILVEC4 = Vector4(NAN, NAN, NAN, NAN)`
- `Vector2 lerp(Vector2& a, Vector2& b, float p)` = Linear Interpolation `a * (1 - p) + b * p`.
- `Vector3 lerp(Vector3& a, Vector3& b, float p)`
- `Vector4 lerp(Vector4& a, Vector4& b, float p)`
- `Vector4 fromPoint(Vector3& u)` = Pads a `w` coordinate of one.
- `{Vector2 vec2, Vector3 vec3, Vector4 vec4}({Vector2, Vector3, Vector4} u)` = Converts between vectors by truncating or padding with zeros.