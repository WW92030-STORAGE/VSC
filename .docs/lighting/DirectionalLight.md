# lighting/DirectionalLight

`class DirectionalLight : public Light`

## Description

Directional light (NOT YET FULLY IMPLEMENTED)

## Fields

- `dir: Vector3` = Normalized light direction

## Constructors

- `DirectionalLight()`
- `DirectionalLight(Vector3 v)`
- `DirectionalLight(Vector3 v, Vector3 i)` = Initializes the DirectionalLight. If provided, `dir` is set to `v` and `intensity` is set to `i`.
