#ifndef BEZIER_EXT
#define BEZIER_EXT

#include <vector>
#include <string>
#include "../Math.h"

#include "BaseCurve.h"
#include "../../objects/Mesh.h"

// #include <iostream>

class BezierCurve : public BaseCurve {
    public:
    
    BezierCurve() : BaseCurve() {
    }

    BezierCurve(std::vector<Vector3> v) : BaseCurve(v) {
    }
    
    inline Vector3 query(float t) {
        t = BASE::clamp(t, 0, 1);

        std::vector<Vector3> lerps(controls);
        std::vector<Vector3> interps(controls);
        for (int i = size - 1; i >= 1; i--) {
            for (int j = 0; j < i; j++) interps[j] = lerp(lerps[j], lerps[j + 1], t);

            lerps = interps;
        }
        return lerps[0];
    }

    inline std::string to_string() {
        std::string res = "Bezier[";
        for (int i = 0; i < size; i++) {
            if (i) res = res + ", ";
            res = res + controls[i].to_string();
        }
        return res + "]";
    }
};

class BezierSurface : public BaseSurface {
    public:

    BezierSurface() : BaseSurface() {

    }

    BezierSurface(std::vector<std::vector<Vector3>> v) : BaseSurface(v) {
        
    }

    inline std::vector<Vector3> getUCurve(float u) {
        std::vector<Vector3> thing(N);
        for (int i = 0; i < N; i++) {
            BezierCurve curve = BezierCurve(controls[i]);
            thing[i] = curve.query(u);
        }
        return thing;
    }
    
    inline Vector3 query(float u, float v) {
        u = BASE::clamp(u, 0, 1);
        v = BASE::clamp(v, 0, 1);

        std::vector<Vector3> ucurve = getUCurve(u);
        return BezierCurve(ucurve).query(v);
    }
};

#endif