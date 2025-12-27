#ifndef QUADMESH_EXT
#define QUADMESH_EXT

/*

Meshes are the formation of triangles into manifolds, approximating three-dimensional surfaces. 
All meshes are defined to be "watertight" in the sense that you don't really have 2 different stored vertices in the same position.

Quadmeshes are formed using quadrilaterals. 
They cannot be directly rendered and require triangulating (splitting each quad into two triangles) to create a converted Mesh.

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

	QuadMesh();

	QuadMesh(int sz);

	QuadMesh(Vector3* v, int** t, int nv, int sz);

	QuadMesh(Vector3* v, int** t, Vector2* tex, int** texco, int nv, int sz, int nuuv);

	QuadMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t);

	QuadMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco);

	QuadMesh(const QuadMesh& other);

	void Trans(Transform t) override;

	void ForceTrans(Transform t) override;

	// Get texture coord for triangle index
	Vector2 getVertexUV(int i, int s);

	// Flip the normals

	void flipNormals();

	// Centroid of a face

	Vector3 centroid(int i);

	// Get point

	Vector3 get(int face, int id);

	// CONVERT FROM A QUADMESH TO A MESH

	Mesh convert();

	Mesh convertfiner();


	// The rest of the methods detail reading in from OBJ files.

	static QuadMesh fromOBJ(std::string filename);
};

class SharpQuadMesh : public QuadMesh {
	public:
	std::set<Vector3> shverts;
	std::set<std::pair<Vector3, Vector3>> shedges;

	bool isSharp(Vector3 v);

	bool isSharp(std::pair<Vector3, Vector3> v);

	
	SharpQuadMesh(int sz);

	SharpQuadMesh(Vector3* v, int** t, int nv, int sz) ;

	SharpQuadMesh(Vector3* v, int** t, Vector2* tex, int** texco, int nv, int sz, int nuuv);

	SharpQuadMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t);

	SharpQuadMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco) ;

	SharpQuadMesh(const SharpQuadMesh& other);

	SharpQuadMesh(const QuadMesh& other);

	void Trans(Transform t) override;

	void ForceTrans(Transform t) override;

};

#endif