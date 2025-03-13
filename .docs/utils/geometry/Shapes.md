# utils/geometry/Shapes

## Description

Functions to generate basic shape meshes. All meshes start centered at the origin, you must `Trans(form)` them.

## Functions

- `Mesh cube(float X = 16)` = Generates a cube of side length `X` centered at the origin. Each face has the same texture mapping.
- `Mesh cube6(float X = 16)` = Generates a cube of side length `X`. Each face has a different texture given by the combined uv mapping in `/img/rgbcube2.png`.
- `Mesh GridSquare(float X = 16, int N = 1)` = Generates a large square lamina of side length `X` and subdivided into a `NxN` grid of small squares. Each square has two triangles. Each square has the same texture mapping.
- `Mesh icosphere(float R = 1, int S = 1)` = Generates an icosphere with radius `R` and `S` levels of subdivision (0 levels is an icosahedron). No texture mapping is provided.

## Internal Functions

- `int VFE_sphere(std::vector<Vector3>& vertices, std::map<std::pair<int, int>, int>& lookup, int f, int s)`
- `std::vector<std::vector<int>> subdivide_sphere(std::vector<Vector3>& verts, std::vector<std::vector<int>>& tris)`
