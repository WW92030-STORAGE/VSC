#ifndef EVERYTHING_EXT
#define EVERYTHING_EXT

// #include <iostream>

// Math and linalg

#include "utils/Math.h"
#include "utils/Random.h"

// Geometry and objects

#include "utils/geometry/Primitives.h"
#include "objects/Object.h"
#include "objects/Triangle.h"
#include "objects/Mesh.h"
#include "objects/MorphedMesh.h"

#include "utils/geometry/Fragment.h"

#include "objects/Triangulate.h"

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

// Material

#include "material/Material.h"
#include "material/ImageTexture.h"

// Ray tracing

#include "raytracer/Ray.h"
#include "raytracer/RayTracer.h"

#endif