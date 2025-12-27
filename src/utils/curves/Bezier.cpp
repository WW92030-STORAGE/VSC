#include <vector>
#include <string>
#include "../../../include/utils/Math.h"

#include "../../../include/utils/curves/BaseCurve.h"
#include "../../../include/objects/Mesh.h"



#include "../../../include/utils/curves/Bezier.h"

    BezierCurve::BezierCurve() : BaseCurve() {
    }

    BezierCurve::BezierCurve(std::vector<Vector3> v) : BaseCurve(v) {
    }
    
    Vector3 BezierCurve::query(float t) {
        t = BASE::clamp(t, 0, 1);

        std::vector<Vector3> lerps(controls);
        std::vector<Vector3> interps(controls);
        for (int i = size - 1; i >= 1; i--) {
            for (int j = 0; j < i; j++) interps[j] = lerp(lerps[j], lerps[j + 1], t);

            lerps = interps;
        }
        return lerps[0];
    }

    std::string BezierCurve::to_string() {
        std::string res = "Bezier[";
        for (int i = 0; i < size; i++) {
            if (i) res = res + ", ";
            res = res + controls[i].to_string();
        }
        return res + "]";
    }

    BezierSurface::BezierSurface() : BaseSurface() {

    }

    BezierSurface::BezierSurface(std::vector<std::vector<Vector3>> v) : BaseSurface(v) {
        
    }

    std::vector<Vector3> BezierSurface::getUCurve(float u) {
        std::vector<Vector3> thing(N);
        for (int i = 0; i < N; i++) {
            BezierCurve curve = BezierCurve(controls[i]);
            thing[i] = curve.query(u);
        }
        return thing;
    }
    
    Vector3 BezierSurface::query(float u, float v) {
        u = BASE::clamp(u, 0, 1);
        v = BASE::clamp(v, 0, 1);

        std::vector<Vector3> ucurve = getUCurve(u);
        return BezierCurve(ucurve).query(v);
    }