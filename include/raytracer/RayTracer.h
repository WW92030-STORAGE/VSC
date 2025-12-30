#ifndef RAYTRACER_EXT
#define RAYTRACER_EXT

#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <cfloat>
#include <climits>

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

#include "Ray.h"
#include "aabb.h"
#include "BVH.h"


// #include <iostream>

/*

VSC PART II - FINAL DESTINATION

RayTracer.h is a subclass of Scene.h that supports both rasterization and ray tracing.
Currently we support Phong shading of objects and the casting of shadows.
Reflections are also supported but not refractions/translucent objects yet.

*/

class RayTracer : public Scene {
	public:

	int DEPTH = 0;

	BVH* bvh = nullptr;
	bool UseBVH = true; // Since VSC 9.81 this is always on by default.

	const float RAY_EPSILON = 0.01;

	RayTracer(float depth = 0);

	RayTracer(float depth = 0, int w = 256, int h = 256);

	virtual ~RayTracer();
	
	// Lighting and Shading

	Vector3 shade(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true) override;
	
	// Ray Tracing

	IntersectionPoint intersectRayNaive(Ray r);

	IntersectionPoint intersectRay(Ray r);

	Ray castRay(Vector2 NDC);

	Vector3 traceRay(Ray r, bool LIT = false, int depth = 0);

	uint32_t tracePixel(Vector2 NDC, bool LIT = false, int depth = 0);

	// Remember: pixels go from BOTTOM LEFT TO TOP RIGHT. This means you will have to reverse the Y direction when outputting.
	uint32_t tracePixel(int x, int y, bool LIT = true, int depth = 0, float inv_side = -1);

	void createBVH();

	// Remember: pixels go from BOTTOM LEFT TO TOP RIGHT. This means you will have to reverse the Y direction when outputting.
	virtual void render(bool LIT = true, int depth = -1);
};



#endif