# utils/math/Matrices/Matrix2

`class Matrix2`

## Description

Two-dimensional matrix.

## Notes

Indices of rows and columns are zero-indexed.

## Fields

- `xAxis, yAxis: Vector2` = Left and right columns.

## Constructors

- `Matrix2()`
- `Matrix2(Vector2 r, Vector2 s)`
- `Matrix2(float r, float s, float t, float u)` = Row major order.

## Functions

- `static Matrix2 eye()` = Identity matrix.
- `float get(int a, int b)` = Gets element at row `a` and column `b`.
- `Vector2 getRow(int a)`
- `Matrix2 add(const Matrix2& other)`
- `Matrix2 sub(const Matrix2& other)`
- `Matrix2 mul(float other)`
- `Matrix2 div(float other)`
- `float det()` = Determinant.
- `Matrix2 trans()` = Transpose.
- `Matrix2 inv()` = Inverse.
- `Vector2 vecmul(const Vector2& other)`
- `Matrix2 matmul(const Matrix2& other)`
- `Vector2 solve(Vector2 b)` = Solves the system `Ax = b` where `A` is this matrix. One solution is given.
- `Vector2 eigenvalues()`
- `Matrix2 eigenvectors()` = Each vector is a column.
- `Matrix2 operator+(const Matrix2& other)`
- `Matrix2 operator-(const Matrix2& other)`
- `Matrix2 operator*(float other)`
- `Vector2 operator*(const Vector2& other)`
- `Matrix2 operator*(const Matrix2& other)`
- `Matrix2 operator/(float other)`
- `bool operator==(const Matrix2 other)`
- `bool operator!=(const Matrix2 other)`
- `std::string to_string()`

---

# utils/math/Matrices/Matrix3

`class Matrix3`

## Description

Three-dimensional matrix.

## Notes

Indices of rows and columns are zero-indexed.

## Fields

- `xAxis, yAxis, zAxis: Vector3` = Columns.

## Constructors

- `Matrix3()`
- `Matrix3(Vector3 r, Vector3 s, Vector3 t)`
- `Matrix3(float a ... float i)` = Row major order.

## Functions

- `static Matrix3 eye()` = Identity matrix.
- `static Matrix3 eye_scaled(float F)` = `eye() * F`
- `static Matrix3 diagonal(float a, float b, float c)` = Diagonal with entries `(a, b, c)`
- `float get(int a, int b)` = Gets element at row `a` and column `b`.
- `Vector3 getRow(int a)`
- `void set(int a, int b, float v)` = Sets element `[a, b]` to `v`.
- `void setRow(int a, Vector3 v)` = Sets row `a` to `v`.
- `Matrix3 add(const Matrix3& other)`
- `Matrix3 sub(const Matrix3& other)`
- `Matrix3 mul(float other)`
- `Matrix3 div(float other)`
- `float det()` = Determinant.
- `Matrix3 trans()` = Transpose.
- `Matrix3 inv()` = Inverse.
- `Vector3 vecmul(const Vector3& other)`
- `Matrix3 matmul(const Matrix3& other)`
- `Vector3 solve(Vector3 b)` = Solves the system `Ax = b` where `A` is this matrix. One solution is given.
- `Matrix3 operator+(const Matrix3& other)`
- `Matrix3 operator-(const Matrix3& other)`
- `Matrix3 operator*(float other)`
- `Vector3 operator*(const Vector3& other)`
- `Matrix3 operator*(const Matrix3& other)`
- `Matrix3 operator/(float other)`
- `bool operator==(const Matrix3 other)`
- `bool operator!=(const Matrix3 other)`
- `std::string to_string()`
- `std::string sprintf()`

---

# utils/math/Matrices/Matrix4

`class Matrix4`

## Description

Four-dimensional matrix.

## Notes

Indices of rows and columns are zero-indexed.

## Fields

- `xAxis, yAxis, zAxis, wAxis: Vector4` = Columns.

## Constructors

- `Matrix4()`
- `Matrix4(Vector4 r, Vector4 s, Vector4 t, Vector4 u)`
- `Matrix4(float a ... float p)` = Row major order.

## Functions

- `static Matrix4 eye()` = Identity matrix.
- `float get(int a, int b)` = Gets element at row `a` and column `b`.
- `Vector4 getRow(int a)`
- `void set(int a, int b, float v)` = Sets element `[a, b]` to `v`.
- `void setRow(int a, Vector4 v)` = Sets row `a` to `v`.
- `Matrix4 add(const Matrix3& other)`
- `Matrix4 sub(const Matrix3& other)`
- `Matrix4 mul(float other)`
- `Matrix4 div(float other)`
- `float det()` = Determinant.
- `Matrix4 trans()` = Transpose.
- `Matrix4 inv()` = Inverse.
- `Vector4 vecmul(const Vector4& other)`
- `Matrix4 matmul(const Matrix4& other)`
- `Vector4 solve(Vector4 b)` = Solves the system `Ax = b` where `A` is this matrix. One solution is given.
- `Matrix4 operator+(const Matrix4& other)`
- `Matrix4 operator-(const Matrix4& other)`
- `Matrix4 operator*(float other)`
- `Vector4 operator*(const Vector4& other)`
- `Matrix4 operator*(const Matrix4& other)`
- `Matrix4 operator/(float other)`
- `bool operator==(const Matrix2 other)`
- `bool operator!=(const Matrix2 other)`
- `std::string to_string()`
- `std::string sprintf()`

---

# Static Instances

- `Matrix2 NILMAT2 = Matrix2(NAN, NAN, NAN, NAN);`
- `Matrix3 NILMAT3 = Matrix3(NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN);`
- `Matrix2 Rotation2(float angle)` = Two-dimensional rotation.
- `Matrix3 Rotation3(Vector3 a, float angle)` = Three-dimensional rotation around an axis `a`.