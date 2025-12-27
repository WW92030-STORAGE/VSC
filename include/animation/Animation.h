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

namespace ANIMI {
    // A set of almost entirely continuous and almost differentiable functions that operate on [0, 1], with F(0) = 0 and F(1) = 1.
    // These functions only take in one parameter. Sorry, budgeting costs.
    typedef float (*INTERP) (float);
    
    // Interpolations

    float LERP(float x);

    

    float SINE(float x);

    float SINE_OUT(float x);

    float SINE_IN(float x);
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

    /*
    
    Keyframes representing active cameras. 
    While the transform and whatever components can be freely interpolated, there is no interpolation for the active camera: once a keyframe is reached the camera is swapped.
    We will have keyframes for all cameras not just the active one.
    
    */
    std::vector<std::map<int, std::pair<ANIMI::INTERP, Vector3> >> cam_origin;
    std::vector<std::map<int, std::pair<ANIMI::INTERP, Quaternion> >> cam_basis;
    std::map<int, int> cam_active;

    
    Animation();

    Animation(Scene* s);

    Animation(const Animation& other);

    bool isMorph(Mesh* m);

    MorphedMesh* asMorph(Mesh* m);

    void loadScene(Scene* s);

    // The rest of the keyframing you will have to do yourself.

    void setTranslation(int i, int frame, Vector3 position, ANIMI::INTERP interp = ANIMI::LERP);

    void deleteTranslation(int i, int frame);

    void setRotation(int i, int frame, Quaternion q, ANIMI::INTERP interp = ANIMI::LERP);

    void setRotation(int i, int frame, Matrix3 q, ANIMI::INTERP interp = ANIMI::LERP);

    void deleteRotation(int i, int frame);

    void setMorph(int i, int frame, std::vector<float> c, ANIMI::INTERP interp = ANIMI::LERP);

    void deleteMorph(int i, int frame);

    // Camera methods

    void setCamOrigin(int i, int frame, Vector3 position, ANIMI::INTERP interp = ANIMI::LERP);

    void deleteCamOrigin(int i, int frame);

    void setCamBasis(int i, int frame, Quaternion q, ANIMI::INTERP interp = ANIMI::LERP);

    void setCamBasis(int i, int frame, Matrix3 q, ANIMI::INTERP interp = ANIMI::LERP);

    void deleteCamBasis(int i, int frame, Matrix3 q, ANIMI::INTERP interp = ANIMI::LERP);

    void setCamActive(int frame, int i);

    void deleteCamActive(int frame);

    /*
    
    The rest of the methods work to actually animate the scene bound by this animation.
    
    */

    // Piecewise (whatever) interpolation

    template <typename T> 
    T PERP(std::map<int, std::pair<ANIMI::INTERP, T> > c, int x);

    // Special linear interpolation for quaternions
    Quaternion SLERP(std::map<int, std::pair<ANIMI::INTERP, Quaternion> > c, int x);

    // Animate meshes and scenes
    void animateMesh(int i, int frame);

    void animateCamera(int i, int frame);

    void animate(int frame);

    // Get the total length in frames
    int getLength();

    // Encode the length in a file
    void encodeLength(std::string FF);
};

#endif