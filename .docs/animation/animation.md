# animation/Animation

`class Animation`

## Description

Bind animations to scenes

## Notes

- You can load in different scenes however the keyframes and whatnot are preserved except the first frame. Upon loading a scene its initial contents are set as frame zero.
- The `ANIMI::INTERP` functions determine how tweening goes from preceding keyframe to current keyframe. No tweening occurs before the first keyframe and after the last keyframe.

## Fields

Each vector is indexed by mesh index (see `Scene.h` for how that works). Each index stores a map from frame numbers to keyframe values. The `ANIMI::INTERP` that is also stored tells how the mesh will tween from the preceding keyframe to the current one.

- `scene: Scene*` = The scene to be animated
- `N: int` = Number of meshes in the `meshes` of the `scene`
- `translation: std::vector<std::map<int, std::pair<ANIMI::INTERP, Vector3> >>` = Non-rotational translation (i.e. position or origin)
- `rotation: std::vector<std::map<int, std::pair<ANIMI::INTERP, Quaternion> >>` = Rotational motion (i.e. rotation or basis) stored as quaternions
- `morphs: std::vector<std::map<int, std::pair<ANIMI::INTERP, std::vector<float>> >>` = Morph coefficients (use `MorphedMesh::coeffs` to get coeffs) for morphable meshes

## Constructors

- `Animation()`
- `Animation(Scene* s)`

## Functions

- `bool isMorph(Mesh* m)` = Utility to determine if a mesh is morphable
- `MorphedMesh* asMorph(Mesh* m)` = Convert to morphedmesh reference
- `void loadScene(Scene* s)` = Insert a scene. This will set the initial state of all meshes to the 0th frame as the first keyframe.

- `void setTranslation(int i, int frame, Vector3 position, ANIMI::INTERP interp = ANIMI::LERP)` = The `interp` param details how the animation will tween from the preceding keyframe to this one.
- `deleteTranslation(int i, int frame)` = All deletions return nothing.
- `setRotation(int i, int frame, Quaternion q, ANIMI::INTERP interp = ANIMI::LERP)`
- `setRotation(int i, int frame, Matrix3 q, ANIMI::INTERP interp = ANIMI::LERP)` = The rotation `q` is converted to an arbitrary corresponding quaternion
- `void deleteRotation(int i, int frame)`
- `void setMorph(int i, int frame, std::vector<float> c, ANIMI::INTERP interp = ANIMI::LERP)`
- `void deleteMorph(int i, int frame)`
- `template <typename T> T PERP(std::map<int, std::pair<ANIMI::INTERP, T> > c, int x)` = Piecewise interpolate to get the value of a thing at frame `x` given the keyframes `c`.
- `Quaternion SLERP(std::map<int, std::pair<ANIMI::INTERP, Quaternion> > c, int x)` = Special case of `PERP` to do spherical interpolation on quaternions
- `void animateMesh(int i, int frame)` = Animate a single mesh, the mesh at index `i` to the frame `frame`.
- `void animate(int frame)` = Animate the entire scene (all its `meshes`) to the `frame` frame.
- `int getLength()` = The number of the final keyframe.
- `void encodeLength(std::string FF)` = Output the length to a file.

---

# animation/ANIMI

`namespace ANIMI`

## Description

Utility functions for animations, keyframes, and tweening

## Notes

Naming is loosely based on Geometry Dash.

## Fields

- `typedef float (*INTERP)(float)` = We define `INTERP` (`ANIMI::INTERP`) as a function that maps floats to floats. This type specifically represents interpolation functions that are almost entirely continuous and almost entirely differentiable, where `F(0) = 0` and `F(1) = 1`.

## Functions

- `float LERP(float x) = x`
- `SINE(float x) = 0.5 * (1.0 - cosf(x * M_PI))`
- `SINE_OUT(float x) = sinf(M_PI * 0.5 * x)`
- `SINE_IN(float x) = 1.0 - cosf(M_PI * 0.5 * x)`