#ifndef BSPLINE_EXT
#define BSPLINE_EXT

#include <vector>
#include <string>
#include <algorithm>
#include "../Math.h"

#include "BaseCurve.h"
#include "../../objects/Mesh.h"

// #include <iostream>

// Uniform cubic B-spline curve

class CubicBSpline : public BaseCurve {
    public:
    
    CubicBSpline() : BaseCurve() {
    }

    CubicBSpline(std::vector<Vector3> v) : BaseCurve(v) {
    }

    // Knots "off the track" are duplicated and clamped.
    inline Vector3 knot(int i) {
        if (i < 0) return controls[0];
        if (i >= size) return controls[size];
        return controls[i];
    }
    
    // order = degree + 1
    inline Vector3 query(float t) {
        if (size <= 3) return Vector3(controls[0]);

        t = BASE::clamp(t, 0, 1);
        int slice = BASE::ifloor(t * (size - 3));
        float slstart = float(slice) / (size - 3);
        float subtime = (t - slstart) * (size - 3);


        // std::cout << t << " " << slice << " " << " " << slstart << " " << subtime << "\n";


        Vector3 b[4] = {controls[slice], controls[slice + 1], controls[slice + 2], controls[slice + 3]};
        float st[4] = {1, subtime, subtime * subtime, subtime * subtime * subtime};

        Vector3 full[4] = {b[0] * -1 + b[1] * 3 - b[2] * 3 + b[3], b[0] * 3 - b[1] * 6 + b[2] * 3, b[0] * -3 + b[2] * 3, b[0] + b[1] * 4 + b[2]};

        Vector3 pp = full[0] * st[3] + full[1] * st[2] + full[2] * st[1] + full[3] * st[0];
        return pp * (1.0 / 6.0);
    }

    inline std::string to_string() {
        std::string res = "BSpline[";
        for (int i = 0; i < size; i++) {
            if (i) res = res + ", ";
            res = res + controls[i].to_string();
        }
        return res + "]";
    }
};

class CubicBSplineSurface : public BaseSurface {
    public:

    CubicBSplineSurface() : BaseSurface() {

    }

    CubicBSplineSurface(std::vector<std::vector<Vector3>> v) : BaseSurface(v) {
        
    }

    inline std::vector<Vector3> getUCurve(float u) {
        std::vector<Vector3> thing(N);
        for (int i = 0; i < N; i++) {
            CubicBSpline curve = CubicBSpline(controls[i]);
            thing[i] = curve.query(u);
        }
        return thing;
    }
    
    inline Vector3 query(float u, float v) {
        u = BASE::clamp(u, 0, 1);
        v = BASE::clamp(v, 0, 1);

        std::vector<Vector3> ucurve = getUCurve(u);
        return CubicBSpline(ucurve).query(v);
    }
};

#endif