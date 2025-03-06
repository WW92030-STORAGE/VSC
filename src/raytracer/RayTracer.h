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

#include <iostream>

class RayTracer : public Scene {
	public:
	std::vector<Mesh> meshes;
	std::vector<BaseMaterial> materials;
	std::vector<bool> NormInterps;
	int DEPTH = 0;

	RayTracer(float depth = 0) : Scene() {
		DEPTH = depth;
	}

	RayTracer(float depth = 0, int w = 256, int h = 256) : Scene(w, h) {
		DEPTH = depth;
	}

	inline void addMesh(Mesh mesh, BaseMaterial mat = BASEMAT_WHITE, bool INTERPNORM = false) {
		meshes.push_back(mesh);
		materials.push_back(mat);
		NormInterps.push_back(INTERPNORM);
	}

	// Lighting and Shading

	inline uint32_t shade(Vector3 position, Vector3 normal, Vector2 uv, BaseMaterial* material, bool LIT = true) override {
		Vector3 col = rgb(getColor(material, uv));
		if (!LIT) return rgb(col);
		
		Vector3 I(ambientLight);

		for (auto light : lights) {
			if (light.TYPE == POINT) {
				Light pl = light;
				Vector3 L = pl.transform.origin - position;
				Vector3 shadedIntensity(0, 0, 0);

				// WARNING - Need to compute a shadow ray

				const float E = 0.01;
				Ray r(position + L.normalized() * E, L);

				IntersectionPoint pp = intersectRay(r);
				if (pp.valid()) continue;

				// Diffuse term
				float scale = normal.normalized() * L.normalized();
				// Only do light contribution if normal is facing light
				if (scale > 0) {
					shadedIntensity = shadedIntensity + (pl.intensity * scale);

					// Specular term

					if (material->specular > 0) {

						Vector3 reflected = normal * (2 * normal.dot(L)) - L;
						float rv = reflected.normalized().dot((position * -1).normalized());
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
		return rgb(res);
	}

	// Ray Tracing

	inline IntersectionPoint intersectRay(Ray r) {
		Triangle3 t;
		float minTime = INF;
		BaseMaterial mat(BASEMAT_WHITE);
		Vector3 N(NILVEC3);
		Vector2 uv(0, 0);

		for (int objind = 0; objind < meshes.size() && objind < materials.size(); objind++) {
			BaseMaterial material = BaseMaterial(materials[objind]);
			Mesh mesh = meshes[objind];
			for (int triind = 0; triind < mesh.triindices.size(); triind++) {
				Triangle3 triangle = mesh.makeTriangle(triind);

				float time = r.intersectTriangle(triangle);
				if (time < 0) continue;
				if (time == NAN) continue;
				if (time < minTime) {
					minTime = time;
					t = Triangle3(triangle);
					mat = material;
					N = t.normal();
					Vector3 pp = r.get(time);
					if (NormInterps[objind]) {
						N = mesh.getInterpolatedNormal(pp, triind);
					}
					uv = mesh.getUV(pp, triind);
				}
			}
		}

		// if (minTime != INF) std::cout << r.to_string() << minTime << " " << uv.to_string() << " " << mat.TYPE << "\n";

		return IntersectionPoint{&mat, minTime, N, uv};
	}

	inline Ray castRay(Vector2 NDC) {
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

	inline uint32_t tracePixel(Vector2 NDC, bool LIT = false) {
		Ray r = castRay(NDC);
		IntersectionPoint ip = intersectRay(r);

		if (!ip.valid()) return 0x000000FF;

		Vector3 pos = r.get(ip.time);

		return illuminate(r.slope, pos, ip.N, ip.uv, ip.material, true);
	}

	// Remember: pixels go from BOTTOM LEFT TO TOP RIGHT. This means you will have to reverse the Y direction when outputting.
	inline uint32_t tracePixel(int x, int y, bool LIT = true) {
		// In this engine the screen is centered on the center of the NDC space. 
		// If the screen is rectangular then we chop off the sides instead of distorting.
		float Nx = float(2 * x - W) / SIDE;
		float Ny = float(2 * y - H) / SIDE;

		float pixelSize = 1.0 / SIDE;

		Nx += 0.5 * pixelSize;
		Ny += 0.5 * pixelSize;

		return tracePixel(Vector2(Nx, Ny), LIT);
	}

	// Remember: pixels go from BOTTOM LEFT TO TOP RIGHT. This means you will have to reverse the Y direction when outputting.
	inline void render(bool LIT = true) {
		for (int x = 0; x < W; x++) {
			for (int y = 0; y < H; y++) {
				buffer[x][y] = ReducedFrag(0, tracePixel(x, y, LIT));
			}
		}
	}
};



#endif