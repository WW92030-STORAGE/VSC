#ifndef BASECURVE_EXT
#define BASECURVE_EXT

#include <vector>
#include <string>
#include "../Math.h"
#include "../../objects/Mesh.h"


/*

Base curve class. This behaves exactly like a Bezier curve.

*/

class BaseCurve {
    public:
    std::vector<Vector3> controls;
    int size;

    BaseCurve();

    BaseCurve(std::vector<Vector3> v);
    
    virtual Vector3 query(float t);

    virtual std::string to_string();
};

class BaseSurface {
    public:
    std::vector<std::vector<Vector3>> controls;
    int N, M;

    BaseSurface();

    BaseSurface(std::vector<std::vector<Vector3>> v);

    virtual std::vector<Vector3> getUCurve(float u);
    
    virtual Vector3 query(float u, float v);

    Mesh ctrlmesh(Vector2 topleft = Vector2(0, 0), Vector2 topright = Vector2(1, 0), Vector2 botleft = Vector2(0, 1), Vector2 botright = Vector2(1, 1));

    Mesh render(int cellsU = 1, int cellsV = 1, Vector2 topleft = Vector2(0, 0), Vector2 topright = Vector2(1, 0), Vector2 botleft = Vector2(0, 1), Vector2 botright = Vector2(1, 1));
};

#endif