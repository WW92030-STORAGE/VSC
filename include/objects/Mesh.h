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
	void setupvns();

	Mesh();

	Mesh(int sz);

	Mesh(Vector3* v, int** t, int nv, int sz);

	Mesh(Vector3* v, int** t, Vector2* tex, int** texco, int nv, int sz, int nuuv);

	Mesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t);

	Mesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco);

	Mesh(Triangle3* tt, int sz);

	Mesh(std::vector<Triangle3> tt);

	Mesh(const Mesh& other);

	void Trans(Transform t) override;

	void ForceTrans(Transform t) override;

	static int comp (const void * a, const void * b);

	// Get Vertex Normal For Vertex
	Vector3 getVertexNormal(int i);

	Vector3 getVertexNormal(int ind, int i);

	// Get normal for point in triangle, interpolated
	Vector3 getInterpolatedNormal(Vector3 pos, int ind);
	Vector3 getInterpolatedNormal_bary(Vector3 bary, int ind);
	Vector3 getInterpolatedNormal_bary(Vector3 bary, int ind, Triangle3& tri);

	// Get texture coord for triangle index
	Vector2 getVertexUV(int i, int s);
	// get UV of a point in a triangle
	Vector2 getUV(Vector3 pos, int ind);
	Vector2 getUV_bary(Vector3 bary, int ind);
	Vector2 getUV_bary(Vector3 bary, int ind, Triangle3& tri);


	~Mesh();

	// Get the triangle at index i in triindices

	Triangle3 makeTriangle(int i);

	// Flip the normals

	void flipNormals();


	// The rest of the methods detail reading in from OBJ files.


	static Mesh fromOBJ(std::string filename);
};

#endif