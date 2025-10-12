#ifndef ANIMATION_EXT
#define ANIMATION_EXT

#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <cfloat>
#include <climits>

#include "../utils/Math.h"

#include "../objects/Mesh.h"
#include "../objects/Triangle.h"
#include "../objects/Object.h"

#include "../camera/Camera.h"
#include "../utils/geometry/Fragment.h"

#include "../scene/Scene.h"
#include "../scene/Color.h"
#include "../material/Material.h"
#include "../material/ImageTexture.h"

#include "../lighting/Light.h"
#include "../lighting/PointLight.h"
#include "../lighting/DirectionalLight.h"

#include "../raytracer/Ray.h"
#include "../raytracer/aabb.h"
#include "../raytracer/BVH.h"

#include <iostream>

namespace ANIMI {
    // A set of almost entirely continuous and almost differentiable functions that operate on [0, 1], with F(0) = 0 and F(1) = 1.
    // These functions only take in one parameter. Sorry, budgeting costs.
    typedef float (*INTERP) (float);

    float LERP(float x) {
        return x;
    }

    float SINE(float x) {
        return 0.5 * (1.0 - cosf(x * M_PI));
    }

    float SINE_OUT(float x) {
        return sinf(M_PI * 0.5 * x);
    }

    float SINE_IN(float x) {
        return 1.0 - cosf(M_PI * 0.5 * x);
    }
}

class Animation {
    public:
    Scene* scene;
    int N = 0; // Number of meshes
    int N_CAM = 1; // Number of cameras

    /*
    
    Keyframe system. Each keyframe has a frame number and maps to an interpolation (INTERP) and a control point.
    The interpolation of a keyframe represents how you get from the PRECEDING FRAME to the CURRENT FRAME.
    This means the 0th ANIMI::INTERP value has no effects.

    Each one is a vector representing a mesh. 
    
    */
    std::vector<std::map<int, std::pair<ANIMI::INTERP, Vector3> >> translation;
    std::vector<std::map<int, std::pair<ANIMI::INTERP, Quaternion> >> rotation;
    std::vector<std::map<int, std::pair<ANIMI::INTERP, std::vector<float>> >> morphs;

    
    Animation() {

    }

    Animation(Scene* s) {
        loadScene(s);
    }

    Animation(const Animation& other) {
        translation = std::vector<std::map<int, std::pair<ANIMI::INTERP, Vector3> >>(other.translation);
        rotation = std::vector<std::map<int, std::pair<ANIMI::INTERP, Quaternion> >>(other.rotation);
        morphs = std::vector<std::map<int, std::pair<ANIMI::INTERP, std::vector<float>> >>(other.morphs);
    }

    inline bool isMorph(Mesh* m) {
        return (dynamic_cast<MorphedMesh*>(m));
    }

    inline MorphedMesh* asMorph(Mesh* m) {
        return dynamic_cast<MorphedMesh*>(m);
    }

    inline void loadScene(Scene* s) {
        scene = s;
        N = s->meshes.size();
        while (translation.size() < N) {
            translation.push_back(std::map<int, std::pair<ANIMI::INTERP, Vector3>>());
            rotation.push_back(std::map<int, std::pair<ANIMI::INTERP, Quaternion>>());
            morphs.push_back(std::map<int, std::pair<ANIMI::INTERP, std::vector<float>>>());
        }

        // Init the meshes. By default the first keyframe of a mesh is {0 frames, state as given in the scene}. If you change this keyframe the mesh will also change state during that frame.
        for (int i = 0; i < N; i++) {
            Mesh* m = scene->meshes[i];
            translation[i].clear();
            setTranslation(i, 0, m->transform.origin, ANIMI::LERP);
            setRotation(i, 0, m->transform.basis, ANIMI::LERP);
            if (isMorph(m)) {
                MorphedMesh* mm = asMorph(m);
                setMorph(i, 0, mm->coeffs);
            }
        }
    }

    // The rest of the keyframing you will have to do yourself.

    inline void setTranslation(int i, int frame, Vector3 position, ANIMI::INTERP interp = ANIMI::LERP) {
        translation[i][frame] = {interp, position};
    }

