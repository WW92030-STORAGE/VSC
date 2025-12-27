#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <map>

#include "../../include/objects/Triangle.h"
#include "../../include/objects/Object.h"
#include "../../include/utils/math/Matrices.h"
#include "../../include/utils/math/Transform.h"

#include "../../include/utils/stringutils.h"
#include "../../include/utils/LinkedList.h"
#include "../../include/utils/Utils.h"

#include "../../include/objects/Mesh.h"

#include "../../include/objects/MorphedMesh.h"

	// Initialize vertex normals
	void MorphedMesh::setupvns() {

		vn = std::vector<Vector3>(nverts);
		for (int i = 0; i < nverts; i++) vn[i] = Vector3();

		for (int i = 0; i < size; i++) {
			Triangle3 tri = makeTriangle(i);
			for (int j = 0; j < 3; j++) vn[triindices[i][j]] = vn[triindices[i][j]] + tri.N;
		}

		for (int i = 0; i < nverts; i++) vn[i] = vn[i].normalized();
	}

	void MorphedMesh::m_setupvns() {
		mvn = std::vector<std::vector<Vector3>>(nstates, std::vector<Vector3>(nverts));
		for (int index = 0; index < nstates; index++) {
			
			for (int i = 0; i < nverts; i++) mvn[index][i] = Vector3();

			for (int i = 0; i < size; i++) {
				Triangle3 tri = m_makeTriangle(index, i);
				for (int j = 0; j < 3; j++) mvn[index][triindices[i][j]] = mvn[index][triindices[i][j]] + tri.N;
			}

			for (int i = 0; i < nverts; i++) mvn[index][i] = mvn[index][i].normalized();
		}
	}

	// The following three methods copy a given mesh's vertex positions and normals to a state.
	// Precondition: the input mesh is appropriate to copy here, with the same topology of vertices and faces (including triangle corner indices).

	void MorphedMesh::copyTo(int state) {
		if (state < 0) state = nstates;
		nstates = BASE::max(state + 1, nstates);
		while (mvn.size() <= state) mvn.push_back(std::vector<Vector3>(vn));
		while (mverts.size() <= state) mverts.push_back(std::vector<Vector3>(verts));
		while (coeffs.size() <= state) coeffs.push_back(0);

		mvn[state] = std::vector<Vector3>(vn);
		mverts[state] = std::vector<Vector3>(verts);
	}

	void MorphedMesh::copyTo(std::vector<Vector3> ve, std::vector<Vector3> nv, int state) {
		if (state < 0) state = nstates;
		nstates = BASE::max(state + 1, nstates);
		while (mvn.size() <= state) mvn.push_back(std::vector<Vector3>(nv));
		while (mverts.size() <= state) mverts.push_back(std::vector<Vector3>(ve));
		while (coeffs.size() <= state) coeffs.push_back(0);

		mvn[state] = std::vector<Vector3>(nv);
		mverts[state] = std::vector<Vector3>(ve);
	}

	void MorphedMesh::copyTo(Mesh& other, int state) {
		copyTo(other.verts, other.vn, state);
	}

	MorphedMesh::MorphedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t) {
		size = t.size();
		nverts = v.size();
		nuv = nverts;

		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));

		uv = std::vector<Vector2>(nuv);

		for (int i = 0; i < nverts; i++) verts[i] = Vector3(v[i]);
		for (int i = 0; i < nuv; i++) uv[i] = Vector2(0, 0);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3 && j < t[i].size(); j++) triindices[i][j] = t[i][j];
		} 

		setupvns();
		copyTo(0);
		m_setupvns();	

		coeffs = std::vector<float>(1, 1);
	}

	MorphedMesh::MorphedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco) {
		size = t.size();
		nverts = v.size();
		nuv = tex.size();

		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));

		uv = std::vector<Vector2>(nuv);

		for (int i = 0; i < nverts; i++) verts[i] = Vector3(v[i]);
		for (int i = 0; i < nuv; i++) uv[i] = Vector2(tex[i]);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3 && j < t[i].size(); j++) {
				triindices[i][j] = t[i][j];
				if (i < texco.size()) texcoords[i][j] = texco[i][j];
			}
		} 

		setupvns();
		copyTo(0);
		m_setupvns();

		coeffs = std::vector<float>(1, 1);
	}

	MorphedMesh::MorphedMesh(const MorphedMesh& other) : Mesh(other) {
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

		nstates = other.nstates;
		mverts = std::vector<std::vector<Vector3>>(nstates);
		coeffs = std::vector<float>(other.coeffs);

		for (int i = 0; i < nstates; i++) mverts[i] = std::vector<Vector3>(other.mverts[i]);

		setupvns();
		m_setupvns();
	}

	MorphedMesh::MorphedMesh(const Mesh& other) : Mesh(other) {
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
		copyTo(0);
		m_setupvns();
	}

	/*
	
	Morphs the mesh according to the states
	uhhh we're just using barycentric interpolation of the state positions.
	There's also a boolean if you want to barycentric interpolate the vertex normals or if you just want to reset afterwards.
	
	*/

	void MorphedMesh::morph(std::vector<float> coeff, bool interpnorms) {
		coeffs = std::vector<float>(coeff);
		float sum = 0;
		for (auto i : coeff) sum += i;

		if (BASE::fzero(sum)) sum = 1; // no div by zero

		for (int i = 0; i < nverts; i++) {
			verts[i] = Vector3(0, 0, 0);
			for (int j = 0; j < coeff.size() && j < nstates; j++) {
				verts[i] = verts[i] + mverts[j][i] * (coeff[j] / sum);
				if (interpnorms) vn[i] = vn[i] + mvn[j][i] * (coeff[j] / sum);
			}
		}

		if (interpnorms) {
			for (int i = 0; i < nverts; i++) vn[i] = vn[i].normalized();
		} else setupvns();
	}

	void MorphedMesh::morph(float* coeff, int n, bool interpnorms) {
		coeffs = std::vector<float>(coeff, coeff + n);
		float sum = 0;
		for (int i = 0; i < n; i++) sum += coeff[i];

		if (BASE::fzero(sum)) sum = 1; // no div by zero

		for (int i = 0; i < nverts; i++) {
			verts[i] = Vector3(0, 0, 0);
			for (int j = 0; j < n && j < nstates; j++) {
				verts[i] = verts[i] + mverts[j][i] * (coeff[j] / sum);
				if (interpnorms) vn[i] = vn[i] + mvn[j][i] * (coeff[j] / sum);
			}
		}

		if (interpnorms) {
			for (int i = 0; i < nverts; i++) vn[i] = vn[i].normalized();
		} else setupvns();
	}

	void MorphedMesh::morphToState(int state, bool interpnorms) {
		std::vector<float> things(nstates, 0);
		if (state < things.size() && state >= 0) things[state] = 1;
		morph(things, interpnorms);
	}

	void MorphedMesh::Trans(Transform t) {
		transform = t * transform;
		Matrix4 trans = t.matrix();
		for (int i = 0; i < nverts; i++) {
			verts[i] = vec3(trans * fromPoint(verts[i]));
			for (int j = 0; j < nstates; j++) {
				// std::cout << i << " " << j << "\n";
				mverts[j][i] = vec3(trans * fromPoint(mverts[j][i]));
			}
		}
		setupvns();
		m_setupvns();
	}

	void MorphedMesh::ForceTrans(Transform t) {
		Matrix4 trans = t.matrix();
		for (int i = 0; i < nverts; i++) {
			verts[i] = vec3(trans * fromPoint(verts[i]));
			for (int j = 0; j < nstates; j++) {
				mverts[i][j] = vec3(trans * fromPoint(mverts[i][j]));
			}
		}

		setupvns();
		m_setupvns();
	}

	int MorphedMesh::comp (const void * a, const void * b) {
		float x = (*(Triangle3*)a).centroid().z;
		float y = (*(Triangle3*)b).centroid().z;
		if (x < y) return -1;
		if (x == y) return 0;
		if (x > y) return 1;
		return 0;
	}

	// Get Vertex Normal For Vertex
	Vector3 MorphedMesh::getVertexNormal(int i) {
		return vn[i];
	}

	// Get texture coord for triangle index
	Vector2 MorphedMesh::getVertexUV(int i, int s) {
		if (i < 0 || i >= texcoords.size()) {
			// std::cout << "BAD INDEX " << i << " " << texcoords.size() << "\n";
			return  Vector2(0, 0);
		}
		if (s < 0 || s >= texcoords[i].size()) {
			// std::cout << "BAD VERTEX " << s << " " << texcoords[i].size() << "\n";
			return  Vector2(0, 0);
		}
		int sss = texcoords[i][s];
		if (sss < 0 || sss >= uv.size()) {
			// std::cout << "BAD UV POSITION " << sss << " " << uv.size() << "\n";
			return  Vector2(0, 0);
		}
		return Vector2(uv[sss]);
	}

		// get UV of a point in a triangle
	Vector2 MorphedMesh::getUV(Vector3 pos, int ind) {
		Triangle3 tri = makeTriangle(ind);
	
		return tri.interp<Vector2>(pos, getVertexUV(ind, 0), getVertexUV(ind, 1), getVertexUV(ind, 2));
	}

	MorphedMesh::~MorphedMesh() {
	}

	// Get the triangle at index i in triindices

	Triangle3 MorphedMesh::makeTriangle(int i) {
		return Triangle3(verts[triindices[i][0]], verts[triindices[i][1]], verts[triindices[i][2]]);
	}

	Triangle3 MorphedMesh::m_makeTriangle(int index, int i) {
		Triangle3 tri(mverts[index][triindices[i][0]], mverts[index][triindices[i][1]], mverts[index][triindices[i][2]]);
		return tri;
	}

	// Flip the normals

	void MorphedMesh::flipNormals() {
		for (int i = 0; i < size; i++) {
			int temp = triindices[i][0];
			triindices[i][0] = triindices[i][1];
			triindices[i][1] = temp;
		}

		setupvns();
	}


	// The rest of the methods detail reading in from OBJ files.

	// Get vertex given list of vertices and obj form index
	Vector3 MorphedMesh::vindex(std::vector<Vector3>& v, int i) {
		int n = v.size();
		if (i > 0) return Vector3(v[i - 1]);
		return Vector3(v[n - i]);
	}

	std::vector<Triangle3> MorphedMesh::TriangulateConvex(std::vector<int> vertices, std::vector<Vector3> verts) {
		std::vector<Triangle3> tris;
		
		Vector3 v0 = vindex(verts, vertices[0]);
		for (int i = 1; i < vertices.size() - 1; i++) {
			Vector3 v1 = vindex(verts, vertices[i]);
			Vector3 v2 = vindex(verts, vertices[i + 1]);
			tris.push_back(Triangle3(v0, v1, v2));
		}

		return tris;
	}