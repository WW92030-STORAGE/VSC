# objects/Mesh



## Description

Clip meshes against planes!

## Functions

- `std::pair<std::vector<Triangle3>, std::vector<Triangle3>> TriSplit(Triangle3 t, Plane splane)` = Split triangles across planes, returns two vectors of triangles. The first vector has the triangles in the split on the positive side of the plane, the second on the negative side.

- `std::pair<std::vector<std::vector<T>>, std::vector<std::vector<T>>> TriSplitAttr(Triangle3 t, Plane splane, std::vector<T> attr)` = Split the attributes of triangles across planes. This is most useful with `TriSplit` with the same first two parameters. 