    inline void deleteTranslation(int i, int frame) {
        if (translation[i].find(frame) != translation[i].end()) translation[i].erase(translation[i].find(frame));
    }

    inline void setRotation(int i, int frame, Quaternion q, ANIMI::INTERP interp = ANIMI::LERP) {
        rotation[i][frame] = {interp, q};
    }

    inline void setRotation(int i, int frame, Matrix3 q, ANIMI::INTERP interp = ANIMI::LERP) {
        rotation[i][frame] = {interp, Quaternion(q)};
    }

    inline void deleteRotation(int i, int frame) {
        if (rotation[i].find(frame) != rotation[i].end()) rotation[i].erase(rotation[i].find(frame));
    }

    inline void setMorph(int i, int frame, std::vector<float> c, ANIMI::INTERP interp = ANIMI::LERP) {
        morphs[i][frame] = {interp, c};
    }

    inline void deleteMorph(int i, int frame) {
        if (morphs[i].find(frame) != morphs[i].end()) morphs[i].erase(morphs[i].find(frame));
    }

    /*
    
    The rest of the methods work to actually animate the scene bound by this animation.
    
    */

    // Piecewise (whatever) interpolation

    template <typename T> 
    T PERP(std::map<int, std::pair<ANIMI::INTERP, T> > c, int x) {
        int highest = c.rbegin()->first;
        int lowest = c.begin()->first;
        x = BASE::iclamp(x, lowest, highest);
        auto next = c.upper_bound(x);
        auto start = next;
        start--;
        if (next == c.end() || start == c.end()) return c[highest].second;
        if (next == c.begin()) return c[lowest].second;

        int difference = next->first - start->first;
        if (BASE::fzero(difference)) return start->second.second;
        float interval = (float)(x - start->first) / (difference);

        float endweight = next->second.first(interval);
        return (next->second.second * endweight) + (start->second.second * (1.0 - endweight));
    }

    // Special linear interpolation for quaternions
    Quaternion SLERP(std::map<int, std::pair<ANIMI::INTERP, Quaternion> > c, int x) {
        int highest = c.rbegin()->first;
        int lowest = c.begin()->first;
        x = BASE::iclamp(x, lowest, highest);
        auto next = c.upper_bound(x);
        auto start = next;
        start--;
        if (next == c.end() || start == c.end()) return c[highest].second;
        if (next == c.begin()) return c[lowest].second;

        int difference = next->first - start->first;
        if (BASE::fzero(difference)) return start->second.second;
        float interval = (float)(x - start->first) / (difference);

        float endweight = next->second.first(interval);
        return slerp(start->second.second, next->second.second, endweight);
    }

    // Animate meshes and scenes
    void animateMesh(int i, int frame) {
        // Translate
        Mesh* m = scene->meshes[i];
        Transform initial(scene->meshes[i]->transform);
        Transform inverse = initial.inv();

        // Translation and rotation
        Vector3 pos = PERP<Vector3>(translation[i], frame);
        Quaternion rot = SLERP(rotation[i], frame);
        Transform finaltransform(pos, rot.toRotation());
        Transform displacement = finaltransform * inverse;
        /*
        if (i == 1 && frame < 12) {
            std::cout << "Q\n" << rot.to_string() << "\n";
            std::cout << "F\n" << finaltransform.sprintf() << "\n";
            std::cout << "D\n" << displacement.sprintf() << "\n";
        }
        */

        m->Trans(displacement);

        // Morphs
        MorphedMesh* mm = asMorph(m);
        if (mm) {
            std::vector<float> coeffs = PERP<std::vector<float>>(morphs[i], frame);
            mm->morph(coeffs);
        }


    }

    inline void animate(int frame) {
        for (int i = 0; i < N; i++) animateMesh(i, frame);
    }

    // Get the total length in frames
    inline int getLength() {
        if (translation.size() <= 0) return 0;
        int res = 0;
        for (auto i : translation) {
            if (i.size() <= 0) continue;
            int x = (i.rbegin())->first;
            res = std::max(res, x);
        }
        return res;
    }

    // Encode the length in a file
    inline void encodeLength(std::string FF) {
        int res = getLength();

        std::ofstream len(FF);
	    len << res;
	    len.close();
    }
};

#endif