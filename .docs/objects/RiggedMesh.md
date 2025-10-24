# objects/RiggedMesh

`class RiggedMesh : public Mesh`

## Description

Mesh that is controlled by an armature.

## Notes

- THE MESH MUST ONLY CONSIST OF TRIANGLES. UNDEFINED BEHAVIOR IF LOADING A NON-TRIMESH (E.G. A QUADMESH).
- Programs like Blender and online programs can triangulate meshes for you.
- Triangles, vertices, etc. are frequently referred to by their index.
- It is highly recommended to only use this class on outputs of the collada (et al.) parser.\
- No two bones can share a name.
- A bone consists of a joint and a tip. The joint represents the pivot point of the bone.

## Fields

- `nbones: int` = Number of bones.
- `bone_names: std::vector<std::string>` = Names of bones.
- `bone_names_inv: std::unordered_map<std::string, int>` = Mappings of bones to indices.
- `parent: std::vector<int>` = Maps bone indices to their parent indices. Negative entries denote root bones.
- `std::vector<std::vector<int>>` = Denotes, for each bone index, the indices of their children bones.
- `rest_transforms: std::vector<Transform>` = The relative transform, mapping each bone's coordinate space to its parent's coordinate space. The parent of a root bone is the world space.
- `tips: std::vector<Vector3>` = Tips of bones, in their local space.
- `std::vector<Transform> deformations` = Deformations applied to each bone. Initially all deformations are identity, and deformations propagate down the bone tree.
- `absolute_transforms: std::vector<Transform>` = Absolute transforms (map bone space to world space). Anything absolute must be computed after each deformation.
- `absolute_joints: std::vector<Vector3>` = Absolute positions of bone joints.
- `absolute_tips: std::vector<Vector3>` = Absolute tip positions of bones (in world space)
- `a_tips: std::unordered_set<int>` = Bones that have explicitly listed tips in the Collada file. (All other bones assume to tip at the location of one of their children.)
- `vertex_weights: std::vector<std::unordered_map<int, float>>` = Weights of each vertex (primary index) for bone indices (key of map) that influence said vertex.
- `vertex_displacements: std::vector<std::unordered_map<int, Vector3>>` = Rest displacement of vertices from bone joints, in local bone space.
- `original_verts: std::vector<Vector3>` = Original absolute positions of vertices (when no deformation is applied)

## Constructors

- `RiggedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t)`
- `RiggedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco)`
- `RiggedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco, std::vector<std::string>& names, std::vector<int>& par, std::vector<Matrix4>& transforms, std::vector<Vector3>& tipp, std::unordered_set<int>& atips, std::vector<std::unordered_map<int, float>> weights)`

`A : B` denotes `A` is constructed using `B`:
- `bone_names : names`
- `parent : par`
- `rest_transforms : transforms`
- `tips : tipp`
- `a_tips : atips`
- `vertex_weights : weights`

## Functions

- `void init()` = Initialize a few important fields such as `vertex_displacements`. Also does initial computation of absolute transforms and re-offsets bone tips.
- `bool is_root(int i)` = Is bone index `i` a root? (Returns if `parent[i] < 0`)
- `void computeAbsoluteTransforms()` = Computes the `absolute_transforms` based on the current `deformations`
- `void computeVertexLocations()` = Computes the absolute positions of vertices assuming the `absolute_transforms` are up to date.
- `std::vector<Mesh> visBones(float R = 0.2)` = Returns meshes that represent bone locations and orientations. You can render these to visualize bones.