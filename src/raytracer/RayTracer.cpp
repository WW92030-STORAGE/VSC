#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <cfloat>
#include <climits>
#include <chrono>



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



#include "../../include/raytracer/RayTracer.h"


/*

VSC PART II - FINAL DESTINATION

RayTracer is a subclass of Scene that supports both rasterization and ray tracing.
Currently we support Phong shading of objects and the casting of shadows.
Reflections are also supported but not refractions/translucent objects yet.

*/

	RayTracer::RayTracer(float depth) : Scene() {
		DEPTH = depth;
	}

	RayTracer::RayTracer(float depth, int w, int h) : Scene(w, h) {
		DEPTH = depth;
	}

	RayTracer::~RayTracer() {
		if (bvh) delete bvh;
	}
	
	// Lighting and Shading

	Vector3 RayTracer::shade(Vector3 pRay, Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT) {
		Vector3 col = rgb(getColor(material, uv));
		if (material->TYPE == BaseMaterial::IMAGE) {
			ImageTexture* ii = dynamic_cast<ImageTexture*>(material);
			// std::cout << col.to_string() << "\n";
		}
		if (!LIT) return col;
		
		Vector3 I(ambientLight);

		for (auto light : lights) {
			if (light.TYPE == POINT) {
				Light pl = light;
				Vector3 L = pl.transform.origin - position;
				Vector3 shadedIntensity(0, 0, 0);

				// WARNING - Need to compute a shadow ray

				Vector3 Lnorm = L.normalized();

				Ray r(position + Lnorm * RAY_EPSILON, L);

				IntersectionPoint pp = intersectRay(r);
				if (pp.valid()) continue;

				// Diffuse term
				float scale = normal * Lnorm;
				// Only do light contribution if normal is facing light
				if (scale > 0) {
					shadedIntensity = shadedIntensity + (pl.intensity * scale);

					// Specular term

					if (material->specular > 0) {

						Vector3 reflected = normal * (2 * normal.dot(L)) - L;
						float rv = reflected.normalized().dot((pRay * -1).normalized());
						if (rv > 0) {
							float spec = powf(rv, material->specular);
							shadedIntensity = shadedIntensity + (pl.intensity * spec);
						// std::cout << (pl.intensity * spec).to_string() << " " << specular << "\n";
						}
					}

				}

				shadedIntensity = shadedIntensity / fmax(pl.attenuation * L.length(), 1);
				
				I = I + shadedIntensity;
			}
			if (I.x > 1) I.x = 1;
			if (I.y > 1) I.y = 1;
			if (I.z > 1) I.z = 1;
		}

		// std::cout << position.to_string() << " = " << I.to_string() << "\n";


		Vector3 res(col.x * I.x, col.y * I.y, col.z * I.z);
		return res;
	}
	
	// Ray Tracing

	IntersectionPoint RayTracer::intersectRayNaive(Ray r) {

		Triangle3 t;
		float minTime = INF;
		BaseMaterial* mat = materials[0];
		Vector3 N(NILVEC3);
		Vector2 uv(0, 0);

		for (int objind = 0; objind < meshes.size() && objind < materials.size(); objind++) {
			BaseMaterial* material = materials[objind];
			Mesh mesh = *meshes[objind];
			for (int triind = 0; triind < mesh.triindices.size(); triind++) {
				Triangle3 triangle = mesh.makeTriangle(triind);

				if (triangle.N * r.slope > 0) continue; // Backface culling

				float time = r.intersectTriangle(triangle);
				if (time < 0) continue;
				if (std::isnan(time)) continue;
				if (time < minTime) {
					minTime = time;
					t = Triangle3(triangle);
					mat = material;
					N = t.normal();
					Vector3 pp = r.get(time);
					Vector3 bary = triangle.bary(pp);
					if (NormInterps[objind]) {
						N = mesh.getInterpolatedNormal_bary(bary, triind, triangle);
					}
					uv = mesh.getUV_bary(bary, triind, triangle);
				}
			}
		}

		// if (minTime != INF) std::cout << r.to_string() << minTime << " " << uv.to_string() << " " << mat.TYPE << "\n";

		return IntersectionPoint{mat, minTime, N, uv};
	}

	IntersectionPoint RayTracer::intersectRay(Ray r) {
		if (!UseBVH) return intersectRayNaive(r);

		auto i = bvh->IntersectRay(r);
		// std::cout << r.to_string() << " " << i.to_string() << "\n";
		return i;
	}

	Ray RayTracer::castRay(Vector2 NDC) {
		Vector3 right = camera.right();
		Vector3 up = camera.up();
		Vector3 negz = camera.look() * -1; // similar to OpenGL the look actually goes backwards.

		// Recall alpha is the FOV which means a square of radius tanf(alpha * 0.5) 
		// when placed 1 unit in the look direction will match the borders.

		float radius = tanf(camera.alpha * 0.5);

		Vector3 dir = negz + (right * NDC.x * radius) + (up * NDC.y * radius);
		dir = dir.normalized();

		return Ray(camera.transform.origin, dir);
	}

	Vector3 RayTracer::traceRay(Ray r, bool LIT, int depth) {
		if (depth > DEPTH) return Vector3(0, 0, 0);
		IntersectionPoint ip = intersectRay(r);

		// std::cout << ip.to_string() << "\n";

		if (!ip.valid()) return Vector3(0, 0, 0);

		// std::cout << ip.material->to_string() << "\n";

		Vector3 pos = r.get(ip.time);

		Vector3 color = illuminate(r.slope, pos, ip.N, ip.uv, ip.material, LIT);

		if (!LIT) return color;

		Vector3 point = r.get(ip.time);

		if (ip.material->reflective > 0 && !BASE::fzero(ip.material->reflective)) {
			// if (ip.material->reflective > 1) std::cout << (uint64_t)(ip.material) << " = " << ip.material->to_string() << "\n";
			Vector3 incident = r.slope.normalized() * -1;
			Vector3 refl_dir = (ip.N * 2 * (ip.N * incident) - incident).normalized();

			Vector3 reflResult = traceRay(Ray(pos + refl_dir * RAY_EPSILON, refl_dir), LIT, depth + 1);
			color = color + reflResult * ip.material->reflective;
			color = clampColor(color);
		}


		return color;
	}

	uint32_t RayTracer::tracePixel(Vector2 NDC, bool LIT, int depth) {
		Ray r = castRay(NDC);
		Vector3 color = traceRay(r, LIT, depth);

		return rgb(color);
	}

	// Remember: pixels go from BOTTOM LEFT TO TOP RIGHT. This means you will have to reverse the Y direction when outputting.
	uint32_t RayTracer::tracePixel(int x, int y, bool LIT, int depth) {
		// In this engine the screen is centered on the center of the NDC space. 
		// If the screen is rectangular then we chop off the sides instead of distorting.
		float Nx = float(2 * x - W) / SIDE;
		float Ny = float(2 * y - H) / SIDE;

		float pixelSize = 1.0 / SIDE;

		Nx += 0.5 * pixelSize;
		Ny += 0.5 * pixelSize;

		return tracePixel(Vector2(Nx, Ny), LIT, depth);
	}

	void RayTracer::createBVH() {
		delete bvh;
		bvh = create(meshes, materials, NormInterps);
	}

	// Remember: pixels go from BOTTOM LEFT TO TOP RIGHT. This means you will have to reverse the Y direction when outputting.
	void RayTracer::render(bool LIT, int depth) {
		if (depth < 0) depth = 0;
		if (UseBVH) createBVH();

		for (int x = 0; x < W; x++) {
			// std::cout << x << "\n";
			for (int y = 0; y < H; y++) {
				buffer[x][y] = ReducedFrag(0, tracePixel(x, y, LIT, depth));
			}
		}
	}