#ifndef MESH_EXT
#define MESH_EXT

/*

Meshes are the formation of triangles into manifolds, approximating three-dimensional surfaces. 
All meshes are defined to be "watertight" in the sense that you don't really have 2 different stored vertices in the same position.

*/

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
	int size = 0;
	int nverts = 0;
	int nuv = 0;

	std::vector<Vector3> verts; // Array of vertices
	std::vector<std::vector<int>> triindices; // Triangle indices in the vertex array;
	std::vector<std::vector<int>> texcoords; // Texture indices per triangle

	std::vector<Vector3> vn; // Vertex normals. Initialized when we do setupvertsntris() but can also be manually set e.g. when looking thru a .obj file.
	std::vector<Vector2> uv; // Texture coordinates

	// Initialize vertex normals
	void setupvns() {

		vn = std::vector<Vector3>(nverts);

		for (int i = 0; i < size; i++) {
			Triangle3 tri = makeTriangle(i);
			for (int j = 0; j < 3; j++) vn[triindices[i][j]] = vn[triindices[i][j]] + tri.N;
		}

		for (int i = 0; i < nverts; i++) vn[i] = vn[i].normalized();
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

	verts = std::vector<Vector3>({FFF, TFF, FTF, TTF, FFT, TFT, FTT, TTT});
	for (int i = 0; i < 8; i++) verts[i] = Vector3(verts[i]);

	nverts = 8;

	/*

	tris = std::vector<Triangle3>({Triangle3(FFF, TFF, FTF), 
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
	});

	*/

	triindices = std::vector<std::vector<int>>({
		{0, 1, 2}, {1, 3, 2}, {0, 2, 4}, {2, 6, 4}, {0, 4, 1}, {4, 5, 1},
		{7, 5, 6}, {6, 5, 4}, {7, 3, 5}, {5, 3, 1}, {7, 6, 3}, {3, 6, 2}
	});

	size = 12;

	uv = std::vector<Vector2>(nverts, Vector2(0, 0));

	nuv = nverts;

	setupvns();

	}

	Mesh(int sz) {
		size = sz;
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
	}

	Mesh(Vector3* v, int** t, int nv, int sz) {
		nverts = nv;
		size = sz;

		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		uv = std::vector<Vector2>(nverts);

		nuv = nverts;

		for (int i = 0; i < nverts; i++) {
			verts[i] = Vector3(v[i]);
			uv[i] = Vector2(0, 0);
		}
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3; j++) triindices[i][j] = t[i][j];
		} 

		setupvns();
		
	}

	Mesh(Vector3* v, int** t, Vector2* tex, int** texco, int nv, int sz, int nuuv) {
		nverts = nv;
		size = sz;
		nuv = nuuv;

		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		uv = std::vector<Vector2>(nuv);

		for (int i = 0; i < nverts; i++) verts[i] = Vector3(v[i]);
		for (int i = 0; i < nuv; i++) uv[i] = Vector2(tex[i]);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3; j++) {
				triindices[i][j] = t[i][j];
				texcoords[i][j] = texco[i][j];
			}
		} 

		setupvns();
		
	}

	Mesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t) {
		size = t.size();
		nverts = v.size();

		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));

		uv = std::vector<Vector2>(nverts);

		nuv = nverts;

		for (int i = 0; i < nverts; i++) {
			verts[i] = Vector3(v[i]);
			uv[i] = Vector2(0, 0);
		}
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3 && j < t[i].size(); j++) triindices[i][j] = t[i][j];
		} 

		setupvns();
		
	}

	Mesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco) {
		size = t.size();
		nverts = v.size();
		nuv = tex.size();

		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));

		uv = std::vector<Vector2>(nuv);

		for (int i = 0; i < nverts; i++) verts[i] = Vector3(v[i]);
		for (int i = 0; i < tex.size(); i++) uv[i] = Vector2(tex[i]);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3 && j < t[i].size(); j++) {
				triindices[i][j] = t[i][j];
				if (i < texco.size()) texcoords[i][j] = texco[i][j];
			}
		} 

		setupvns();
		
	}

	Mesh(Triangle3* tt, int sz) {
		size = sz;
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3));
		std::map<Vector3, int> indices;
		for (int i = 0; i < sz; i++) {
			for (int j = 0; j < 3; j++) {
				Vector3 v = Vector3(tt[i].p[j]);
				if (indices.find(v) == indices.end()) {
					indices.insert({v, verts.size()});
					verts.push_back(v);
				}
				triindices[i][j] = indices.at(v);
			}
		}

		nverts = verts.size();
		nuv = nverts;

		uv = std::vector<Vector2>(nuv, Vector2(0, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));


		setupvns();
	}

	Mesh(std::vector<Triangle3> tt) {
		int sz = tt.size();
		size = sz;
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3));
		std::map<Vector3, int> indices;
		for (int i = 0; i < sz; i++) {
			for (int j = 0; j < 3; j++) {
				Vector3 v = Vector3(tt[i].p[j]);
				if (indices.find(v) == indices.end()) {
					indices.insert({v, verts.size()});
					verts.push_back(v);
				}
				triindices[i][j] = indices.at(v);
			}
		}

		nverts = verts.size();
		nuv = nverts;

		uv = std::vector<Vector2>(nuv, Vector2(0, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));


		setupvns();
	}

	Mesh(const Mesh& other) : Object(other) {
		size = other.size;
		nverts = other.nverts;
		nuv = other.nuv;
		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size);
		texcoords = std::vector<std::vector<int>>(size);
		uv = std::vector<Vector2>(nuv);
		for (int i = 0; i < nverts; i++) verts[i] = Vector3(other.verts[i]);
		for (int i = 0; i < nuv; i++) uv[i] = Vector2(other.uv[i]);
		for (int i = 0; i < size; i++) {
			triindices[i] = std::vector<int>(other.triindices[i]);
			texcoords[i] = std::vector<int>(other.texcoords[i]);
		}

		setupvns();
	}

	void Trans(Transform t) override {
		transform = t * transform;
		Matrix4 trans = t.matrix();
		for (int i = 0; i < nverts; i++) verts[i] = vec3(trans * fromPoint(verts[i]));
		setupvns();
	}

	void ForceTrans(Transform t) override {
		Matrix4 trans = t.matrix();
		for (int i = 0; i < nverts; i++) verts[i] = vec3(trans * fromPoint(verts[i]));

		setupvns();
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
	inline Vector3 getVertexNormal(int i) {
		return vn[i];
	}

	inline Vector3 getVertexNormal(int ind, int i) {
		return getVertexNormal(triindices[ind][i]);
	}

	// Get normal for point in triangle, interpolated
	inline Vector3 getInterpolatedNormal(Vector3 pos, int ind) {
		Triangle3 tri = makeTriangle(ind);

		return (tri.interp<Vector3>(pos, getVertexNormal(ind, 0), getVertexNormal(ind, 1), getVertexNormal(ind, 2))).normalized();
	}

	// Get texture coord for triangle index
	inline Vector2 getVertexUV(int i, int s) {
		if (i < 0 || i >= texcoords.size()) {
			// std::cout << "BAD INDEX " << i << " " << nuv << "\n";
			return Vector2(0, 0);
		}
		if (s < 0 || s >= texcoords[i].size()) {
			// std::cout << "BAD VERTEX " << s << " " << texcoords[i].size() << "\n";
			return Vector2(0, 0);
		}
		int sss = texcoords[i][s];
		if (sss < 0 || sss >= uv.size()) {
			// std::cout << "BAD UV POSITION " << sss << " " << uv.size() << "\n";
			return Vector2(0, 0);
		}
		
		return Vector2(uv[sss]);
	}

	// get UV of a point in a triangle
	inline Vector2 getUV(Vector3 pos, int ind) {
		Triangle3 tri = makeTriangle(ind);

		return tri.interp<Vector2>(pos, getVertexUV(ind, 0), getVertexUV(ind, 1), getVertexUV(ind, 2));
	}



	~Mesh() {
	}

	// Get the triangle at index i in triindices

	inline Triangle3 makeTriangle(int i) {
		return Triangle3(verts[triindices[i][0]], verts[triindices[i][1]], verts[triindices[i][2]]);
	}

	// Flip the normals

	inline void flipNormals() {
		for (int i = 0; i < size; i++) {
			int temp = triindices[i][0];
			triindices[i][0] = triindices[i][1];
			triindices[i][1] = temp;

			temp = texcoords[i][0];
			texcoords[i][0] = texcoords[i][1];
			texcoords[i][1] = temp;
		}

		setupvns();
	}


	// The rest of the methods detail reading in from OBJ files.


	static Mesh fromOBJ(std::string filename) {
		std::ifstream fin(filename);

		std::string line;

		std::vector<Vector3> verts;
		std::vector<Vector3> vn;
		std::vector<std::vector<int>> tris;
		std::vector<Vector2> uv;
		std::vector<std::vector<int>> texco;


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
				std::vector<int> uvs;
				for (int i = 1; i < sp.size(); i++) {
					int index = find(sp[i], '/', 0);
					if (index < 0) {
						vertices.push_back(std::stoi(sp[i]));
						uvs.push_back(0);
					}
					else {
						vertices.push_back(std::stoi(sp[i].substr(0, index)));
						int texindex = find(sp[i], '/', index + 1);
						if (texindex < 0 || texindex - 1 <= index) {
							uvs.push_back(0);
						}
						else {
							int thing = std::stoi(substr(sp[i], index + 1, texindex));
							uvs.push_back(thing);
						}
					}
				}

				if (!vertices.size()) continue;

				// positive index i represents verts[i - 1]
				// negative index -i represents verts[V - i];

				int n = verts.size();
				for (int i = 0; i < vertices.size(); i++) {
					if (vertices[i] > 0) vertices[i]--;
					else vertices[i] = n - vertices[i];
				}
				tris.push_back(vertices);

				for (int i = 0; i < uvs.size(); i++) {
					if (uvs[i] > 0) uvs[i]--;
					else uvs[i] = uv.size() - uvs[i];
				}

				// std::cout << "TRIANGLE ";
				// for (int i = 0; i < uvs.size(); i++) std::cout << uv[uvs[i]].to_string() << " ";
				// std::cout << "\n";
				texco.push_back(uvs);

			}
			if (sp[0] == "vn") { // Vertex normals
				vn.push_back(Vector3(std::stof(sp[1]), std::stof(sp[2]), std::stof(sp[3])));
			}
			if (sp[0] == "vt") { // TODO later
				uv.push_back(Vector2(std::stof(sp[1]), std::stof(sp[2])));
			}
		}

		/*

		for (auto i : uv) {
			std::cout << i.to_string() << " . ";
		}
		std::cout << "\n";
		for (auto i : texco) {
			std::cout << "[ ";
			for (auto j : i) std::cout << j << " ";
			std::cout << "]";
		}
		std::cout << "\n";

		*/

		// std::cout << "MESH " << verts.size() << " "  << tris.size() << " " <<  uv.size() << " " << texco.size() << "\n";

		Mesh mesh(verts, tris, uv, texco);
		return mesh;
	}
};

#endif