# scene/Scene

`class Scene`

## Description

Scene class for rasterization

## Notes

- Might be a bit jank. Base Scene does not store objects.
- Reduced Fragment = stores minimal data about pixels: color, depth.
- Functions called `draw*(...)` will not fill the insides. Functions that fill are called `fill*(...)`. 

## Fields

- `W, H: int` = Dimensions
- `SIDE: int` = `max(W, H)`
- `TRIANGLE_COUNT: uint64_t` = Counter of drawn triangles (for logistical purposes)
- `triqueue: std::queue<std::pair<Triangle3, std::pair<BaseMaterial*, bool>>>` = Queue of triangles to be drawn
- `camera: Camera` = The camera that looks upon the scene
- `buffer: ReducedFrag**` = Framebuffer of the outputted colors
- `ambientLight: Vector3` = Ambient light influence
- `lights: std::vector<Light>` = Lights in the scene

- `meshes: std::vector<Mesh*>` = List of meshes
- `materials: std::vector<BaseMaterial*>` = List of materials for each mesh
- `norminterps: std::vector<bool>` = Whether we interpolate normals for each mesh
- `names: std::vector<std::string>` = Assigned names of the meshes in `meshes`
- `names_inv: std::unordered_map<std::string, uint64_t>` = Maps the names to indices (This means that you must have unique names for meshes)

## Constructors

- `Scene()` = Dimensions (256, 256)
- `Scene(int w, int h)`

## Functions

- `void addMesh(Mesh* mesh, BaseMaterial* mat = nullptr, bool INTERPNORM = false)` = Adds a mesh, material, and interpnorm status to the scene.
- `int getMesh(std::string name)` = Get the index of a mesh from its name (or -1 if null)
- `virtual void render(bool LIT = true, int depth = 0)` = Render a scene using the meshes in `meshes`. You can also draw meshes independent of `meshes` et al. however this will render all meshes in `meshes`
- `void morph(int index, std::vector<float> states)` = Morph a mesh at the `index` (nothing happens if the mesh cannot be morphed)
- `void morphToState(int index, int state)` = Morph a mesh at the `index`
- `void initBuffer()` = Initialize buffer (called in constructors)
- `void clearBuffer()` = Set buffer to black
- `void fillScreen(int32_t c)` = Fill buffer with color
- `void drawPixel(int x, int y, uint32_t z)` = Set a single pixel
- `void drawFragment(Fragment& F, int x, int y)` = Set the buffer entry `buffer[x][y]` to `ReducedFrag(F.ndc.w, F.color)`
- `uint32_t getColor(BaseMaterial* material, Vector2 uv)` = Get the color at uv `uv` in material `material`.
- `Vector3 shade(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true)` = Shade the point `position` given incoming ray `pRay` and other data, return the color.
- `uint32_t shadeInt(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true)` = Return the shaded color as `uint32_t`.
- `Vector3 illuminate(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true)` = Wrapper for the `shade` function, basically gets the illuminated color.
- `uint32_t illuminateInt(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true)` = Illuminate except `uint32_t`.
- `void drawLine(int x0, int y0, int x1, int y1, uint32_t c)`
- `void drawLine(Vector2& a, Vector2& b, uint32_t c)`
- `drawTriangle(Triangle2 s, uint32_t c)`
- `void fillTriangle(Triangle2& s, uint32_t c)`
- `bool BackFaceCull(Triangle3 s)` = If `s` faces away from the camera
- `TriangleF project(Triangle3 s, uint32_t c = 0xFFFFFFFF)` = For a `Triangle3`, return the corresponding `TriangleF` that represents projecting the `Triangle3` to the camera view.
- `void drawTriangle(TriangleF s, uint32_t c)`
- `DrawTriFrag(TriangleF s, Triangle3 t, int x, int y, bool PHONGSHADE = false)` = Draws a fragment at buffer position `(x, y)` using Triangles `s, t` as reference. `s` should be the projection of `t`.
- `void fillTriangle(TriangleF s, Triangle3 T, bool PHONGSHADE = false, bool SCAN = false)`
- `bool clip(TriangleF p)` = Clip a TriangleF against the sides of the NDC cube (Might revamp?)
- `void drawTriangle(Triangle3 s, BaseMaterial* material = nullptr, Vector3* vn = nullptr, Vector2* uv = nullptr, bool BACKFACECULL = false, bool PHONGSHADE = false, bool INTERPNORM = false)`
- `void fillTriangle(Triangle3 s, BaseMaterial* material = nullptr, Vector3* vn = nullptr, Vector2* uv = nullptr, bool BACKFACECULL = true, bool PHONGSHADE = false, bool INTERPNORM = false)`
- `void drawMesh(Mesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = false)`
- `void drawMesh(MorphedMesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = false)`
- `void fillMesh(Mesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = true)`
- `void fillMesh(MorphedMesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = true)`
- `void QueueTriangle(Triangle3 s, BaseMaterial* material, bool FILL = false, bool BACKFACECULL = true)` = Queue a triangle to be drawn or filled (This will result in flat shading)
- `void QueueMesh(Mesh& m, BaseMaterial* material, bool FILL = true)` = Queue a mesh to be drawn or filled (This will result in flat shading)
- `void drawQueue()` = Draw everything in the queue.
- `float getResolution(Vector4 s)` = Get a grayscale approximation of the color.
- `float getResolution(uint32_t c)` = Get a grayscale approximation of the color.
- `std::string to_string()` = String Representation
- `std::string buffer_data()` = Output buffer contents as string (NOT RECOMMENDED).
- `void outputFrags(std::string OUTPUT__)` = Output the buffer contents to a FILE (NOT RECOMMENDED).
- `void outputBuffer(std::string OUTPUT__)` = Output the buffer, a matrix of colors, and the dimensions, to an easily-parseable FILE.
- `std::vector<std::vector<uint32_t>> bufferMatrix()` = Return a matrix of colors corresponding to the buffer.

## Internal Functions

These shouldn't be used by the user.

- `void LineLow(int x0, int y0, int x1, int y1, uint32_t c)`
- `void LineHigh(int x0, int y0, int x1, int y1, uint32_t c)`
- `void fillBotFlat(TriangleF s, Triangle3 t, int bx1, int bx2, int by, int tx, int ty, bool PHONGSHADE = false)` = Needs to be improved
- `void fillTopFlat(TriangleF s, Triangle3 t, int bx1, int bx2, int by, int tx, int ty, bool PHONGSHADE = false)` = Needs to be improved
- `fillTopFlat(TriangleF s, Triangle3 t, float bx1, float bx2, float by, float tx, float ty, bool PHONGSHADE = false)`
- `fillBotFlat(TriangleF s, Triangle3 t, float bx1, float bx2, float by, float tx, float ty, bool PHONGSHADE = false)`
- `fillTriangleFScan(TriangleF s, Triangle3 T, bool PHONGSHADE = false)`