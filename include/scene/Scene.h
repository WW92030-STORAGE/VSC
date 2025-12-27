#ifndef SCENE_EXT
#define SCENE_EXT

#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <cfloat>
#include <climits>
#include <unordered_map>

#include "../objects/Mesh.h"
#include "../objects/MorphedMesh.h"
#include "../objects/RiggedMesh.h"
#include "../objects/Triangle.h"
#include "../objects/Object.h"
#include "../objects/Clip.h"

#include "../camera/Camera.h"
#include "../utils/geometry/Fragment.h"

#include "Color.h"
#include "../material/Material.h"
#include "../material/ImageTexture.h"

#include "../lighting/Light.h"
#include "../lighting/PointLight.h"
#include "../lighting/DirectionalLight.h"

#include "Shader.h"

#include <optional>

/*

Base scene class. Supports the rendering of individual meshes, but also the bulk storage and rendering of meshes.

*/

class Scene { // CENA!
	public:
	std::vector<Mesh*> meshes;
	std::vector<std::optional<FragShader>> frag_shaders;
	std::optional<FragShader> global_shader = std::nullopt;
	std::vector<BaseMaterial*> materials;
	std::vector<bool> NormInterps;

	std::vector<std::string> names;
	std::unordered_map<std::string, uint64_t> names_inv;
	// int numObjs;

	int W, H;
	int SIDE;

	uint64_t TRIANGLE_COUNT = 0;
	uint64_t TRIFRAG_COUNT = 0;
	uint64_t TRIFRAG_AVOID = 0;



	std::queue<std::pair<Triangle3, std::pair<BaseMaterial*, bool>>> triqueue;

	std::vector<Camera> cameras; // camera vector 
	Camera camera; // active camera
	int active_camera = 0; // camera active

	ReducedFrag** buffer; // Coordinates are (x, y) where x goes right and y up. This is to align with the projection matrix.
	Vector3 ambientLight = Vector3(0.1, 0.1, 0.1);

	std::vector<Light> lights;

	// Functions

	/*
	
	WHAT IS HERE IS ORITINALLY TAKEN FROM RAYTRACER AND GENERALIZED
	
	*/

	// Add a mesh to the system. WARNING - This copies the mesh in question.
	void addMesh(Mesh* mesh, BaseMaterial* mat = nullptr, bool INTERPNORM = false, std::string name = "", std::optional<FragShader> frag_shader = std::nullopt);

	// Get the mesh of a name
	int getMesh(std::string name);

	// Render the scene, assuming what is stored in meshes, etc. are the desired objects
	virtual void render(bool LIT = true, int depth = 0, std::optional<FragShader> shader = std::nullopt);

		// Object Modification

	void morph(int index, std::vector<float> states);

	void morphToState(int index, int state);

	uint64_t countTriangles();



	/*
	
	EVERYTHING ELSE HERE IS BASICALLY COPIED FROM THE LEGACY SCENE
	
	*/
	
	void initBuffer();
	// Sorry but you're only getting these 2 constructors. Do the rest of it yourself.
	Scene();

	Scene(int w, int h);
	void setActiveCamera(int x);

	void kill();

	virtual ~Scene();

	void clearBuffer();

	void fillScreen(int32_t c);

	// Draw 2d primitives

	void drawPixel(int x, int y, uint32_t z);

	void drawFragment(Fragment& F, int x, int y);

	// Get color from material

	uint32_t getColor(BaseMaterial* material, Vector2 uv);

	// Handle the lighting: compute the illumination of a given point.
	// Params: pRay = source ray direction (from the light source)
	// position = position in EUCLIDEAN space
	// material = Material used to do the calcs
	// LIT = debug

	virtual Vector3 shade(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true);

	uint32_t shadeInt(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true);

	// Actual raytracing. The pRay is the slope of the ray, the position is the intersection point.
	Vector3 illuminate(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true);

	uint32_t illuminateInt(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true);

	// The rest of this class deals with drawing primitives such as triangles.

	// Bresanham's method

	// Draw a line going right at a shallow slope
	void LineLow(int x0, int y0, int x1, int y1, uint32_t c);

	// Draw a line going up at a steep slope
	void LineHigh(int x0, int y0, int x1, int y1, uint32_t c);

