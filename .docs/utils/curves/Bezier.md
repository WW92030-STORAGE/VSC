# .utils/curves/Bezier

`class BezierCurve : BaseCurve`

## Description

Bezier spline.

## Fields

- `controls: std::vector<Vector3>`
- `size: int`

## Constructors

- `BezierCurve() : BaseCurve()` = 4 control points: the origin and three unit basis vectors.
- `BezierCurve(std::vector<Vector3> v) : BaseCurve(v)`

## Functions

- `Vector3 query(float t)` = Grabs the spline point at time `t : [0, 1]`