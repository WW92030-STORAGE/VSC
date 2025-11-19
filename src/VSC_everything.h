#ifndef EVERYTHING_EXT
#define EVERYTHING_EXT

// #include <iostream>

// Math and linalg

#include "utils/Math.h"
#include "utils/Random.h"

// Curves

#include "utils/curves/BaseCurve.h"
#include "utils/curves/Bezier.h"
#include "utils/curves/BSpline.h"

// Geometry and objects

#include "utils/geometry/Primitives.h"
#include "objects/Object.h"
#include "objects/Triangle.h"
#include "objects/Mesh.h"
#include "objects/MorphedMesh.h"
#include "objects/RiggedMesh.h"

#include "objects/QuadMesh.h"

#include "objects/Clip.h"


#include "utils/geometry/Fragment.h"

#include "objects/Triangulate.h"
#include "objects/Subdivide.h"

// Pre-built shape generators

#include "utils/geometry/Shapes.h"
#include "utils/geometry/Shapes2.h"

// Camera and lighting

#include "camera/Camera.h"
#include "lighting/Light.h"
#include "lighting/DirectionalLight.h"
#include "lighting/PointLight.h"

// Scene

#include "scene/Color.h"
#include "scene/Scene.h"
#include "scene/Shader.h"

// Material

#include "material/Material.h"
#include "material/ImageTexture.h"

// Ray tracing

#include "raytracer/Ray.h"
#include "raytracer/RayTracer.h"
#include "raytracer/aabb.h"
#include "raytracer/BVH.h"

// Animation

#include "animation/Animation.h"

#endif