	void drawLine(int x0, int y0, int x1, int y1, uint32_t c);

	void drawLine(Vector2& a, Vector2& b, uint32_t c);

	// Draw a TRIANGLE

	void drawTriangle(Triangle2 s, uint32_t c);

	// Draw a TRIANGLE with the insides colored in
	
	void fillTriangle(Triangle2& s, uint32_t c);

	// Draw and Fill Triangles in 3D!!!

	// Backface cull
	bool BackFaceCull(Triangle3 s);

	// The updated version takes a Triangle3 and returns a triangle of fragments.
	// The fragments for now only have normalized device coordinates, the original triangle normal, the color, and teh uvs of the original triangle.
	// The triangle is returned with the coordinates as they are represented in NDC. However
	// However the x and y coordinates are transformed to fit in the buffer.
	TriangleF project(Triangle3 s, uint32_t c = 0xFFFFFFFF);

	// Draw fragment triangles. We will expand this in the future.
	void drawTriangle(TriangleF s, uint32_t c);

	void DrawTriFrag(TriangleF s, Triangle3 t, int x, int y, bool PHONGSHADE = false, std::optional<FragShader> shader = std::nullopt, bool INTERP__ = true);

	// A scanline based algorithm 
	// https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/

	int orient2d(Vector2i a, Vector2i b, Vector2i c);

	float orient2dF(Vector2 a, Vector2 b, Vector2i c);

	Vector2i vec2i(Vector4 v);

	void fillTriangleFScan(TriangleF s, Triangle3 T, bool PHONGSHADE = false, std::optional<FragShader> shader = std::nullopt);

	void fillTriangleFScanF(TriangleF s, Triangle3 T, bool PHONGSHADE = false, std::optional<FragShader> shader = std::nullopt);

	bool smallEnough(TriangleF s);

	void fillTriangle(TriangleF s, Triangle3 T, bool PHONGSHADE = false, std::optional<FragShader> shader = std::nullopt, bool SCAN = true);

	// Draw 3d triangles

	bool clip(TriangleF p);

	int intersectsPlane(Triangle3& tri, Plane& p);

	// Right now this always draws triangles using the base color.
	void drawTriangle(Triangle3 s, BaseMaterial* material = nullptr, Vector3* vn = nullptr, Vector2* uv = nullptr, bool BACKFACECULL = false, bool PHONGSHADE = false, bool INTERPNORM = false);

	void fillTriangle(Triangle3 s, BaseMaterial* material = nullptr, Vector3* vn = nullptr, Vector2* uv = nullptr, std::optional<FragShader> shader = std::nullopt, bool BACKFACECULL = true, bool PHONGSHADE = false, bool INTERPNORM = false, bool edgeclip = true);



	// draw a MESH

	void drawMesh(Mesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = false);

	void drawMesh(MorphedMesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = false) ;

	void fillMesh(Mesh& m, BaseMaterial* material = nullptr, std::optional<FragShader> shader = std::nullopt, bool SMOOTHSHADE = true, bool PHONGSHADE = true, bool INTERPNORM = false, bool BACKFACECULL = true);

	void fillMesh(MorphedMesh& m, BaseMaterial* material = nullptr, std::optional<FragShader> shader = std::nullopt, bool SMOOTHSHADE = true, bool PHONGSHADE = true, bool INTERPNORM = false, bool BACKFACECULL = true);

	// Queue various meshes and triangles to be drawn. WARNING - This will result in flat shading.

	void QueueTriangle(Triangle3 s, BaseMaterial* material, bool FILL = false, bool BACKFACECULL = true);

	void QueueMesh(Mesh& m, BaseMaterial* material, bool FILL = true);

	void drawQueue();


	// Resolution of a fragment or color in grayscale. https://en.wikipedia.org/wiki/Luma_(video)

	float getResolution(Vector4 s);

	float getResolution(uint32_t c);

	std::string to_string();

	std::string buffer_data();

	void outputFrags(std::string OUTPUT__);

	void outputBuffer(std::string OUTPUT__);

	std::vector<std::vector<uint32_t>> bufferMatrix();

	void setBuffer(std::vector<std::vector<uint32_t>>& res);
};

#endif