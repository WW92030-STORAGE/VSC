#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <cfloat>
#include <climits>

#include "../../include/utils/Math.h"

#include "../../include/objects/Mesh.h"
#include "../../include/objects/Triangle.h"
#include "../../include/objects/Object.h"

#include "../../include/camera/Camera.h"
#include "../../include/utils/geometry/Fragment.h"

#include "../../include/scene/Scene.h"
#include "../../include/scene/Color.h"
#include "../../include/material/Material.h"
#include "../../include/material/ImageTexture.h"

#include "../../include/lighting/Light.h"
#include "../../include/lighting/PointLight.h"
#include "../../include/lighting/DirectionalLight.h"

#include "../../include/raytracer/Ray.h"
#include "../../include/raytracer/aabb.h"
#include "../../include/raytracer/BVH.h"

#include "../../include/animation/Animation.h"


namespace ANIMI {
    // A set of almost entirely continuous and almost differentiable functions that operate on [0, 1], with F(0) = 0 and F(1) = 1.
    // These functions only take in one parameter. Sorry, budgeting costs.
    // Interpolations

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

    
    Animation::Animation() {

    }

    Animation::Animation(Scene* s) {
        loadScene(s);
    }

    Animation::Animation(const Animation& other) {
        translation = std::vector<std::map<int, std::pair<ANIMI::INTERP, Vector3> >>(other.translation);
        rotation = std::vector<std::map<int, std::pair<ANIMI::INTERP, Quaternion> >>(other.rotation);
        morphs = std::vector<std::map<int, std::pair<ANIMI::INTERP, std::vector<float>> >>(other.morphs);

        cam_origin = std::vector<std::map<int, std::pair<ANIMI::INTERP, Vector3> >>(other.cam_origin);
        cam_basis = std::vector<std::map<int, std::pair<ANIMI::INTERP, Quaternion> >>(other.cam_basis);
        cam_active = std::map<int, int>(other.cam_active);
    }

    bool Animation::isMorph(Mesh* m) {
        return (dynamic_cast<MorphedMesh*>(m));
    }

    MorphedMesh* Animation::asMorph(Mesh* m) {
        return dynamic_cast<MorphedMesh*>(m);
    }

    void Animation::loadScene(Scene* s) {
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

        // Init the cameras
        N_CAM = s->cameras.size();
        while (cam_basis.size() < N_CAM) {
            cam_origin.push_back(std::map<int, std::pair<ANIMI::INTERP, Vector3>>());
            cam_basis.push_back(std::map<int, std::pair<ANIMI::INTERP, Quaternion>>());
        }

        cam_active.clear();
        cam_active.insert({0, 0}); // Camera at time 0 is camera 0, the default camera

        for (int i = 0; i < N_CAM; i++) {
            Camera c = scene->cameras[i];
            cam_origin[i].clear();
            setCamOrigin(i, 0, c.transform.origin, ANIMI::LERP);
            setCamBasis(i, 0, c.transform.basis, ANIMI::LERP);
        }
    }

    // The rest of the keyframing you will have to do yourself.

    void Animation::setTranslation(int i, int frame, Vector3 position, ANIMI::INTERP interp) {
        translation[i][frame] = {interp, position};
    }

    void Animation::deleteTranslation(int i, int frame) {
        if (translation[i].find(frame) != translation[i].end()) translation[i].erase(translation[i].find(frame));
    }

    void Animation::setRotation(int i, int frame, Quaternion q, ANIMI::INTERP interp) {
        rotation[i][frame] = {interp, q};
    }

    void Animation::setRotation(int i, int frame, Matrix3 q, ANIMI::INTERP interp) {
        rotation[i][frame] = {interp, Quaternion(q)};
    }

    void Animation::deleteRotation(int i, int frame) {
        if (rotation[i].find(frame) != rotation[i].end()) rotation[i].erase(rotation[i].find(frame));
    }

    void Animation::setMorph(int i, int frame, std::vector<float> c, ANIMI::INTERP interp) {
        morphs[i][frame] = {interp, c};
    }

    void Animation::deleteMorph(int i, int frame) {
        if (morphs[i].find(frame) != morphs[i].end()) morphs[i].erase(morphs[i].find(frame));
    }

    // Camera methods

    void Animation::setCamOrigin(int i, int frame, Vector3 position, ANIMI::INTERP interp) {
        cam_origin[i][frame] = {interp, position};
    }

    void Animation::deleteCamOrigin(int i, int frame) {
        if (cam_origin[i].find(frame) != cam_origin[i].end()) cam_origin[i].erase(cam_origin[i].find(frame));
    }

    void Animation::setCamBasis(int i, int frame, Quaternion q, ANIMI::INTERP interp) {
        cam_basis[i][frame] = {interp, q};
    }

    void Animation::setCamBasis(int i, int frame, Matrix3 q, ANIMI::INTERP interp) {
        cam_basis[i][frame] = {interp, Quaternion(q)};
    }

    void Animation::deleteCamBasis(int i, int frame, Matrix3 q, ANIMI::INTERP interp) {
        if (cam_basis[i].find(frame) != cam_basis[i].end()) cam_basis[i].erase(cam_basis[i].find(frame));
    }

    void Animation::setCamActive(int frame, int i) {
        cam_active[frame] = i;
    }

    void Animation::deleteCamActive(int frame) {
        if (cam_active.find(frame) != cam_active.end()) cam_active.erase(cam_active.find(frame));
    }

    /*
    
    The rest of the methods work to actually animate the scene bound by this animation.
    
    */

    // Piecewise (whatever) interpolation

    template <typename T> 
    T Animation::PERP(std::map<int, std::pair<ANIMI::INTERP, T> > c, int x) {
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
    Quaternion Animation::SLERP(std::map<int, std::pair<ANIMI::INTERP, Quaternion> > c, int x) {
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
    void Animation::animateMesh(int i, int frame) {
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

    void Animation::animateCamera(int i, int frame) {
        Camera c = Camera(scene->cameras[i]);
        Transform initial(c.transform);
        Transform inverse = initial.inv();


        Vector3 pos = PERP<Vector3>(cam_origin[i], frame);
        Quaternion rot = SLERP(cam_basis[i], frame);

        Transform finaltransform(pos, rot.toRotation());
        Transform displacement = finaltransform * inverse;

        c.Trans(displacement);
        scene->cameras[i] = c;
    }

    void Animation::animate(int frame) {
        for (int i = 0; i < N; i++) animateMesh(i, frame);
        for (int i = 0; i < N_CAM; i++) animateCamera(i, frame);

        auto index = cam_active.upper_bound(frame);
        if (index == cam_active.begin()) {
            scene->setActiveCamera(0);
        } else {
            index--;
            scene->setActiveCamera(index->second);
        }
    }

    // Get the total length in frames
    int Animation::getLength() {
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
    void Animation::encodeLength(std::string FF) {
        int res = getLength();

        std::ofstream len(FF);
	    len << res;
	    len.close();
    }