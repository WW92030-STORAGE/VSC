#ifndef QUADMESH_EXT
#define QUADMESH_EXT

/*

Meshes are the formation of triangles into manifolds, approximating three-dimensional surfaces. 
All meshes are defined to be "watertight" in the sense that you don't really have 2 different stored vertices in the same position.

Quadmeshes are formed using quadrilaterals. 
They cannot be directly rendered and require triangulating (splitting each quad into two triangles) to create a converted Mesh.

*/

#include <iostream>
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
#include "Mesh.h"

class QuadMesh : public Object {
	public:
	int size = 0;
	int nverts = 0;
	int nuv = 0;

	std::vector<Vector3> verts; // Array of vertices
	std::vector<std::vector<int>> quadindices; // Triangle indices in the vertex array;
	std::vector<std::vector<int>> texcoords; // Texture indices per triangle
	
	std::vector<Vector2> uv; // Texture coordinates

	QuadMesh() { // Default cube

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

	quadindices = std::vector<std::vector<int>>({
		{0, 1, 3, 2}, {0, 1, 5, 4}, {0, 3, 6, 2}, {7, 6, 3, 5}, {7, 5, 1, 3}, {7, 3, 2, 6}
	});

	size = 6;

	uv = std::vector<Vector2>(nverts, Vector2(0, 0));

	nuv = nverts;

	}

	QuadMesh(int sz) {
		size = sz;
		quadindices = std::vector<std::vector<int>>(size, std::vector<int>(4, 0));
	}

