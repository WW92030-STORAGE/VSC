# raytracer/aabb/AABB

`class AABB`

## Description

Axis-Aligned Bounding Box

## Fields

- `inferior, superior: Vector3` = (Minimum x, y, z corner), (Maximium x, y, z corner)

## Constructors

- `AABB(Vector3 a, Vector3 b)`

## Functions

- `AABB merge(AABB other)` = Create a combined AABB smallest one containing (this) and `other`
- `bool operator<(AABB& other)` = Sort first by inferior lexicographic, then by superior.
- `bool operator==(AABB& other)` = Identical inferior and superior.
- `std::string to_string()` = String representation.