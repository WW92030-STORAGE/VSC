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

#include "../objects/Mesh.h"
#include "../objects/MorphedMesh.h"
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

// Legacy version of the Scene class before VSC 9.8x

class LegacyScene { // CENA!
	public:
	std::vector<Mesh*> meshes;
	std::vector<BaseMaterial*> materials;
	std::vector<bool> NormInterps;
	// int numObjs;

	int W, H;
	int SIDE;

	uint64_t TRIANGLE_COUNT = 0;



	std::queue<std::pair<Triangle3, std::pair<BaseMaterial*, bool>>> triqueue;

	Camera camera;

	ReducedFrag** buffer; // Coordinates are (x, y) where x goes right and y up. This is to align with the projection matrix.

	Vector3 ambientLight = Vector3(0.1, 0.1, 0.1);

	std::vector<Light> lights;

	// Functions

	inline void initBuffer() {
		SIDE = H > W ? H : W;
		buffer = new ReducedFrag*[W];
		for (int i = 0; i < W; i++) buffer[i] = new ReducedFrag[H];
		for (int i = 0; i < W; i++) {
			for (int j =- 0; j < H; j++) buffer[i][j] = ReducedFrag();
		}
	}

	// Sorry but you're only getting these 2 constructors. Do the rest of it yourself.
	LegacyScene() {
		camera = Camera();
		// numObjs = 0;

		W = 256;
		H = 256;

		SIDE = H > W ? H : W;

		initBuffer();
	}

	LegacyScene(int w, int h) {
		camera = Camera();
		// numObjs = 0;

		W = w;
		H = h;

		SIDE = H > W ? H : W;

		initBuffer();
	}

	inline void clearBuffer() {
		fillScreen(0x000000FF);
	}

