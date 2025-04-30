# objects/QuadMesh



## Description

Subdivision of meshes. The parameter `k` refers to how many times we do the subdivision. Each iteration takes all the faces and splits them into four smaller faces.

## Functions

- `template <typename T> int subdiv::getIndexOrCreate(std::vector<T>& v, std::map<T, int>& inv, T i)` = Given an array of unique values `v` and a map `inv` mapping those unique values to the array indices, either pushes a new value `i` to the array `v` and returns its new index, or retrieves the index of `i` if it already exists in `v`.
- `Mesh subdivide(Mesh m, int k = 1)` = Linear subdivision of a trimesh.
- `Mesh subdivideLoop(Mesh m, int k = 1)` = Spline-based subdivision of trimeshes (Loop subdivision).
- `QuadMesh subdivideCC(QuadMesh m, int k = 1)` = Spline-based subdivision of quadmeshes (Catmull-Clark subdivision)