# raytracer/RayTracer

`class RayTracer : public Scene`

## Description

Scene that supports ray tracing

## Fields

- `meshes: std::vector<Mesh*>` = List of meshes
- `materials: std::vector<BaseMaterial*>` = List of materials for each mesh
- `norminterps: std::vector<bool>` = Whether we interpolate normals for each mesh
- `DEPTH: int` = Maximum recursion depth (0 for no recursion)

- `RAY_EPSILON: const float = 0.01` = Margin of error for some calculations.
- `bvh: BVH` = Bounding Volume Hierarchy used for acceleration.
- `UseBVH: bool` = Do we use the BVH? (WARNING: THIS IS TURNED OFF BY DEFAULT. YOU MUST SET IT TO BE TURNED ON.)

## Constructors

- `RayTracer(float depth = 0) : Scene()`
- `RayTracer(float depth = 0, int w = 256, int h = 256) : Scene(w, h)`

## Functions

- `void addMesh(Mesh* mesh, BaseMaterial* mat = nullptr, bool INTERPNORM = false)` = Adds a mesh, material, and interpnorm status to the scene.
- `Vector3 shade(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true)` = Shade a point `position` given the incoming ray `pRay` and other data and return the resultant color.
- `IntersectionPoint intersectRay(Ray r)` = Finds the first intersection point (lowest time) of the ray with objects (triangles) in the scene. If `UseBVH` is enabled then this will use the BVH.
- `IntersectionPoint intersectRayNaive(Ray r)` = Finds the first intersection point (lowest time) of the ray with objects (triangles) in the scene (Does not use the BVH).
- `Ray castRay(Vector2 NDC)` = Cast a ray from the camera origin position in the corresponding `NDC` direction in Normalized Device Coordinates (The ray is expressed in world coordinates).
- `Vector3 traceRay(Ray r, bool LIT = false, int depth = 0)` = Recursive ray tracing, return the resultant color.
- `uint32_t tracePixel(Vector2 NDC, bool LIT = false, int depth = 0)` = Trace a ray corresponding to Normalized Device Coordinates `NDC` and return the color
- `tracePixel(int x, int y, bool LIT = true, int depth = 0)` = Trace a pixel given its `(x, y)` coordinates, where `(0, 0)` is bottom left.
- `void render(bool LIT = true, int depth = 0)` = Ray trace all pixels in the buffer.
- `void morph(int index, std::vector<float> states)` = Morph a MorphedMesh at index `index` in the meshes array, using `states` as the coefficients.
- `void morphToState(int index, int state)` = Morph a MorphedMesh at index `index` to state `state`.
- `uint64_t countTriangles()` = Returns the number of triangles stored in the meshes.