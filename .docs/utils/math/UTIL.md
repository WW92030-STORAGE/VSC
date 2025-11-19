# utils/math/UTIL



## Description

Math utility functions that use custom data types in VSC.

## Functions

- `Vector2 quadratic(float a, float b, float c)` = Solves `a * x * x + b * x + c = 0` and arranges roots in order.
- `bool isZero(Vector2 v)` = Components `fzero`.
- `bool isZero(Vector3 v)`
- `bool isZero(Vector4 v)`
- `Vector3 NormSum(Vector3 v)` = Divides by the element sum, so the sum of the returned vector elements is one.
- `Vector3 min(Vector3 a, Vector3 b)` = Elementwise.
- `Vector3 max(Vector3 a, Vector3 b)`