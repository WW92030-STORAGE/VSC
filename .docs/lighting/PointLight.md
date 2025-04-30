# lighting/PointLight

`class PointLight : public Light`

## Description

Point light with attenuation.

## Constructors

- `PointLight()`
- `PointLight(Vector3 i)`
- `PointLight(float a)`
- `PointLight(Vector3 i, float a)` = Initializes the PointLight. If provided, `intensity` is set to `i` and `attenuation` to `a`. The default position is (0, 0, 0).
