#ifndef BEZIER_EXT
#define BEZIER_EXT

#include <vector>
#include <string>
#include "../Math.h"

#include "BaseCurve.h"
#include "../../objects/Mesh.h"

class BezierCurve : public BaseCurve {
    public:
    
    BezierCurve();

    BezierCurve(std::vector<Vector3> v);
    
    Vector3 query(float t);

    std::string to_string();
};

class BezierSurface : public BaseSurface {
    public:

    BezierSurface();

    BezierSurface(std::vector<std::vector<Vector3>> v);

    std::vector<Vector3> getUCurve(float u);
    
    Vector3 query(float u, float v);
};

#endif