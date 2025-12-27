#ifndef RIGGEDMESH_EXT
#define RIGGEDMESH_EXT

#include <string>
#include <vector>
#include <utility>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "Triangle.h"
#include "Object.h"
#include "../utils/math/Matrices.h"
#include "../utils/math/Transform.h"

#include "../utils/stringutils.h"
#include "../utils/LinkedList.h"
#include "../utils/Utils.h"

#include "../utils/geometry/Shapes.h"



#include "Mesh.h"

class RiggedMesh : public Mesh {
	public:
	int size = 0;
	int nverts = 0;
	int nuv = 0;
	int nbones = 0;

    std::vector<std::string> bone_names;
    std::unordered_map<std::string, int> bone_names_inv;

    std::vector<int> parent;
    std::vector<std::vector<int>> children;
    std::vector<Transform> rest_transforms;
    std::vector<Vector3> tips;
    std::vector<Transform> deformations;

    std::vector<Transform> absolute_transforms;
    std::vector<Vector3> absolute_joints;
    std::vector<Vector3> absolute_tips;
    std::unordered_set<int> a_tips;

    std::vector<std::unordered_map<int, float>> vertex_weights;
    std::vector<std::unordered_map<int, Vector3>> vertex_displacements;

    std::vector<Vector3> original_verts;

    RiggedMesh(const RiggedMesh& other) ;

    void init();

    void Trans(Transform t) override;

    void ForceTrans(Transform t) override;

    RiggedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t);

    RiggedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco);

    RiggedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco, 
    std::vector<std::string>& names, std::vector<int>& par, std::vector<Matrix4>& transforms, std::vector<Vector3>& tipp, std::unordered_set<int>& atips, std::vector<std::unordered_map<int, float>> weights);

    bool is_root(int i);


    void computeAbsoluteTransforms();

    // QuOTD - I have a baby brother... (wow!) Blue has a baby brother... (she does?) I have a baby brother... I wonder who he is... (me too!) -Various (Blue's Room, 2006)
    void computeVertexLocations();
    std::vector<Mesh> visBones(float R = 0.2) ;

    void deform(std::string name, Transform t) ;
};

#endif