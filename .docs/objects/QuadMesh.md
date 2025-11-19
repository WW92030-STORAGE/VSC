# objects/QuadMesh

`class QuadMesh : public Mesh`

## Description

QUADRILATERAL mesh class. Stores vertices and quadrilaterals as indices of vertices, uvs, etc.

## Notes

- THE MESH MUST ONLY CONSIST OF QUADRILATERALS. UNDEFINED BEHAVIOR IF LOADING A NON-QUADMESH (E.G. A TRIMESH).
- Quads, vertices, etc. are frequently referred to by their index.
- THE MESH MUST BE TRIANGULATED FIRST (using a convert method) TO BE ABLE TO RENDER. QuadMesh also does not store normals: those will be generated upon triangulation.

## Fields

- `size, nverts, nuv: int` = number of triangles, number of vertices, number of uv points.
- `verts: std::vector<Vector3>` = List of vertices
- `quadindices: std::vector<std::vector<int>>` = List of quads as indices in `verts`
- `texcoords: std::vector<std::vector<int>>` = List of texture coordinates for quads as indices in `uv`
- `uv: std::vector<Vector3>` = List of vertex uvs

## Constructors

- `QuadMesh()` = Default mesh is an untexturable cube.
- `QuadMesh(int sz)` = sets the `size`
- `QuadMesh(Vector3* v, int** t, int nv, int sz)`
- `QuadMesh(Vector3* v, int** t, Vector2* tex, int** texco, int nv, int sz, int nuuv)`
- `QuadMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t)`
- `QuadMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco)`

## Functions

- `Vector2 getVertexUV(int i, int s)` = Gets the uv coordinates of vertex `s` in quad `i`
- `Vector2 getUV(Vector3 pos, int ind)` = Gets the uv coordinates of position `pos` with respect to quad `ind`.
- `void flipNormals()` = Flip the normal vectors by swapping the order of vertices in each quad.
- `Vector3 centroid(int i)` = Get the centroid (average of the four vertices) of a face.
- `Vector3 get(int face, int id)` = Get the vertex at the `id` index of `face` quad.
- `Mesh convert()` = Convert the quadmesh into a trimesh by splitting each quad into two triangles.
- `Mesh convertfiner()` = Convert the quadmesh by splitting each quad into FOUR triangles using the centroid.

- `static Mesh fromOBJ(std::string filename)` = Returns a QuadMesh created from an `.obj` file.


## Deprecated/Unused Functions