	inline void fillScreen(int32_t c) {
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

	inline virtual Vector3 shade(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true) {
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
				float scale = normal.normalized() * L.normalized();
				// Only do light contribution if normal is facing light
				if (scale > 0) {
					shadedIntensity = shadedIntensity + (pl.intensity * scale);

					// Specular term

					if (material->specular > 0) {

						Vector3 reflected = normal * (2 * normal.dot(L)) - L;
						float rv = reflected.normalized().dot((pRay * -1).normalized());
						if (rv > 0) {
							float spec = powf(rv, material->specular);
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
	inline void LineLow(int x0, int y0, int x1, int y1, uint32_t c) {
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
	inline void LineHigh(int x0, int y0, int x1, int y1, uint32_t c) {
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

	inline void drawLine(int x0, int y0, int x1, int y1, uint32_t c)  {
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
	
	inline void fillTriangle(Triangle2& s, uint32_t c) {
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
	inline TriangleF project(Triangle3 s, uint32_t c = 0xFFFFFFFF) {
		
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

		tri.ON = Vector3(s.normal());

		for (int i = 0; i < 3; i++) {
			tri.p[i] = Fragment(res[i], s.N, c, s.uv[i]);
		}

		return tri;
	}

	// Draw fragment triangles. We will expand this in the future.
	inline void drawTriangle(TriangleF s, uint32_t c) {
		Triangle2 t(vec2(s.p[0].ndc), vec2(s.p[1].ndc), vec2(s.p[2].ndc));
		drawTriangle(t, c);
	}

	inline void DrawTriFrag(TriangleF s, Triangle3 t, int x, int y, bool PHONGSHADE = false) {
		float zc = s.interp(x, y, s.p[0].ndc.z, s.p[1].ndc.z, s.p[2].ndc.z);
		float wc = 1.0 / s.interp(x, y, 1.0 / s.p[0].ndc.w, 1.0 / s.p[1].ndc.w, 1.0 / s.p[2].ndc.w);

		// for (int i = 0; i < 3; i++) std::cout << "UV " << s.p[i].uv.to_string() << " ";
		// std::cout << "\n";

		Vector3 b = s.bary(x, y);
		// if (b.x > 1 || b.y > 1 || b.z > 1) return;
		b.x = BASE::clamp(b.x, 0, 1);
		b.y = BASE::clamp(b.y, 0, 1);
		b.z = BASE::clamp(b.z, 0, 1);
		if (!BASE::fzero(b.x + b.y + b.z)) {
			float sum = b.x + b.y + b.z;
			b.x /= sum;
			b.y /= sum;
			b.z /= sum;
		}

		// std::cout << s.to_string() << " " << x <<  " " << y << " = " << b.to_string() << "\n\n";

		// Interpolate uv TODO - make this perspective correct
		// u/z and v/z linearly interpolate

		Vector2 finaluv;
		for (int i = 0; i < 3; i++) finaluv = finaluv + ((s.p[i].uv / s.p[i].ndc.w) * b.get(i));
		finaluv = finaluv * wc;


		uint32_t c = getColor(s.material, finaluv);
		uint32_t oc = c;
		// std::cout << finaluv.to_string() << " " << c << " " << (s.material.TYPE == BaseMaterial::IMAGE) << "\n";
		if (PHONGSHADE) {
			Vector3 point;
			// for (int ss = 0; ss < 3; ss++) point = point + (t.p[ss] * b.get(ss));
			for (int ss = 0; ss < 3; ss++) point = point + (t.p[ss] / s.p[ss].ndc.w) * b.get(ss);
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
		Fragment F__F(Vector4(x, y, zc, wc), Vector3(s.ON), c, finaluv, oc);

		// std::cout << s.bary(x, y).to_string() << " " << F.ndc.z << " " << F.ndc.w << " = " << F.color << "\n";
		drawFragment(F__F, x, y);
	}

	// Originally a method using three cross products, now using a variation on Bresenham's method.
	// https://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

	inline void fillTriangleFlatTop(TriangleF s, Triangle3 T, int bx1, int bx2, int by, int ax, int ay, bool PHONGSHADE = false) {
		if (bx1 > bx2) std::swap(bx1, bx2);
		float m1 = float(bx1 - ax) / float(by - ay);
		float m2 = float(bx2 - ax) / float(by - ay);

		float cx1 = ax;
		float cx2 = ax;
		for (int sy = ay; sy <= by; sy++) {
			for (int sx = BASE::ifloor(cx1); sx <= BASE::iceil(cx2); sx++) DrawTriFrag(s, T, sx, sy, PHONGSHADE);
			cx1 += m1;
			cx2 += m2;
		}
	}

	inline void fillTriangleFlatBottom(TriangleF s, Triangle3 T, int bx1, int bx2, int by, int ax, int ay, bool PHONGSHADE = false) {
		if (bx1 > bx2) std::swap(bx1, bx2);
		float m1 = float(bx1 - ax) / float(by - ay);
		float m2 = float(bx2 - ax) / float(by - ay);

		float cx1 = ax;
		float cx2 = ax;
		for (int sy = ay; sy >= by; sy--) {
			for (int sx = BASE::ifloor(cx1); sx <= BASE::iceil(cx2); sx++) DrawTriFrag(s, T, sx, sy, PHONGSHADE);
			cx1 -= m1;
			cx2 -= m2;
		}
	}

	inline void fillTriangleFScan(TriangleF s, Triangle3 T, bool PHONGSHADE = false) {
		if (s.p[1].ndc.y < s.p[0].ndc.y) {
			std::swap(s.p[0], s.p[1]);
			std::swap(T.p[0], T.p[1]);
		}
		if (s.p[2].ndc.y < s.p[0].ndc.y) {
			std::swap(s.p[2], s.p[0]);
			std::swap(T.p[2], T.p[0]);
		}
		if (s.p[2].ndc.y < s.p[1].ndc.y) {
			std::swap(s.p[2], s.p[1]);
			std::swap(T.p[2], T.p[1]);
		}

		if (BASE::fequal(s.p[1].ndc.y, s.p[2].ndc.y)) fillTriangleFlatTop(s, T, s.p[1].ndc.x, s.p[2].ndc.x, s.p[1].ndc.y, s.p[0].ndc.x, s.p[0].ndc.y, PHONGSHADE);
		else if (BASE::fequal(s.p[0].ndc.y, s.p[1].ndc.y)) fillTriangleFlatBottom(s, T, s.p[0].ndc.x, s.p[1].ndc.x, s.p[0].ndc.y, s.p[2].ndc.x, s.p[2].ndc.y, PHONGSHADE);
	
		else {
			float divx = s.p[0].ndc.x + (s.p[2].ndc.x - s.p[0].ndc.x) * (s.p[1].ndc.y - s.p[0].ndc.y) / (s.p[2].ndc.y - s.p[0].ndc.y);
			fillTriangleFlatBottom(s, T, divx, s.p[1].ndc.x, s.p[1].ndc.y, s.p[2].ndc.x, s.p[2].ndc.y, PHONGSHADE);
			fillTriangleFlatTop(s, T, divx, s.p[1].ndc.x, s.p[1].ndc.y, s.p[0].ndc.x, s.p[0].ndc.y, PHONGSHADE);
		}
	}

	inline void fillTriangle(TriangleF s, Triangle3 T, bool PHONGSHADE = false, bool SCAN = false) {
		if (SCAN) {
			fillTriangleFScan(s, T, PHONGSHADE);
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

		for (int x = BASE::max(x0, 0); x <= x1 && x < W; x++) {
			for (int y = BASE::max(y0, 0); y <= y1 && y < H; y++) {
				if (!s.inside(Vector2(x, y))) continue;
				DrawTriFrag(s, T, x, y, PHONGSHADE);
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
	inline void drawTriangle(Triangle3 s, BaseMaterial* material = nullptr, Vector3* vn = nullptr, Vector2* uv = nullptr, bool BACKFACECULL = false, bool PHONGSHADE = false, bool INTERPNORM = false) {
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

	inline void fillTriangle(Triangle3 s, BaseMaterial* material = nullptr, Vector3* vn = nullptr, Vector2* uv = nullptr, bool BACKFACECULL = true, bool PHONGSHADE = false, bool INTERPNORM = false, bool edgeclip = true) {
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
				fillTriangle(ii, material, i_vn, i_uv, BACKFACECULL, PHONGSHADE, INTERPNORM, false);
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

		auto p = project(s);
		p.material = material;
		if (!uv) uv = new Vector2[3] {Vector2(), Vector2(), Vector2()};
		if (!vn) PHONGSHADE = false;
		if (vn) for (int i = 0; i < 3; i++) p.p[i].normal = INTERPNORM ? vn[i] : s.N;
		if (uv) for (int i = 0; i < 3; i++) p.p[i].uv = uv[i];

		for (int i = 0; i < 3; i++) p.p[i].color = rgb(illuminate(s.p[i], s.p[i], p.p[i].normal, uv[i], p.material));
		
		if (clip(p)) return;
		fillTriangle(p, s, PHONGSHADE);
	}



	// draw a MESH

	inline void drawMesh(Mesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = false) {
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
		}
	}

	inline void drawMesh(MorphedMesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = false) {
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
		}
	}

	inline void fillMesh(Mesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = true) {
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
			fillTriangle(m.makeTriangle(i), material, isna ? nullptr : vn, isnat ? nullptr : vt, BACKFACECULL, PHONGSHADE, INTERPNORM);
			delete[] vn;
			delete[] vt;
		}
	}

	inline void fillMesh(MorphedMesh& m, BaseMaterial* material = nullptr, bool SMOOTHSHADE = false, bool PHONGSHADE = false, bool INTERPNORM = false, bool BACKFACECULL = true) {
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
			fillTriangle(m.makeTriangle(i), material, isna ? nullptr : vn, isnat ? nullptr : vt, BACKFACECULL, PHONGSHADE, INTERPNORM);
			delete[] vn;
			delete[] vt;
		}
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

	inline void outputFrags(std::string OUTPUT__) {
		std::ofstream output(OUTPUT__);
		for (int y = H - 1; y >= 0; y--) {
			for (int x = 0; x < W; x++) {
				output << buffer[x][y].to_string() << ",";
			}
			output << "\n";
		}

		output.close();
	}

	inline void outputBuffer(std::string OUTPUT__) {
		std::ofstream output(OUTPUT__);
		output << "[" << std::to_string(W) << ", " << std::to_string(H) << "]\n";

		for (int y = H - 1; y >= 0; y--) {
			for (int x = 0; x < W; x++) {
				output << std::to_string(buffer[x][y].color) << ",";
			}
			output << "\n";
		}

		output.close();
	}

	inline std::vector<std::vector<uint32_t>> bufferMatrix() {
		std::vector<std::vector<uint32_t>> res(W, std::vector<uint32_t>(H, 0));
		for (int i = 0; i < W; i++) {
			for (int j = 0; j < H; j++) res[i][j] = buffer[i][j].color;
		}
		return res;
	}


	~LegacyScene() {
		for (int i = 0; i < W; i++) delete[] buffer[i];
		delete[] buffer;
	}
};

#endif