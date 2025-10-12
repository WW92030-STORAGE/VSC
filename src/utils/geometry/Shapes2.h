#ifndef SHAPESII_EXT
#define SHAPESII_EXT

// #include <iostream>

#include "Primitives.h"
#include "../../objects/Mesh.h"
#include "../../objects/Triangle.h"

#include "Shapes.h"

#include <map>
#include <vector>
#include <utility>

/*

This header has generators for more advanced shapes. Often these shapes are composed of multiple primitive meshes.

*/

std::vector<Mesh> MengerSponge(float R, int L, bool cubesix = false) {
	L = BASE::iclamp(L, 0, 4);

	if (L <= 0 || BASE::fzero(R) || R < 0) return std::vector<Mesh>(1, cube(1));

	std::vector<Vector3> points(1, Vector3(0, 0, 0));

	float SR = R;

	for (int l = 0; l < L; l++) {
		std::vector<Vector3> next;
		for (auto i : points) {
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					for (int z = -1; z <= 1; z++) {
						if (x == 0 && y == 0) continue;
						if (y == 0 && z == 0) continue;
						if (z == 0 && x == 0) continue;
						Vector3 disp = Vector3(x, y, z) * SR * 1 / 3;
						next.push_back(i + disp);
					}
				}
			}
		}

		points = next;
		SR /= 3;
	}

	std::vector<Mesh> res;

	for (auto i : points) {
		Mesh m;
		if (cubesix) m = cube6(SR);
		else m = cube(SR);
		m.Trans(Transform(i));
		res.push_back(Mesh(m));
	}
	return res;
}


#endif