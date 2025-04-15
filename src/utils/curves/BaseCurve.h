#ifndef BASECURVE_EXT
#define BASECURVE_EXT

#include <vector>
#include "../Math.h"

/*

Base curve class. This behaves exactly like a Bezier curve.

*/

class BaseCurve {
    public:
    std::vector<Vector3> controls;
    int size;

    BaseCurve() {
        controls = std::vector<Vector3>({Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1)});
        size = controls.size();
    }

    BaseCurve(std::vector<Vector3> v) {
        controls = std::vector<Vector3>(v);
        size = controls.size();
    }
    
    virtual inline Vector3 query(float t) {
        t = BASE::clamp(t, 0, 1);

        std::vector<Vector3> lerps(controls);
        std::vector<Vector3> interps(controls);
        for (int i = size - 1; i >= 1; i--) {
            for (int j = 0; j < i; j++) interps[j] = lerp(lerps[j], lerps[j + 1], t);

            lerps = interps;
        }
        return lerps[0];
    }
};

#endif