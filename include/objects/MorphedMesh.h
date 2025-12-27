#ifndef MORPHEDMESH_EXT
#define MORPHEDMESH_EXT


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

class MorphedMesh : public Mesh {
	public:
	int size = 0;
	int nverts = 0;
	int nuv = 0;
	int nstates = 0;

	// Additional information on the morphs.

	std::vector<std::vector<Vector3>> mverts;
	std::vector<std::vector<Vector3>> mvn;

	std::vector<float> coeffs;

	// Other stuff from base Mesh

	/*

	std::vector<Vector3> verts; // Array of vertices
	std::vector<std::vector<int>> triindices; // Triangle indices in the vertex array;
	std::vector<std::vector<int>> texcoords; // Texture indices per triangle

	std::vector<Vector3> vn; // Vertex normals. Initialized when we do setupvertsntris() but can also be manually set e.g. when looking thru a .obj file.
	std::vector<Vector2> uv; // Texture coordinates

	*/

	// Initialize vertex normals
	void setupvns();

	void m_setupvns();

	// The following three methods copy a given mesh's vertex positions and normals to a state.
	// Precondition: the input mesh is appropriate to copy here, with the same topology of vertices and faces (including triangle corner indices).

	void copyTo(int state = -1);

	void copyTo(std::vector<Vector3> ve, std::vector<Vector3> nv, int state = -1);

	void copyTo(Mesh& other, int state = -1);

	MorphedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t);

	MorphedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco);

	MorphedMesh(const MorphedMesh& other);

	MorphedMesh(const Mesh& other);

	/*
	
	Morphs the mesh according to the states
	uhhh we're just using barycentric interpolation of the state positions.
	There's also a boolean if you want to barycentric interpolate the vertex normals or if you just want to reset afterwards.
	
	*/

	void morph(std::vector<float> coeff, bool interpnorms = true);

	void morph(float* coeff, int n, bool interpnorms = true) ;

	void morphToState(int state, bool interpnorms = true);

	void Trans(Transform t) override;

	void ForceTrans(Transform t) override ;

	static int comp (const void * a, const void * b);

	// Get Vertex Normal For Vertex
	Vector3 getVertexNormal(int i);

	// Get texture coord for triangle index
	Vector2 getVertexUV(int i, int s) ;

		// get UV of a point in a triangle
	Vector2 getUV(Vector3 pos, int ind);

	~MorphedMesh() ;

	// Get the triangle at index i in triindices

	Triangle3 makeTriangle(int i);
	Triangle3 m_makeTriangle(int index, int i) ;

	// Flip the normals

	void flipNormals() ;


	// The rest of the methods detail reading in from OBJ files.

	// Get vertex given list of vertices and obj form index
	static Vector3 vindex(std::vector<Vector3>& v, int i);

	static std::vector<Triangle3> TriangulateConvex(std::vector<int> vertices, std::vector<Vector3> verts);
};

#endif