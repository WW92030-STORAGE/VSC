# .utils/curves/BaseCurve

`class BaseCurve`

## Description

Base curves (splines) class (representing 1-dimensional spline ``manifolds'').

## Fields

- `controls: std::vector<Vector3>`
- `size: int`

## Constructors

- `BaseCurve()` = 4 control points: the origin and three unit basis vectors.
- `BaseCurve(std::vector<Vector3> v)`

## Functions

- `virtual Vector3 query(float t)` = Grabs the spline point at time `t : [0, 1]`