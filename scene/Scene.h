#ifndef SCENE_EXT
#define SCENE_EXT

// #include <iostream>
#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <cfloat>
#include <climits>

#include "../objects/Mesh.h"
#include "../objects/Triangle.h"
#include "../objects/Object.h"

#include "../camera/Camera.h"
#include "../utils/geometry/Fragment.h"

#include "Color.h"


class Scene { // CENA!
	public:
	Object** objects;
	int numObjs;

	int W, H;
	int SIDE;

	std::priority_queue<std::pair<TriangleF, std::pair<uint32_t, bool>>> triqueue;

	Camera camera;

	Fragment** buffer; // Coordinates are (x, y) where x goes right and y up. This is to align with the projection matrix.

	DirectionalLight light;

	inline void initBuffer() {
		SIDE = H > W ? H : W;
		buffer = new Fragment*[W];
		for (int i = 0; i < W; i++) buffer[i] = new Fragment[H];
		for (int i = 0; i < W; i++) {
			for (int j =- 0; j < H; j++) buffer[i][j] = Fragment();
		}
	}

	// Sorry but you're only getting these 2 constructors. Do the rest of it yourself.
	Scene() {
		camera = Camera();
		objects = nullptr;
		numObjs = 0;

		W = 256;
		H = 256;

		initBuffer();
	}

	Scene(int w, int h) {
		camera = Camera();
		objects = nullptr;
		numObjs = 0;

		W = w;
		H = h;

		initBuffer();
	}

	inline void clearBuffer() {
		fillScreen(0x000000FF);
	}

	inline void fillScreen(int32_t c) {
		for (int i = 0; i < W; i++) {
			for (int j = 0; j < H; j++) {
				buffer[i][j] = Fragment();
				buffer[i][j].color = 0;
			}
		}
	}

	// Draw 2d primitives

	inline void drawPixel(int x, int y, uint32_t z) {
		if (x < 0 || y < 0 || x >= W || y >= H) return;
		buffer[x][y] = Fragment(buffer[x][y]);
		buffer[x][y].color = z;
	}

