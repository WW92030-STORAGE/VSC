#ifndef BSPLINE_EXT
#define BSPLINE_EXT

#include <vector>
#include <string>
#include <algorithm>
#include "../Math.h"

#include "BaseCurve.h"
#include "../../objects/Mesh.h"

// Uniform cubic B-spline curve

class CubicBSpline : public BaseCurve {
    public:
    
    CubicBSpline();

    CubicBSpline(std::vector<Vector3> v);

    // Knots "off the track" are duplicated and clamped.
    Vector3 knot(int i);
    
    // order = degree + 1
    Vector3 query(float t);

    std::string to_string();
};

class CubicBSplineSurface : public BaseSurface {
    public:

    CubicBSplineSurface();

    CubicBSplineSurface(std::vector<std::vector<Vector3>> v);

    std::vector<Vector3> getUCurve(float u);
    
    Vector3 query(float u, float v);
};

#endif