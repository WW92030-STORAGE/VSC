# .utils/curves/Bezier

`class Bezier : BaseCurve`

## Description

Bezier spline.

## Fields

- `controls: std::vector<Vector3>`
- `size: int`

## Constructors

- `Bezier() : BaseCurve()` = 4 control points: the origin and three unit basis vectors.
- `Bezier(std::vector<Vector3> v) : BaseCurve(v)`

## Functions

- `Vector3 query(float t)` = Grabs the spline point at time `t : [0, 1]`