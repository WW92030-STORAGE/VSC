# objects/MorphedMesh

`class MorphedMesh : public Mesh`

## Description

Class that stores a list of meshes with the same topological structure (same number of vertices, same triangle and texcoord indices, etc.) you can interpolate between to do morph animations.

## Notes

- THE MESH MUST ONLY CONSIST OF TRIANGLES. UNDEFINED BEHAVIOR IF LOADING A NON-TRIMESH (E.G. A QUADMESH).
- Programs like Blender and online programs can triangulate meshes for you.
- Triangles, vertices, etc. are frequently referred to by their index.

## Fields

- `nstates: int` = Number of states in the MorphedMesh.
- `mverts: std::vector<std::vector<Vector3>>` = Lists of vertices for each state. States are indexed.
- `std::vector<std::vector<Vector3>>` = Lists of vertex normals for each state.

Remember that uvs, texture coordinates, and triangle indices are to be preserved across states.

## Constructors

- `MorphedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t)`
- `MorphedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco)`

Each of these constructors makes a MorphedMesh with a single state. The latter also initializes the texture mappings.

You can copy constructor from both a Mesh and a MorphedMesh. The former will create a one-state MorphedMesh.

## Functions

- `m_setupvns()` = Setup up vertex normals for ALL states.
- `void copyTo(int state = -1)` = Take the current deformation of the mesh, copy it to a new or existing state (-1 = new).
- `void copyTo(std::vector<Vector3> ve, std::vector<Vector3> nv, int state = -1)` = Copy a (vertex list, normals list) to a state.
- `copyTo(Mesh& other, int state = -1)` = Copy a mesh to a state.
- `void morph(std::vector<float> coeff, bool interpnorms = true)` = Morphs the mesh to a linear combination of its states, given by coeff. If `interpnorms` is set then the normals become a normalized weighted average of the state normals, otherwise `setupvns()` is called.
- `void morph(float* coeff, int n, bool interpnorms = true)` = Same as `morph(...)` but with a native array.
- `void morphToState(int state, bool interpnorms = true)` = Morphs to a given state.
- `Triangle3 makeTriangle(int i)` = Returns a Triangle3 corresponding to index `i` of the current deformation.
- `Triangle3 m_makeTriangle(int index, int i)` = Returns a Triangle3 corresponding to the `i`th triangle of state `index`.

## Deprecated/Unused Functions

- `Vector3 vindex(std::vector<Vector3>& v, int i)` = Gets the `i`th vertex.
- `std::vector<Triangle3> TriangulateConvex(std::vector<int> vertices, std::vector<Vector3> verts)` = Triangulates a polygon given as indices `vertices` in the vertex array `verts`.