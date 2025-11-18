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
	void addMesh(Mesh* mesh, BaseMaterial* mat = nullptr, bool INTERPNORM = false, std::string name = "", std::optional<FragShader> frag_shader = std::nullopt) {
		if (name == "") name = "Mesh" + std::to_string(meshes.size());
		frag_shaders.push_back(frag_shader);

		names.push_back(name);
		names_inv[name] = meshes.size();

		MorphedMesh* morphed = dynamic_cast<MorphedMesh*>(mesh);
		RiggedMesh* rigged = dynamic_cast<RiggedMesh*>(mesh);
		if (morphed != nullptr) meshes.push_back(new MorphedMesh(*morphed));
		else if (rigged) meshes.push_back(new RiggedMesh(*rigged));
		else meshes.push_back(new Mesh(*mesh));

		if (mat != nullptr) {
			ImageTexture* imgtex = dynamic_cast<ImageTexture*>(mat);
			if (imgtex != nullptr) materials.push_back(new ImageTexture(*imgtex));
			else materials.push_back(new BaseMaterial(*mat));
		}
		else materials.push_back(new BaseMaterial(BASEMAT_WHITE));
		NormInterps.push_back(INTERPNORM);
	}

	// Get the mesh of a name
	inline int getMesh(std::string name) {
		if (names_inv.find(name) == names_inv.end()) return -1;
		return names_inv[name];
	}

	// Render the scene, assuming what is stored in meshes, etc. are the desired objects
	virtual void render(bool LIT = true, int depth = 0, std::optional<FragShader> shader = std::nullopt) {
		clearBuffer();
		global_shader = shader;
		for (int index = 0; index < meshes.size(); index++) {
			fillMesh(*(meshes[index]), materials[index], frag_shaders[index], true, LIT, NormInterps[index], true); 
		}
	}

		// Object Modification

	void morph(int index, std::vector<float> states) {
		if (index < 0 || index >= meshes.size()) return;

		Mesh* m = meshes[index];
		MorphedMesh* mm = dynamic_cast<MorphedMesh*>(m);
		if (mm == nullptr) return;
		mm->morph(states);
	}

	void morphToState(int index, int state) {
		if (index < 0 || index >= meshes.size()) return;

		Mesh* m = meshes[index];
		MorphedMesh* mm = dynamic_cast<MorphedMesh*>(m);
		if (mm == nullptr) return;
		mm->morphToState(state);
	}

	uint64_t countTriangles() {
		uint64_t res = 0;
		for (auto i : meshes) res += i->triindices.size();
		return res;
	}



	/*
	
	EVERYTHING ELSE HERE IS BASICALLY COPIED FROM THE LEGACY SCENE
	
	*/
	
	inline void initBuffer() {
		SIDE = H > W ? H : W;
		buffer = new ReducedFrag*[W];
		for (int i = 0; i < W; i++) buffer[i] = new ReducedFrag[H];
		for (int i = 0; i < W; i++) {
			for (int j =- 0; j < H; j++) buffer[i][j] = ReducedFrag();
		}
	}

	// Sorry but you're only getting these 2 constructors. Do the rest of it yourself.
	Scene() {
		camera = Camera();
		cameras.push_back(camera);
		// numObjs = 0;

		W = 256;
		H = 256;

		SIDE = H > W ? H : W;

		initBuffer();
	}

	Scene(int w, int h) {
		camera = Camera();
		cameras.push_back(camera);
		// numObjs = 0;

		W = w;
		H = h;

		SIDE = H > W ? H : W;

		initBuffer();
	}

	inline void setActiveCamera(int x) {
		active_camera = x;
		camera = cameras[x];
	}

	void kill() {
		for (int i = 0; i < W; i++) delete[] buffer[i];
		delete[] buffer;
		for (int i = 0; i < meshes.size(); i++) {
			Mesh* m = meshes[i];
			MorphedMesh* mm = dynamic_cast<MorphedMesh*>(m);
			RiggedMesh* rm = dynamic_cast<RiggedMesh*>(m);
			if (mm) delete mm;
			else if (rm) delete rm;
			else delete m; 
		}
		
		for (int i = 0; i < materials.size(); i++) {
			BaseMaterial* bm = materials[i];
			ImageTexture* im = dynamic_cast<ImageTexture*>(bm);
			if (im) delete im;
			else delete bm;
		}
	}

	virtual ~Scene() {
		kill();
	}

	inline void clearBuffer() {
		fillScreen(0x000000FF);
	}

	void fillScreen(int32_t c) {
		for (int i = 0; i < W; i++) {
			for (int j = 0; j < H; j++) {
				buffer[i][j] = ReducedFrag();
				buffer[i][j].color = c;
			}
		}
	}

	// Draw 2d primitives

	inline void drawPixel(int x, int y, uint32_t z) {
		if (x < 0 || y < 0 || x >= W || y >= H) return;
		buffer[x][y] = ReducedFrag(buffer[x][y]);
		buffer[x][y].color = z;
	}

	inline void drawFragment(Fragment& F, int x, int y) {
		if (x < 0 || y < 0 || x >= W || y >= H) return;
		if (F.ndc.z < -2 || F.ndc.z > 2) return;
		if (buffer[x][y].depth < F.ndc.w) return;
		buffer[x][y] = ReducedFrag(F.ndc.w, F.color);
	}

	// Get color from material

	inline uint32_t getColor(BaseMaterial* material, Vector2 uv) {
		if (material->TYPE == BaseMaterial::IMAGE) {
			ImageTexture* imgtex = dynamic_cast<ImageTexture*>(material);
			if (imgtex) return imgtex->getColor(uv);
		}
		return material->getColor(uv);
	}

	// Handle the lighting: compute the illumination of a given point.
	// Params: pRay = source ray direction (from the light source)
	// position = position in EUCLIDEAN space
	// material = Material used to do the calcs
	// LIT = debug

	virtual Vector3 shade(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true) {
		// std::cout << pRay.to_string() << " " << position.to_string() << " " << normal.to_string() << "\n";
		
		Vector3 col = rgb(getColor(material, uv));
		if (!LIT) return col;
		
		Vector3 I(ambientLight);

		for (auto light : lights) {
			if (light.TYPE == POINT) {
				Light pl = light;
				Vector3 L = pl.transform.origin - position;
				Vector3 shadedIntensity(0, 0, 0);

				// Diffuse term
				float scale = normal.cosine(L);
				// Only do light contribution if normal is facing light
				if (scale > 0) {
					shadedIntensity = shadedIntensity + (pl.intensity * scale);

					// Specular term

					if (material->specular > 0) {

						Vector3 reflected = normal * (2 * normal.dot(L)) - L;
						float rv_inv = reflected.cosine(pRay);
						if (rv_inv < 0) {
							float spec = powf(rv_inv * -1, material->specular);
							shadedIntensity = shadedIntensity + (pl.intensity * spec);
						// std::cout << (pl.intensity * spec).to_string() << " " << specular << "\n";
						}
					}

				}

				shadedIntensity = shadedIntensity / fmax(pl.attenuation * L.length(), 1);
				
				I = I + shadedIntensity;
			}
			if (I.x > 1) I.x = 1;
			if (I.y > 1) I.y = 1;
			if (I.z > 1) I.z = 1;
		}

		// std::cout << position.to_string() << " = " << I.to_string() << "\n";


		Vector3 res(col.x * I.x, col.y * I.y, col.z * I.z);
		return res;
	}

	inline uint32_t shadeInt(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true) {
		return rgb(shade(pRay, position, normal, uv, material, LIT));
	}

	// Actual raytracing. The pRay is the slope of the ray, the position is the intersection point.
	inline Vector3 illuminate(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true) {
		return shade(pRay, position, normal, uv, material, LIT);
	}

	inline uint32_t illuminateInt(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true) {
		return rgb(illuminate(pRay, position, normal, uv, material, LIT));
	}

	// The rest of this class deals with drawing primitives such as triangles.

	// Bresanham's method

	// Draw a line going right at a shallow slope
	void LineLow(int x0, int y0, int x1, int y1, uint32_t c) {
		int dx = x1 - x0;
		int dy = y1 - y0;

		int yi = 1;

		if (dy < 0) {
			yi = -1;
			dy = -dy;
		}

		int D = 2 * dy - dx;

		int y = y0;

		for (int x = x0; x <= x1; x++) {
			drawPixel(x, y, c);
			if (D > 0) {
				y += yi;
				D += 2 * (dy - dx);
			}
			else {
				D += 2 * dy;
			}
		}
	}

	// Draw a line going up at a steep slope
	void LineHigh(int x0, int y0, int x1, int y1, uint32_t c) {
		int dx = x1 - x0;
		int dy = y1 - y0;

		int xi = 1;

		if (dx < 0) {
			xi = -1;
			dx = -dx;
		}

		int D = 2 * dx - dy;

		int x = x0;

		for (int y = y0; y <= y1; y++) {
			drawPixel(x, y, c);
			if (D > 0) {
				x += xi;
				D += 2 * (dx - dy);
			}
			else {
				D += 2 * dx;
			}
		}
	}

	void drawLine(int x0, int y0, int x1, int y1, uint32_t c)  {
		if (x0 == x1) {
			for (int y = BASE::min(y0, y1); y <= BASE::max(y0, y1); y++) drawPixel(x0, y, c);
			return;
		}
		else if (y0 == y1) {
			for (int x = BASE::min(x0, x1); x <= BASE::max(x0, x1); x++) drawPixel(x, y0, c);
		}
		if (abs(y1 - y0) < abs(x1 - x0)) {
        	if (x0 > x1) LineLow(x1, y1, x0, y0, c);
        	else LineLow(x0, y0, x1, y1, c);
		}
    	else {
        	if (y0 > y1) LineHigh(x1, y1, x0, y0, c);
        	else LineHigh(x0, y0, x1, y1, c);
		}
	}

	inline void drawLine(Vector2& a, Vector2& b, uint32_t c) {
		drawLine(round(a.x), round(a.y), round(b.x), round(b.y), c);
	}

	// Draw a TRIANGLE

	inline void drawTriangle(Triangle2 s, uint32_t c) {
		for (int i = 0; i < 3; i++) drawLine(s.p[i], s.p[(i + 1) % 3], c);
	}

	// Draw a TRIANGLE with the insides colored in
	
	void fillTriangle(Triangle2& s, uint32_t c) {
		int x0 = s.p[0].x;
		int x1 = x0;
		int y0 = s.p[0].y;
		int y1 = y0;

		for (int i = 0; i < 3; i++) {
			if (x0 > s.p[i].x) x0 = s.p[i].x;
			if (x1 < s.p[i].x) x1 = s.p[i].x;
			if (y0 > s.p[i].y) y0 = s.p[i].y;
			if (y1 < s.p[i].y) y1 = s.p[i].y;
		}

		for (int x = x0; x <= x1; x++) {
			for (int y = y0; y <= y1; y++) {
				if (s.inside(Vector2(x, y))) drawPixel(x, y, c);
			}
		}
	}

	// Draw and Fill Triangles in 3D!!!

	// Backface cull
	inline bool BackFaceCull(Triangle3 s) {
		return (s.N * (s.p[0] - camera.transform.origin)) >= 0;
	}

	// The updated version takes a Triangle3 and returns a triangle of fragments.
	// The fragments for now only have normalized device coordinates, the original triangle normal, the color, and teh uvs of the original triangle.
	// The triangle is returned with the coordinates as they are represented in NDC. However
	// However the x and y coordinates are transformed to fit in the buffer.
	TriangleF project(Triangle3 s, uint32_t c = 0xFFFFFFFF) {
		
		Vector4 res[3];

		for (int i = 0; i < 3; i++) {
			Vector4 v = vec4(s.p[i]);
			v.w = 1;
			Vector4 vv = camera.glFrustum(v);
			res[i] = vv;

			res[i].x = (res[i].x + 1) * 0.5 * SIDE - 0.5 * (SIDE - W);
			res[i].y = (res[i].y + 1) * 0.5 * SIDE - 0.5 * (SIDE - H);
		}

		TriangleF tri;

		tri.ON = (s.normal());

		for (int i = 0; i < 3; i++) {
			tri.p[i] = Fragment(res[i], s.N, c, s.uv[i]);
			tri.p[i].wspos = s.p[i];
		}

		return tri;
	}

	// Draw fragment triangles. We will expand this in the future.
	inline void drawTriangle(TriangleF s, uint32_t c) {
		Triangle2 t(vec2(s.p[0].ndc), vec2(s.p[1].ndc), vec2(s.p[2].ndc));
		drawTriangle(t, c);
	}

	void DrawTriFrag(TriangleF s, Triangle3 t, int x, int y, bool PHONGSHADE = false, std::optional<FragShader> shader = std::nullopt) {
		Vector3 b = s.bary(x, y);
		float winv[3] = {1.0 / s.p[0].ndc.w, 1.0 / s.p[1].ndc.w, 1.0 / s.p[2].ndc.w};
		float wc = 1.0 / s.interp_given_bary(b, winv[0], winv[1], winv[2]);
		// DEPTH TEST!!!!!
		if (buffer[x][y].depth < wc) return;


		float zc = s.interp_given_bary(b, s.p[0].ndc.z, s.p[1].ndc.z, s.p[2].ndc.z);
		

		// for (int i = 0; i < 3; i++) std::cout << "UV " << s.p[i].uv.to_string() << " ";
		// std::cout << "\n";
		
		// std::cout << s.to_string() << " " << x <<  " " << y << " = " << b.to_string() << "\n\n";

		// Interpolate uv TODO - make this perspective correct
		// u/z and v/z linearly interpolate



		Vector2 finaluv;
		for (int i = 0; i < 3; i++) finaluv = finaluv + ((s.p[i].uv * winv[i]) * b.get(i));
		finaluv = finaluv * wc;

		// And same goes for world space position
		Vector3 finalwsp(0, 0, 0);
		for (int i = 0; i < 3; i++) finalwsp = finalwsp + ((s.p[i].wspos) * winv[i]) * b.get(i);
		finalwsp = finalwsp * wc;


		uint32_t c = getColor(s.material, finaluv);
		uint32_t oc = c;
		// std::cout << finaluv.to_string() << " " << c << " " << (s.material.TYPE == BaseMaterial::IMAGE) << "\n";
		if (PHONGSHADE) {
			Vector3 point;
			// for (int ss = 0; ss < 3; ss++) point = point + (t.p[ss] * b.get(ss));
			for (int ss = 0; ss < 3; ss++) point = point + (t.p[ss] * winv[ss]) * b.get(ss);
			point = point * wc;

			// std::cout << t.p[0].to_string() << " " << t.p[1].to_string() << " " << t.p[2].to_string() << " + " << b.to_string() << " = " << point.to_string() << "\n";



			Vector3 interpnormal;
			for (int ss = 0; ss < 3; ss++) interpnormal = interpnormal + s.p[ss].normal * b.get(ss);
			interpnormal = interpnormal.normalized();

			// std::cout << s.material.to_string() << "\n";
			c = rgb(illuminate(point, point, interpnormal, finaluv, s.material));
		} else {
			c = rgb(illuminate(t.centroid(), t.centroid(), s.N, finaluv, s.material));
		}
		Fragment F__F(Vector4(x, y, zc, wc), (s.ON), c, finaluv, oc);
		F__F.screenUV = Vector2((float)(x) / W, (float)(y) / H);
		F__F.wspos = (finalwsp);

		// std::cout << "DrawTriFrag " << (uint64_t)(shader) << "\n";

		if (shader.has_value()) {
			// std::cout << "SHADER DETECTED " << F__F.to_string() << "\n";
			F__F = shader.value()(F__F);
		}
		if (global_shader.has_value()) {
			F__F = global_shader.value()(F__F);
		}

		// std::cout << s.bary(x, y).to_string() << " " << F.ndc.z << " " << F.ndc.w << " = " << F.color << "\n";
		drawFragment(F__F, x, y);
	}

	// A scanline based algorithm 
	// https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/

	int orient2d(Vector2i a, Vector2i b, Vector2i c) {
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	}

	inline Vector2i vec2i(Vector4 v) {
		return Vector2i(BASE::ifloor(v.x), BASE::ifloor(v.y));
	}

	void fillTriangleFScan(TriangleF s, Triangle3 T, bool PHONGSHADE = false, std::optional<FragShader> shader = std::nullopt) {
		Vector2i v0 = vec2i(s.p[0].ndc);
		Vector2i v1 = vec2i(s.p[1].ndc);
		Vector2i v2 = vec2i(s.p[2].ndc);

		
		Vector2i vv[3] = {v0, v1, v2};

    	Vector2i e01(v0.y - v1.y, v1.x - v0.x);
		Vector2i e12(v1.y - v2.y, v2.x - v1.x);
		Vector2i e20(v2.y - v0.y, v0.x - v2.x);

		Vector2i BL = vec2i(s.p[0].ndc);
		Vector2i TR(BL);
		for (int i = 1; i < 3; i++) {
			BL.x = std::min(BL.x, vv[i].x);
			BL.y = std::min(BL.y, vv[i].y);
			TR.x = std::max(TR.x, vv[i].x);
			TR.y = std::max(TR.y, vv[i].y);
		}

		BL.x = std::max(BL.x, 0);
		BL.y = std::max(BL.y, 0);

		TR.x = std::min(TR.x, W - 1);
		TR.y = std::min(TR.y, H - 1);

		Vector2i p(BL);

    	int64_t w0_row = orient2d(v1, v2, p);
    	int64_t w1_row = orient2d(v2, v0, p);
    	int64_t w2_row = orient2d(v0, v1, p);

		for (p.y = BL.y; p.y <= TR.y; p.y++) {
        	int64_t w0 = w0_row;
        	int64_t w1 = w1_row;
        	int64_t w2 = w2_row;
        
        	for (p.x = BL.x; p.x <= TR.x; p.x++) {
        	    // If p is on or inside all edges, render pixel.
        	    if ((w0 | w1 | w2) >= 0) {
					DrawTriFrag(s, T, p.x, p.y, PHONGSHADE, shader);
				}

        	    // One step to the right
        	    w0 += e12.x;
        	    w1 += e20.x;
        	    w2 += e01.x;
        	}

        	// One row step
        	w0_row += e12.y;
        	w1_row += e20.y;
        	w2_row += e01.y;
    	}
	}

	inline bool smallEnough(TriangleF s) {
		Vector2 BL = vec2(s.p[0].ndc);
		Vector2 TR = vec2(s.p[0].ndc);

		for (int i = 0; i < 3; i++) {
			BL.x = std::min(BL.x, s.p[i].ndc.x);
			BL.y = std::min(BL.x, s.p[i].ndc.y);

			TR.x = std::max(BL.x, s.p[i].ndc.x);
			TR.y = std::max(BL.x, s.p[i].ndc.y);
		}

		const int THRESHOLD = 2;

		return (TR.y - BL.y <= THRESHOLD) && (TR.x - BL.x <= THRESHOLD);
	}

	void fillTriangle(TriangleF s, Triangle3 T, bool PHONGSHADE = false, std::optional<FragShader> shader = std::nullopt, bool SCAN = true) {
		if (SCAN && !smallEnough(s)) {
			fillTriangleFScan(s, T, PHONGSHADE, shader);
			return;
		}

		int x0 = s.p[0].ndc.x;
		int x1 = x0;
		int y0 = s.p[0].ndc.y;
		int y1 = y0;

		for (int i = 0; i < 3; i++) {
			if (x0 > s.p[i].ndc.x) x0 = s.p[i].ndc.x;
			if (x1 < s.p[i].ndc.x) x1 = s.p[i].ndc.x;
			if (y0 > s.p[i].ndc.y) y0 = s.p[i].ndc.y;
			if (y1 < s.p[i].ndc.y) y1 = s.p[i].ndc.y;
		}

		// std::cout << "FillTriangleF " << (uint64_t)(shader ) << "\n";

		for (int x = BASE::max(x0, 0); x <= x1 && x < W; x++) {
			for (int y = BASE::max(y0, 0); y <= y1 && y < H; y++) {
				if (!s.inside(Vector2(x, y))) continue;
				// std::cout << "FillTriangleF " << (uint64_t)(shader ) << "\n";
				DrawTriFrag(s, T, x, y, PHONGSHADE, shader);
			}
		}
	}

	// Draw 3d triangles

	inline bool clip(TriangleF p) {
		return false;
		// return p.centroid().z > 1 || p.centroid().z < -1;
		for (int i = 0; i < 3; i++) {
			if (p.p[i].ndc.z >= -1 && p.p[i].ndc.z <= 1) return false;
		}
		return true;
	}

	// Right now this always draws triangles using the base color.
	void drawTriangle(Triangle3 s, BaseMaterial* material = nullptr, Vector3* vn = nullptr, Vector2* uv = nullptr, bool BACKFACECULL = false, bool PHONGSHADE = false, bool INTERPNORM = false) {
		if (!material) material = new BaseMaterial(BASEMAT_WHITE);
		
		TRIANGLE_COUNT++;
		if (BACKFACECULL && BackFaceCull(s)) return;
		Vector3 cen = s.centroid();
		auto p = project(s);
		p.material = material;
		if (!uv) uv = new Vector2[3] {Vector2(), Vector2(), Vector2()};
		if (!vn) PHONGSHADE = false;
		if (vn) for (int i = 0; i < 3; i++) p.p[i].normal = INTERPNORM ? vn[i] : s.N;
		if (uv) for (int i = 0; i < 3; i++) p.p[i].uv = uv[i];
		for (int i = 0; i < 3; i++) p.p[i].color = rgb(illuminate(s.p[i], s.p[i], p.p[i].normal, uv[i], p.material));
		if (clip(p)) return;
		drawTriangle(p, rgb(material->baseColor));
	}

	void fillTriangle(Triangle3 s, BaseMaterial* material = nullptr, Vector3* vn = nullptr, Vector2* uv = nullptr, std::optional<FragShader> shader = std::nullopt, bool BACKFACECULL = true, bool PHONGSHADE = false, bool INTERPNORM = false, bool edgeclip = true) {
		if (edgeclip) {
			std::vector<Triangle3> tt = TriSplit(s, camera.F).first;

			Plane arr[5] = {camera.N, camera.U, camera.L, camera.D, camera.R};
			for (int ii = 0; ii < 5; ii++) {

			std::vector<Triangle3> vv;
			for (auto i : tt) {
				auto uu = TriSplit(i, arr[ii]).first;
				for (auto j : uu) vv.push_back(j);
			}
			std::swap(tt, vv);
			vv.clear();

			}
			Vector3* i_vn = nullptr;
			Vector2* i_uv = nullptr;
			if (vn) i_vn = new Vector3[3];
			if (uv) i_uv = new Vector2[3];

			for (auto ii : tt) {

				for (int i = 0; i < 3; i++) {
					Vector3 point(ii.p[i]);
					if (vn) i_vn[i] = s.interp<Vector3>(point, vn[0], vn[1], vn[2]);
					if (uv) i_uv[i] = s.interp<Vector2>(point, uv[0], uv[1], uv[2]);
				}
				fillTriangle(ii, material, i_vn, i_uv, shader, BACKFACECULL, PHONGSHADE, INTERPNORM, false);
			}

			delete[] i_vn;
			delete[] i_uv;


			return;
		}
		if (!material) material = new BaseMaterial(BASEMAT_WHITE);
		
		TRIANGLE_COUNT++;
		if (BACKFACECULL && BackFaceCull(s)) {
			// std::cout << "CULLED\n";
			return;
		}
		Vector3 cen = s.centroid();

		TriangleF p = project(s);
		p.material = material;
		if (!uv) uv = new Vector2[3] {Vector2(), Vector2(), Vector2()};
		if (!vn) PHONGSHADE = false;
		if (vn) for (int i = 0; i < 3; i++) p.p[i].normal = INTERPNORM ? vn[i] : s.N;
		if (uv) for (int i = 0; i < 3; i++) p.p[i].uv = uv[i];

		for (int i = 0; i < 3; i++) p.p[i].color = rgb(illuminate(s.p[i], s.p[i], p.p[i].normal, uv[i], p.material));
		
		if (clip(p)) return;
		// std::cout << "FillTriangle3 " << (uint64_t)(shader) << "\n";
		fillTriangle(p, s, PHONGSHADE, shader);
	}



	// draw a MESH

	void drawMesh(Mesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = false) {
		bool matprovided = material;
		if (!material) material = new BaseMaterial(BASEMAT_WHITE);
		
		for (int i = 0; i < m.size; i++) {
			Vector3* vn = new Vector3[3];
			for (int s = 0; s < 3; s++) vn[s] = m.getVertexNormal(m.triindices[i][s]);
			// std::cout << "normals " << vn[0].to_string() << " " << vn[1].to_string() << " " << vn[2].to_string() << "\n";
			Vector2* vt = new Vector2[3];
			for (int s = 0; s < 3; s++) vt[s] = m.getVertexUV(i, s);

			// std::cout << "tex " << vt[0].to_string() << " " << vt[1].to_string() << " " << vt[2].to_string() << "\n";

			bool isna = false;
			for (int s = 0; s < 3; s++) {
				if (vn[s] == Vector3()) isna = true;
			}
			isna |= SMOOTHSHADE;

			bool isnat = false;
			for (int s = 0; s < 3; s++) {
				if (vt[s] == NILVEC2) isnat = true;
			}

			drawTriangle(m.makeTriangle(i), material, isna ? nullptr : vn, isnat ? nullptr: vt, BACKFACECULL, PHONGSHADE, INTERPNORM);
			delete[] vn;
			delete[] vt;
		}

		if (!matprovided) delete material;
	}

	void drawMesh(MorphedMesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = false) {
		bool matprovided = material;
		if (!material) material = new BaseMaterial(BASEMAT_WHITE);
		
		for (int i = 0; i < m.size; i++) {
			Vector3* vn = new Vector3[3];
			for (int s = 0; s < 3; s++) vn[s] = m.getVertexNormal(m.triindices[i][s]);
			// std::cout << "normals " << vn[0].to_string() << " " << vn[1].to_string() << " " << vn[2].to_string() << "\n";
			Vector2* vt = new Vector2[3];
			for (int s = 0; s < 3; s++) vt[s] = m.getVertexUV(i, s);

			// std::cout << "tex " << vt[0].to_string() << " " << vt[1].to_string() << " " << vt[2].to_string() << "\n";

			bool isna = false;
			for (int s = 0; s < 3; s++) {
				if (vn[s] == Vector3()) isna = true;
			}
			isna |= SMOOTHSHADE;

			bool isnat = false;
			for (int s = 0; s < 3; s++) {
				if (vt[s] == NILVEC2) isnat = true;
			}

			drawTriangle(m.makeTriangle(i), material, isna ? nullptr : vn, isnat ? nullptr: vt, BACKFACECULL, PHONGSHADE, INTERPNORM);
			delete[] vn;
			delete[] vt;
		}

		if (!matprovided) delete material;
	}

	void fillMesh(Mesh& m, BaseMaterial* material = nullptr, std::optional<FragShader> shader = std::nullopt, bool SMOOTHSHADE = true, bool PHONGSHADE = true, bool INTERPNORM = false, bool BACKFACECULL = true) {
		bool matprovided = material;
		if (!material) material = new BaseMaterial(BASEMAT_WHITE);
		
		// for (int i = 0; i < m.nverts; i++) std::cout << m.verts[i].to_string() << ".";
		// std::cout << "\n" << m.nverts << "\n";
		for (int i = 0; i < m.size; i++) {
			// if (i % 64 == 0) std::cout << i << "/" << m.size << "...\n";
			Vector3* vn = new Vector3[3];
			for (int s = 0; s < 3; s++) vn[s] = m.getVertexNormal(m.triindices[i][s]);

			Vector2* vt = new Vector2[3];
			for (int s = 0; s < 3; s++) vt[s] = m.getVertexUV(i, s);
			// for (int s = 0; s < 3; s++) std::cout << vt[s].to_string() << " ";
			/*
			std::cout << "\n";
			for (int t = 0; t < 3; t++) std::cout << m.tris[i].p[t].to_string() << " / " << vn[t].to_string() << "\n";
			std::cout << "\n" << m.tris[i].N.to_string() << "\n\n";
			*/
			
			bool isna = false;
			for (int s = 0; s < 3; s++) {
				if (vn[s] == Vector3()) isna = true;
			}
			isna |= !SMOOTHSHADE;


			bool isnat = false;
			for (int s = 0; s < 3; s++) {
				if (vt[s] == NILVEC2) isnat = true;
			}
			fillTriangle(m.makeTriangle(i), material, isna ? nullptr : vn, isnat ? nullptr : vt, shader, BACKFACECULL, PHONGSHADE, INTERPNORM);
			delete[] vn;
			delete[] vt;
		}

		if (!matprovided) delete material;
	}

	void fillMesh(MorphedMesh& m, BaseMaterial* material = nullptr, std::optional<FragShader> shader = std::nullopt, bool SMOOTHSHADE = true, bool PHONGSHADE = true, bool INTERPNORM = false, bool BACKFACECULL = true) {
		bool matprovided = material;
		if (!material) material = new BaseMaterial(BASEMAT_WHITE);
		
		// for (int i = 0; i < m.nverts; i++) std::cout << m.verts[i].to_string() << ".";
		// std::cout << "\n" << m.nverts << "\n";
		for (int i = 0; i < m.size; i++) {
			// if (i % 64 == 0) std::cout << i << "/" << m.size << "...\n";
			Vector3* vn = new Vector3[3];
			for (int s = 0; s < 3; s++) vn[s] = m.getVertexNormal(m.triindices[i][s]);

			Vector2* vt = new Vector2[3];
			for (int s = 0; s < 3; s++) vt[s] = m.getVertexUV(i, s);
			/*
			std::cout << "\n";
			for (int t = 0; t < 3; t++) std::cout << m.tris[i].p[t].to_string() << " / " << vn[t].to_string() << "\n";
			std::cout << "\n" << m.tris[i].N.to_string() << "\n\n";
			*/
			
			bool isna = false;
			for (int s = 0; s < 3; s++) {
				if (vn[s] == Vector3()) isna = true;
			}
			isna |= !SMOOTHSHADE;


			bool isnat = false;
			for (int s = 0; s < 3; s++) {
				if (vt[s] == NILVEC2) isnat = true;
			}
			fillTriangle(m.makeTriangle(i), material, isna ? nullptr : vn, isnat ? nullptr : vt, shader, BACKFACECULL, PHONGSHADE, INTERPNORM);
			delete[] vn;
			delete[] vt;
		}

		if (!matprovided) delete material;
	}

	// Queue various meshes and triangles to be drawn. WARNING - This will result in flat shading.

	inline void QueueTriangle(Triangle3 s, BaseMaterial* material, bool FILL = false, bool BACKFACECULL = true) {
		if (BACKFACECULL && BackFaceCull(s)) return;
		triqueue.push({s, {material, FILL}});
	}

	inline void QueueMesh(Mesh& m, BaseMaterial* material, bool FILL = true) {
		for (int i = 0; i < m.size; i++) QueueTriangle(m.makeTriangle(i), material, FILL);
	}

	inline void drawQueue() {
		while (triqueue.size()) {
			auto p = triqueue.front();
			triqueue.pop();

			if (p.second.second) fillTriangle(p.first, p.second.first);
			else drawTriangle(p.first, p.second.first);
		}
	}


	// Resolution of a fragment or color in grayscale. https://en.wikipedia.org/wiki/Luma_(video)

	inline float getResolution(Vector4 s) {
		float res = 0.2126 * s.x + 0.7152 * s.y + 0.0722 * s.z;
		// // std::cout << s.to_string() << "\n" << res << "\n";
		return res;
	}
	inline float getResolution(uint32_t c) {
		return getResolution(rgba(c));
	}

	inline std::string to_string() {
		std::string res = "";

		for (int y = H - 1; y >= 0; y--) {
			for (int x = 0; x < W; x++) {
				float f = getResolution(buffer[x][y].color);

				if (f < 0.2) res = res + "#";
				else if (f < 0.4) res = res + "X";
				else if (f < 0.6) res = res + "O";
				else if (f < 0.8) res = res + ".";
				else res = res + " ";
			}
			res = res + "\n";
		}
		return res;
	}

	inline std::string buffer_data() {
		std::string res = "[" + std::to_string(W) + ", " + std::to_string(H) + "]\n";

		for (int y = H - 1; y >= 0; y--) {
			// std::cout << y << "\n";
			for (int x = 0; x < W; x++) {
				res = res + std::to_string(buffer[x][y].color) + ",";
			}
			res = res + "\n";
		}
		return res;
	}

	void outputFrags(std::string OUTPUT__) {
		std::ofstream output(OUTPUT__);
		std::string res = "";
		for (int y = H - 1; y >= 0; y--) {
			for (int x = 0; x < W; x++) {
				res += buffer[x][y].to_string() + ",";
			}
			res.push_back('\n');
		}

		output << res;

		output.close();
	}

	void outputBuffer(std::string OUTPUT__) {
		std::ofstream output(OUTPUT__);
		output << "[" + std::to_string(W) + ", " + std::to_string(H) + "]\n";
		std::string buf = "";

		for (int y = H - 1; y >= 0; y--) {
			for (int x = 0; x < W; x++) {
				buf += std::to_string(buffer[x][y].color) + ",";
			}
			buf.push_back('\n');
		}

		output << buf;

		output.close();
	}

	std::vector<std::vector<uint32_t>> bufferMatrix() {
		std::vector<std::vector<uint32_t>> res(W, std::vector<uint32_t>(H, 0));
		for (int i = 0; i < W; i++) {
			for (int j = 0; j < H; j++) res[i][j] = buffer[i][j].color;
		}
		return res;
	}

	void setBuffer(std::vector<std::vector<uint32_t>>& res) {
		for (int i = 0; i < W && i < res.size(); i++) {
			for (int j = 0; j < H && j < res[i].size(); j++) res[i][j] = buffer[i][j].color;
		}
	}
};

#endif