	inline void drawFragment(Fragment& F, int x, int y) {
		if (x < 0 || y < 0 || x >= W || y >= H) return;
		if (F.ndc.z < -2 || F.ndc.z > 2) return;
		if (buffer[x][y].ndc.w < F.ndc.w) return;
		buffer[x][y] = Fragment(F);
	}

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
			for (int y = min(y0, y1); y <= max(y0, y1); y++) drawPixel(x0, y, c);
			return;
		}
		else if (y0 == y1) {
			for (int x = min(x0, x1); x <= max(x0, x1); x++) drawPixel(x, y0, c);
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
		return (s.N * (s.p[0] - camera.transform.origin)) > 0;
	}

	// The updated version takes a Triangle3 and returns a triangle of fragments.
	// The fragments for now only have normalized device coordinates, the original triangle normal, and teh color.
	// The triangle is returned with the coordinates as they are represented in NDC. However
	// However the x and y coordinates are transformed to fit in the buffer.
	inline TriangleF project(Triangle3 s, uint32_t c = 0xFFFFFFFF) {
		
		Vector4 res[3];

		for (int i = 0; i < 3; i++) {
			Vector4 v = vec4(s.p[i]);
			v.w = 1;
			Vector4 vv = camera.glFrustum(v);
			res[i] = vv;

			res[i].x = (res[i].x + 1) * 0.5 * SIDE;
			res[i].y = (res[i].y + 1) * 0.5 * SIDE;
		}

		TriangleF tri;

		tri.ON = Vector3(s.normal());

		for (int i = 0; i < 3; i++) {
			tri.p[i] = Fragment(res[i], s.N, c);
		}

		return tri;
	}

	// Draw fragment triangles. We will expand this in the future.
	inline void drawTriangle(TriangleF s, uint32_t c) {
		Triangle2 t(vec2(s.p[0].ndc), vec2(s.p[1].ndc), vec2(s.p[2].ndc));
		drawTriangle(t, c);
	}

	inline void DrawTriFrag(TriangleF s, int x, int y, uint32_t c) {
		float zc = s.interp(x, y, s.p[0].ndc.z, s.p[1].ndc.z, s.p[1].ndc.z);
		float wc = 1.0 / s.interp(x, y, 1.0 / s.p[0].ndc.w, 1.0 / s.p[1].ndc.w, 1.0 / s.p[1].ndc.w);
		Fragment F(Vector4(x, y, zc, wc), Vector3(s.ON), c);

		// std::cout << s.bary(x, y).to_string() << " " << F.ndc.z << " " << F.ndc.w << " = " << F.color << "\n";
		drawFragment(F, x, y);
	}

	// Originally a method using three cross products, now using a variation on Bresenham's method.
	// https://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html

	// Fill a triangle with a flat bottom: draw the lines (t, b1) and (t, b2) and scan along the way
	// s is the ORIGINAL triangle we reference.
	inline void fillBotFlat(TriangleF s, int bx1, int bx2, int by, int tx, int ty, uint32_t c) {
		if (bx1 > bx2) std::swap(bx1, bx2);
		int dx1 = tx - bx1;
		int dx2 = tx - bx2;
		int dy = ty - by; // always positive

		int D1 = 2 * dy - abs(dx1);
		int D2 = 2 * dy - abs(dx2);

		if (abs(dx1) < dy) D1 = 2 * abs(dx1) - dy;
		if (abs(dx2) < dy) D2 = 2 * abs(dx2) - dy;

		int x1 = bx1;
		int y1 = by;
		int y2 = by;
		int x2 = bx2;

		int Rx1 = (dx1 > 0) ? 1 : -1;
		int Rx2 = (dx2 > 0) ? 1 : -1;
		int Ry = 1;

		while (y1 < ty) {
			// FIRST LINE
			while (true) {
				// Line is shallow
				bool f = false;
				if (abs(dx1) >= dy) {
					DrawTriFrag(s, x1, y1, c);
					if (D1 > 0) {
						y1 = y1 + 1;
						D1 += 2 * (dy - abs(dx1));
						f = true;
					}
					else D1 += 2 * dy;
					x1 += Rx1;
					if (f) break;
				}
				// Line is steep
				else {
					DrawTriFrag(s, x1, y1, c);
					if (D1 > 0) {
						x1 += Rx1;
						D1 += 2 * (abs(dx1) - dy);
					}
					else D1 += 2 * abs(dx1);
					y1 += Ry;
					break;
				}
			}
			// SECOND LINE
			while (true) {
				bool f = false;
				if (abs(dx2) >= dy) {
					DrawTriFrag(s, x2, y2, c);
					if (D2 > 0) {
						y2 = y2 + 1;
						D2 += 2 * (dy - abs(dx2));
						f = true;
					}
					else D2 += 2 * dy;
					x2 += Rx2;
					if (f) break;
				}
				else {
					DrawTriFrag(s, x2, y2, c);
					if (D2 > 0) {
						x2 += Rx2;
						D2 += 2 * (abs(dx2) - dy);
					}
					else D2 += 2 * abs(dx2);
					y2 += Ry;
					break;
				}
			}

			for (int xx = x1; xx <= x2; xx++) DrawTriFrag(s, xx, y1, c);
		}
	}

	inline void fillTopFlat(TriangleF s, int bx1, int bx2, int by, int tx, int ty, uint32_t c) {
		if (bx1 > bx2) std::swap(bx1, bx2);
		int dx1 = bx1 - tx;
		int dx2 = bx2 - tx;
		int dy = by - ty;

		int D1 = 2 * dy - dx1;
		int D2 = 2 * dy - dx2;

		if (abs(dx1) < dy) D1 = 2 * dx1 - dy;
		if (abs(dx2) < dy) D2 = 2 * dx2 - dy;

		int x1 = tx;
		int y1 = ty;
		int y2 = ty;
		int x2 = tx;

		int Rx1 = (dx1 > 0) ? 1 : -1;
		int Rx2 = (dx2 > 0) ? 1 : -1;
		int Ry = 1;

		while (y1 < by) {
			// FIRST LINE
			while (true) {
				// Line is shallow
				bool f = false;
				if (abs(dx1) >= dy) {
					DrawTriFrag(s, x1, y1, c);
					if (D1 > 0) {
						y1 = y1 + Ry;
						D1 += 2 * (dy - abs(dx1));
						f = true;
					}
					else D1 += 2 * dy;
					x1 += Rx1;
					if (f) break;
				}
				// Line is steep
				else {
					DrawTriFrag(s, x1, y1, c);
					if (D1 > 0) {
						x1 += Rx1;
						D1 += 2 * (abs(dx1) - dy);
					}
					else D1 += 2 * abs(dx1);
					y1 += Ry;
					break;
				}
			}
			// SECOND LINE
			while (true) {
				bool f = false;
				if (abs(dx2) >= dy) {
					DrawTriFrag(s, x2, y2, c);
					if (D2 > 0) {
						y2 = y2 + Ry;
						D2 += 2 * (dy - abs(dx2));
						f = true;
					}
					else D2 += 2 * dy;
					x2 += Rx2;
					if (f) break;
				}
				else {
					DrawTriFrag(s, x2, y2, c);
					if (D2 > 0) {
						x2 += Rx2;
						D2 += 2 * (abs(dx2) - dy);
					}
					else D2 += 2 * abs(dx2);
					y2 += Ry;
					break;
				}
			}

			for (int xx = x1; xx <= x2; xx++) DrawTriFrag(s, xx, y1, c);
		}
	}

	inline void fillTriangleFast(TriangleF& t, uint32_t c) {
		// drawTriangle(t, c);
		TriangleF s(t);
		if (s.p[1].ndc.y < s.p[0].ndc.y) std::swap(s.p[0], s.p[1]);
		if (s.p[2].ndc.y < s.p[0].ndc.y) std::swap(s.p[2], s.p[0]);
		if (s.p[2].ndc.y < s.p[1].ndc.y) std::swap(s.p[2], s.p[1]);

		if (fequal(s.p[1].ndc.y, s.p[2].ndc.y)) fillTopFlat(s, iround(s.p[1].ndc.x), iround(s.p[2].ndc.x), iround(s.p[1].ndc.y), iround(s.p[0].ndc.x), iround(s.p[0].ndc.y), c);
		else if (fequal(s.p[0].ndc.y, s.p[1].ndc.y)) fillBotFlat(s, iround(s.p[0].ndc.x), iround(s.p[1].ndc.x), iround(s.p[0].ndc.y), iround(s.p[2].ndc.x), iround(s.p[2].ndc.y), c);
	
		else {
			float divx = s.p[0].ndc.x + (s.p[2].ndc.x - s.p[0].ndc.x) * (s.p[1].ndc.y - s.p[0].ndc.y) / (s.p[2].ndc.y - s.p[0].ndc.y);
			fillBotFlat(s, divx, s.p[1].ndc.x, s.p[1].ndc.y, s.p[2].ndc.x, s.p[2].ndc.y, c);
			fillTopFlat(s, divx, s.p[1].ndc.x, s.p[1].ndc.y, s.p[0].ndc.x, s.p[0].ndc.y, c);
		}
	}

	inline void fillTriangle(TriangleF s, uint32_t c, bool OPTIM = true) {
		if (OPTIM) {
			fillTriangleFast(s, c);
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

		for (int x = x0; x <= x1; x++) {
			for (int y = y0; y <= y1; y++) {
				if (!s.inside(Vector2(x, y))) continue;
				DrawTriFrag(s, x, y, c);
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

	inline void drawTriangle(Triangle3 s, uint32_t c, bool BACKFACECULL = false, bool LIGHT = false) {
		if (BACKFACECULL && BackFaceCull(s)) return;
		if (LIGHT) c = colmul(c, -(s.N * light.dir));
		auto p = project(s);
		if (clip(p)) return;
		drawTriangle(p, c);
	}

	inline void fillTriangle(Triangle3 s, uint32_t c, bool BACKFACECULL = true, bool LIGHT = true) {
		if (BACKFACECULL && BackFaceCull(s)) {
			// std::cout << "CULLED\n";
			return;
		}
		if (LIGHT) c = colmul(c, -(s.N * light.dir));
		auto p = project(s);
		if (clip(p)) return;
		fillTriangle(p, c);
	}



	// draw a MESH

	inline void drawMesh(Mesh& m, uint32_t c, bool BACKFACECULL = false, bool LIGHT = false) {
		for (int i = 0; i < m.size; i++) {
			drawTriangle(m.tris[i], c, BACKFACECULL, LIGHT);
		}
	}

	inline void fillMesh(Mesh& m, uint32_t c, bool BACKFACECULL = true, bool LIGHT = true) {
		for (int i = 0; i < m.size; i++) {
			// if (i % 64 == 0) std::cout << i << "/" << m.size << "...\n";
			fillTriangle(m.tris[i], c, BACKFACECULL, LIGHT);
		}
	}

	// Queue various meshes and triangles to be drawn

	inline void QueueTriangle(Triangle3 s, uint32_t c, bool FILL = false, bool BACKFACECULL = true, bool LIGHT = true) {
		if (BACKFACECULL && BackFaceCull(s)) return;
		if (LIGHT) c = colmul(c, -(s.N * light.dir));
		triqueue.push({project(s), {c, FILL}});
	}

	inline void QueueMesh(Mesh& m, uint32_t c, bool FILL = true) {
		for (int i = 0; i < m.size; i++) QueueTriangle(m.tris[i], c, FILL);
	}

	inline void drawQueue() {
		while (triqueue.size()) {
			auto p = triqueue.top();
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

	inline void outputFrags(std::string OUTPUT) {
		std::ofstream output(OUTPUT);
		for (int y = H - 1; y >= 0; y--) {
			for (int x = 0; x < W; x++) {
				output << buffer[x][y].to_string() << ",";
			}
			output << "\n";
		}

		output.close();
	}

	inline void outputBuffer(std::string OUTPUT) {
		std::ofstream output(OUTPUT);
		output << "[" << std::to_string(W) << ", " << std::to_string(H) << "]\n";

		for (int y = H - 1; y >= 0; y--) {
			for (int x = 0; x < W; x++) {
				output << std::to_string(buffer[x][y].color) << ",";
			}
			output << "\n";
		}

		output.close();
	}


	~Scene() {
		delete[] objects;
		for (int i = 0; i < W; i++) delete[] buffer[i];
		delete[] buffer;
	}
};

#endif