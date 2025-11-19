# objects/Mesh

`class Mesh : public Object`

## Description

Triangle mesh class. Stores vertices and triangles as indices of vertices, uvs, etc.

## Notes

- THE MESH MUST ONLY CONSIST OF TRIANGLES. UNDEFINED BEHAVIOR IF LOADING A NON-TRIMESH (E.G. A QUADMESH).
- Programs like Blender and online programs can triangulate meshes for you.
- Triangles, vertices, etc. are frequently referred to by their index.

## Fields

- `size, nverts, nuv: int` = number of triangles, number of vertices, number of uv points.
- `verts: std::vector<Vector3>` = List of vertices
- `triindices: std::vector<std::vector<int>>` = List of triangles as indices in `verts`
- `texcoords: std::vector<std::vector<int>>` = List of texture coordinates for triangles as indices in `uv`
- `vn: std::vector<Vector3>` = List of vertex normals (normalized vectors)
- `uv: std::vector<Vector3>` = List of vertex uvs

## Constructors

- `Mesh()` = Default mesh is an untexturable cube.
- `Mesh(int sz)` = sets the `size`
- `Mesh(Vector3* v, int** t, int nv, int sz)`
- `Mesh(Vector3* v, int** t, Vector2* tex, int** texco, int nv, int sz, int nuuv)`
- `Mesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t)`
- `Mesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco)`
- `Mesh(Triangle3* tt, int sz)` = No material support.
- `Mesh(std::vector<Triangle3> tt)` = No material support.

`v` is the list of vertices in the mesh
`t` is the list of triangle, represented as indices regarding `v`
`tex` is the list of `uv` coordinates that the mesh uses for vertices
`texco` is the list of triangle corner uvs, in terms of indices into `tex`

## Functions

- `setupvns()` = Given the mesh, re-calculates the vertex normals as averages of the normals of adjoining faces. 
- `Vector3 getVertexNormal(int i)` = Gets normal of vertex `i`
- `Vector3 getVertexNormal(int ind, int i)` = Gets normal of vertex `i` in triangle `ind`
- `Vector3 getInterpolatedNormal(Vector3 pos, int ind)` = Gets a barycentrically interpolated normal at position `pos`, assuming it is a point on or near the triangle `ind`.
- `Vector2 getVertexUV(int i, int s)` = Gets the uv coordinates of vertex `s` in triangle `i`
- `Vector2 getUV(Vector3 pos, int ind)` = Gets the uv coordinates of position `pos` with respect to triangle `ind`.
- `Triangle3 makeTriangle(int i)` = Returns a Triangle3 corresponding to index `i`.
- `void flipNormals()` = Flip the normal vectors by swapping two vertices in each triangle.
- `static Mesh fromOBJ(std::string filename)` = Returns a mesh created from an `.obj` file.

## Deprecated/Unused Functions

- `static int comp (const void * a, const void * b)` = compare 2 Triangle3 pointers by the z coordinates of their centroids