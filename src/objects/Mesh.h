#ifndef MESH_EXT
#define MESH_EXT

/*

Meshes are the formation of triangles into manifolds, approximating three-dimensional surfaces. 

*/

// #include <iostream>

#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <map>

#include "Triangle.h"
#include "Object.h"
#include "../utils/math/Matrices.h"
#include "../utils/math/Transform.h"

#include "../utils/stringutils.h"
#include "../utils/LinkedList.h"
#include "../utils/Utils.h"

class Mesh : public Object {
	public:
	int size;
	int nverts;

	Triangle3* tris; // WARNING - This is an array of TRIANGLES
	Vector3* verts; // Array of vertices
	int** triindices; // The same array of triangles, except this time in terms of the indices in the verts array.

	Vector3* vn; // Vertex normals. Initialized when we do setupvertsntris() but can also be manually set e.g. when looking thru a .obj file.
	
	// Initialize vertices and triangle index directory from triangles.
	inline void setupvertsntris() {
		std::map<Vector3, int> vm;
		std::vector<Vector3> temp;
		std::vector<Vector3> tn; // temp normals

		triindices = new int*[size];
		for (int i = 0; i < size; i++) {
			int* ti = new int[3];
			for (int s = 0; s < 3; s++) {
				Vector3 pp(tris[i].p[s]); // current vertex
				if (vm.find(pp) == vm.end()) {
					vm.insert({Vector3(pp), temp.size()});
					temp.push_back(Vector3(pp));
					tn.push_back(Vector3());
				}
				// vm.at(pp) is the index of the vertex in the vertices array
				ti[s] = vm.at(pp);
				tn[vm.at(pp)] = tn[vm.at(pp)] + tris[i].N;
			}
			triindices[i] = ti;
		}

		nverts = temp.size();
		verts = new Vector3[nverts];
		vn = new Vector3[nverts];
		for (int i = 0; i < nverts; i++) {
			verts[i] = Vector3(temp[i]);
			vn[i] = tn[i].normalized();
		}

	}

	inline void resetup() {
		delete[] triindices;
		delete[] verts;
		delete[] vn;
		
		setupvertsntris();
	}

	Mesh() { // Default cube

		const int X = 2;
		Vector3 OFFSET = Vector3(-1, -1, 1) * X;
		
	Vector3 FFF(0, 0, 0);
	Vector3 TFF(X, 0, 0);
	Vector3 FTF(0, X, 0);
	Vector3 TTF(X, X, 0);
	Vector3 FFT(0, 0, -X);
	Vector3 TFT(X, 0, -X);
	Vector3 FTT(0, X, -X);
	Vector3 TTT(X, X, -X);

	FFF = FFF + OFFSET;
	TFF = TFF + OFFSET;
	FTF = FTF + OFFSET;
	TTF = TTF + OFFSET;
	FFT = FFT + OFFSET;
	TFT = TFT + OFFSET;
	FTT = FTT + OFFSET;
	TTT = TTT + OFFSET;

	Triangle3* tris = new Triangle3[12] {Triangle3(FFF, TFF, FTF), 
	Triangle3(TFF, TTF, FTF),
	Triangle3(FFF, FTF, FFT),
	Triangle3(FTF, FTT, FFT),
	Triangle3(FFF, FFT, TFF),
	Triangle3(FFT, TFT, TFF),
	Triangle3(TTT, TFT, FTT),
	Triangle3(FTT, TFT, FFT),
	Triangle3(TTT, TTF, TFT),
	Triangle3(TFT, TTF, TFF),
	Triangle3(TTT, FTT, TTF),
	Triangle3(TTF, FTT, FTF)
	};

	size = 12;

	setupvertsntris();

	}

	Mesh(int sz) {
		size = sz;
		tris = new Triangle3[sz];
		for (int i = 0; i < sz; i++) tris[i] = Triangle3();

		setupvertsntris();
	}

	Mesh(Triangle3* tt, int sz) {
		size = sz;
		tris = new Triangle3[sz];
		for (int i = 0; i < sz; i++) tris[i] = Triangle3(tt[i]);

		setupvertsntris();
	}

