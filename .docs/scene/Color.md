# scene/Color

## Description

Methods for handling colors represented as `[Vector3, Vector4, uint32_t]` RGB data.

## Notes

- All methods are static.

## Functions

- `uint32_t rgba(Vector4 v)`
- `Vector4 rgba(uint32_t s)`
- `Vector3 rgb(uint32_t s)`
- `uint32_t rgb(Vector3 s)`
- `uint32_t rgb(int r, int g, int b)`
- `uint32_t colmul(uint32_t s, float f)` = Multiply color by scalar
- `Vector4 FullHue(float prop)` = Generates a full saturation, full value color on the color wheel: 0 is red, 1/3 is green, 2/3 is blue.
- `Vector3 clampColor(Vector3 s)` = Clamp the components to within `[0, 1]`