	QuadMesh(Vector3* v, int** t, int nv, int sz) {
		nverts = nv;
		size = sz;

		verts = std::vector<Vector3>(nverts);
		quadindices = std::vector<std::vector<int>>(size, std::vector<int>(4, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(4, 0));
		uv = std::vector<Vector2>(nverts);

		nuv = nverts;

		for (int i = 0; i < nverts; i++) {
			verts[i] = Vector3(v[i]);
			uv[i] = Vector2(0, 0);
		}
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 4; j++) quadindices[i][j] = t[i][j];
		} 
		
	}

	QuadMesh(Vector3* v, int** t, Vector2* tex, int** texco, int nv, int sz, int nuuv) {
		nverts = nv;
		size = sz;
		nuv = nuuv;

		verts = std::vector<Vector3>(nverts);
		quadindices = std::vector<std::vector<int>>(size, std::vector<int>(4, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(4, 0));
		uv = std::vector<Vector2>(nuv);

		for (int i = 0; i < nverts; i++) verts[i] = Vector3(v[i]);
		for (int i = 0; i < nuv; i++) uv[i] = Vector2(tex[i]);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 4; j++) {
				quadindices[i][j] = t[i][j];
				texcoords[i][j] = texco[i][j];
			}
		} 
		
	}

	QuadMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t) {
		size = t.size();
		nverts = v.size();

		verts = std::vector<Vector3>(nverts);
		quadindices= std::vector<std::vector<int>>(size, std::vector<int>(4, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(4, 0));

		uv = std::vector<Vector2>(nverts);

		nuv = nverts;

		for (int i = 0; i < nverts; i++) {
			verts[i] = Vector3(v[i]);
			uv[i] = Vector2(0, 0);
		}
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 4 && j < t[i].size(); j++) quadindices[i][j] = t[i][j];
		} 
		
	}

	QuadMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco) {
		size = t.size();
		nverts = v.size();
		nuv = tex.size();

		verts = std::vector<Vector3>(nverts);
		quadindices = std::vector<std::vector<int>>(size, std::vector<int>(4, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(4, 0));

		uv = std::vector<Vector2>(nuv);

		for (int i = 0; i < nverts; i++) verts[i] = Vector3(v[i]);
		for (int i = 0; i < tex.size(); i++) uv[i] = Vector2(tex[i]);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 4 && j < t[i].size(); j++) {
				quadindices[i][j] = t[i][j];
				if (i < texco.size()) texcoords[i][j] = texco[i][j];
			}
		} 
		
	}

	QuadMesh(const QuadMesh& other) : Object(other) {
		size = other.size;
		nverts = other.nverts;
		nuv = other.nuv;
		verts = std::vector<Vector3>(nverts);
		quadindices = std::vector<std::vector<int>>(size);
		texcoords = std::vector<std::vector<int>>(size);
		uv = std::vector<Vector2>(nuv);
		for (int i = 0; i < nverts; i++) verts[i] = Vector3(other.verts[i]);
		for (int i = 0; i < nuv; i++) uv[i] = Vector2(other.uv[i]);
		for (int i = 0; i < size; i++) {
			quadindices[i] = std::vector<int>(other.quadindices[i]);
			texcoords[i] = std::vector<int>(other.texcoords[i]);
		}
	}

	void Trans(Transform t) override {
		transform = t * transform;
		Matrix4 trans = t.matrix();
		for (int i = 0; i < nverts; i++) verts[i] = vec3(trans * fromPoint(verts[i]));
	}

	void ForceTrans(Transform t) override {
		Matrix4 trans = t.matrix();
		for (int i = 0; i < nverts; i++) verts[i] = vec3(trans * fromPoint(verts[i]));
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

	// Flip the normals

	inline void flipNormals() {
		for (int i = 0; i < size; i++) {
			int temp = quadindices[i][0];
			quadindices[i][0] = quadindices[i][3];
			quadindices[i][3] = temp;

			temp = quadindices[i][1];
			quadindices[i][1] = quadindices[i][2];
			quadindices[i][2] = temp;

			temp = texcoords[i][0];
			texcoords[i][0] = texcoords[i][3];
			texcoords[i][3] = temp;
			temp = texcoords[i][1];
			texcoords[i][1] = texcoords[i][2];
			texcoords[i][2] = temp;
		}
	}

	// Centroid of a face

	inline Vector3 centroid(int i) {
		Vector3 res(0, 0, 0);
		for (int s = 0; s < 4; s++) res = res + verts[quadindices[i][s]];
		return (res * 0.25);
	}

	// Get point

	inline Vector3 get(int face, int id) {
		return Vector3(verts[quadindices[face][id]]);
	}

	// CONVERT FROM A QUADMESH TO A MESH

	inline Mesh convert() {
		std::vector<std::vector<int>> triindices;
		std::vector<std::vector<int>> uvs;
		for (int i = 0; i < size; i++) {
			auto q = quadindices[i];
			auto u = texcoords[i];
			// 0-2 diagonal vs. 2-0 diagonal

			bool use02diag = true;

			float d02 = (verts[q[0]] - verts[q[1]]).normsquared();
			float d13 = (verts[q[1]] - verts[q[3]]).normsquared();
			if (d02 > d13) use02diag = false;

			if (use02diag) {
				triindices.push_back(std::vector<int>({q[0], q[1], q[2]}));
				triindices.push_back(std::vector<int>({q[2], q[3], q[0]}));

				uvs.push_back(std::vector<int>({u[0], u[1], u[2]}));
				uvs.push_back(std::vector<int>({u[2], u[3], u[0]}));
			}
			else {
				triindices.push_back(std::vector<int>({q[1], q[2], q[3]}));
				triindices.push_back(std::vector<int>({q[3], q[0], q[1]}));

				uvs.push_back(std::vector<int>({u[1], u[2], u[3]}));
				uvs.push_back(std::vector<int>({u[3], u[0], u[1]}));
			}
		}

		return Mesh(verts, triindices, uv, uvs);
	}

	inline Mesh convertfiner() {
		std::vector<Vector3> trueverts;
		std::map<Vector3, int> trueverts_inv;

		std::vector<Vector2> trueuv;
		std::map<Vector2, int> trueuv_inv;

		std::vector<std::vector<int>> triindices;
		std::vector<std::vector<int>> uvindices;

		for (int face = 0; face < size; face++) {
			auto quad = quadindices[face];
			auto qv = texcoords[face];
			Vector3 quadv[4] = {verts[quad[0]], verts[quad[1]], verts[quad[2]], verts[quad[3]]};
			Vector2 uvs[4] = {uv[qv[0]], uv[qv[1]], uv[qv[2]], uv[qv[3]]};
			Vector3 fifthvert(0, 0, 0);
			Vector2 fifthuv(0, 0);
			for (int i = 0; i < 4; i++) {
				fifthvert = fifthvert + quadv[i];
				fifthuv = fifthuv + uvs[i];
			}

			fifthvert = fifthvert * 0.25;
			fifthuv = fifthuv * 0.25;

			int v5i = pog<Vector3>(trueverts, trueverts_inv, fifthvert);
			int u5i = pog<Vector2>(trueuv, trueuv_inv, fifthuv);

			for (int i = 0; i < 4; i++) {
				int firstvi = pog<Vector3>(trueverts, trueverts_inv, quadv[i]);
				int secondvi = pog<Vector3>(trueverts, trueverts_inv, quadv[(i + 1) % 4]);

				int firstui = pog<Vector2>(trueuv, trueuv_inv, uvs[i]);
				int secondui = pog<Vector2>(trueuv, trueuv_inv, uvs[(i + 1) % 4]);

				triindices.push_back(std::vector<int>({firstvi, secondvi, v5i}));
				uvindices.push_back(std::vector<int>({firstui, secondui, u5i}));
			}
		}

		return Mesh(trueverts, triindices, trueuv, uvindices);
	}


	// The rest of the methods detail reading in from OBJ files.

	static QuadMesh fromOBJ(std::string filename) {
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
			// std::cout << " <" << sp.size() << ">\n";
			
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

		QuadMesh mesh(verts, tris, uv, texco);
		return mesh;
	}
};

#endif