	Mesh(const Mesh& other) {
		size = other.size;
		tris = new Triangle3[size];
		for (int i = 0; i < size; i++) tris[i] = Triangle3(other.tris[i]);

		setupvertsntris();
	}

	void Trans(Transform t) override {
		transform = t * transform;
		Matrix4 trans = t.matrix();
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3; j++) tris[i].p[j] = vec3(trans * fromPoint(tris[i].p[j]));
			tris[i].N = tris[i].normal();
		}

		resetup();
	}

	void ForceTrans(Transform t) override {
		Matrix4 trans = t.matrix();
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3; j++) tris[i].p[j] = vec3(trans * fromPoint(tris[i].p[j]));
			tris[i].N = tris[i].normal();
		}

		resetup();
	}

	static int comp (const void * a, const void * b) {
		float x = (*(Triangle3*)a).centroid().z;
		float y = (*(Triangle3*)b).centroid().z;
		if (x < y) return -1;
		if (x == y) return 0;
		if (x > y) return 1;
		return 0;
	}

	// Get Vertex Normal For Vertex
	inline Vector3 getVertexNormal(Vector3 v) {
		for (int i = 0; i < nverts; i++) {
			if (v == verts[i]) return Vector3(vn[i]);
		}
		return Vector3();
	}

	~Mesh() {
		delete[] tris;
		for (int i = 0; i < size; i++) delete[] triindices[i];
		delete[] triindices;
		delete[] verts;
		delete[] vn;
	}


	// The rest of the methods detail reading in from OBJ files.

	// Get vertex given list of vertices and obj form index
	inline static Vector3 vindex(std::vector<Vector3>& v, int i) {
		int n = v.size();
		if (i > 0) return Vector3(v[i - 1]);
		return Vector3(v[n - i]);
	}

	inline static std::vector<Triangle3> TriangulateConvex(std::vector<int> vertices, std::vector<Vector3> verts) {
		std::vector<Triangle3> tris;
		
		Vector3 v0 = vindex(verts, vertices[0]);
		for (int i = 1; i < vertices.size() - 1; i++) {
			Vector3 v1 = vindex(verts, vertices[i]);
			Vector3 v2 = vindex(verts, vertices[i + 1]);
			tris.push_back(Triangle3(v0, v1, v2));
		}

		return tris;
	}

	// Algorithm: until there aRe N - 2 triangles: find 3 consecutive verticEs that are counterclockwise and contain no vertices in the interior.
	// The three vertices form a triangle, and the middle vertex is clipped.
	inline static std::vector<Triangle3> Triangulate(std::vector<int> vertices, std::vector<Vector3> verts) {

		std::vector<Triangle3> tris;
		int N = vertices.size();
		if (N < 3) return tris;

		if (N == 3) {
			Vector3 s[3];
			for (int i = 0; i < 3; i++) s[i] = vindex(verts, vertices[i]);
			return std::vector<Triangle3>(1, Triangle3(s[0], s[1], s[2]));
		}

		std::vector<int> v(vertices);

		for (int in = 0; in < N - 2; in++) {
			int n = v.size();
			int i = 0;
			for (i = 0; i < n; i++) {
				int a = vertices[i];
				int b = vertices[(i + 1) % n];
				int c = vertices[(i + 2) % n];
				auto A = vindex(verts, a);
				auto B = vindex(verts, b);
				auto C = vindex(verts, c);
				Triangle3 testtri(A, B, C);
				bool ear = true;
				for (int j = 3; j < n; j++) {
					auto D = vindex(verts, vertices[(i + j) % n]);
					if (testtri.inside(D)) {
						ear = false;
						break;
					}
				}
				if (ear) break;
			}
				int a = vertices[i];
				int b = vertices[(i + 1) % n];
				int c = vertices[(i + 2) % n];
				auto A = vindex(verts, a);
				auto B = vindex(verts, b);
				auto C = vindex(verts, c);
			Triangle3 thetri(A, B, C);
			tris.push_back(thetri);
			vertices.erase(vertices.begin() + i);
		}
		return tris;
	}

	static Mesh fromOBJ(std::string filename) {
		std::ifstream fin(filename);

		std::string line;

		std::vector<Vector3> verts;

		std::vector<Vector3> vn;

		std::vector<Triangle3> tris;
		while (std::getline(fin, line)) {
			// std::cout << ">" << line << "\n";
			if (!line.length()) continue;
			if (line[0] == '#') continue; // comment
			std::vector<std::string> sp = split(line);
			if (!sp.size()) continue;
			// for (auto i : sp) std::cout << "[" << i << "]";
			// std::cout << "\n";
			
			if (sp[0] == "v") {
				// std::cout << "VERTEX\n";
				verts.push_back(Vector3(std::stof(sp[1]), std::stof(sp[2]), std::stof(sp[3])));
			}
			if (sp[0] == "f") {

				std::vector<int> vertices;
				for (int i = 1; i < sp.size(); i++) {
					int index = find(sp[i], '/', 0);
					if (index < 0) vertices.push_back(std::stoi(sp[i]));
					else vertices.push_back(std::stoi(sp[i].substr(0, index)));
				}

				if (!vertices.size()) continue;

				// positive index i represents verts[i - 1]
				// negative index -i represents verts[V - i];

				// Triangulate a polygon

				std::vector<Triangle3> triangulated = Triangulate(vertices, verts);
				for (auto i : triangulated) tris.push_back(i);
			}
			if (sp[0] == "vn") { // Vertex normals
				vn.push_back(Vector3(std::stof(sp[1]), std::stof(sp[2]), std::stof(sp[3])));
			}
			if (sp[0] == "vt") { // TODO later

			}
		}

		Triangle3* triangles = new Triangle3[tris.size()];
		for (int i = 0; i < tris.size(); i++) triangles[i] = tris[i];

		Mesh mesh(triangles, tris.size());

		// mesh.verts = fromvec<Vector3>(verts);
		// mesh.vn = fromvec<Vector3>(vn);
		// mesh.nverts = verts.size();
		return mesh;
	}

	static Mesh fromOBJString(std::string source) {
		std::stringstream fin(source);

		std::string line;

		std::vector<Vector3> verts;

		std::vector<Vector3> vn;

		std::vector<Triangle3> tris;
		while (std::getline(fin, line)) {
			// std::cout << ">" << line << "\n";
			if (!line.length()) continue;
			if (line[0] == '#') continue; // comment
			std::vector<std::string> sp = split(line);
			if (!sp.size()) continue;
			// for (auto i : sp) std::cout << "[" << i << "]";
			// std::cout << "\n";
			
			if (sp[0] == "v") {
				// std::cout << "VERTEX\n";
				verts.push_back(Vector3(std::stof(sp[1]), std::stof(sp[2]), std::stof(sp[3])));
			}
			if (sp[0] == "f") {

				std::vector<int> vertices;
				for (int i = 1; i < sp.size(); i++) {
					int index = find(sp[i], '/', 0);
					if (index < 0) vertices.push_back(std::stoi(sp[i]));
					else vertices.push_back(std::stoi(sp[i].substr(0, index)));
				}

				if (!vertices.size()) continue;

				// positive index i represents verts[i - 1]
				// negative index -i represents verts[V - i];

				// Triangulate a polygon

				std::vector<Triangle3> triangulated = Triangulate(vertices, verts);
				for (auto i : triangulated) tris.push_back(i);
			}
			if (sp[0] == "vn") { // Vertex normals
				vn.push_back(Vector3(std::stof(sp[1]), std::stof(sp[2]), std::stof(sp[3])));
			}
			if (sp[0] == "vt") { // TODO later

			}
		}

		Triangle3* triangles = new Triangle3[tris.size()];
		for (int i = 0; i < tris.size(); i++) triangles[i] = tris[i];

		Mesh mesh(triangles, tris.size());

		// mesh.verts = fromvec<Vector3>(verts);
		// mesh.vn = fromvec<Vector3>(vn);
		// mesh.nverts = verts.size();
		return mesh;
	}
};

#endif