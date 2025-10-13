#ifndef MISCTESTS_EXT_
#define MISCTESTS_EXT_

#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

using namespace std;

#include "main_utils.h"

// Miscellaneous tests that do not fall into the main test suite. These are mostly math related things.



// Perlin Noise Test
inline void NoiseTest() {
	PerlinNoise2 pno(101);
	pno.spacing = 0.5;

	float d = 0.02;

	for (float x = -1; x <= 1; x += d) {
		for (float y = -1; y <= 1; y += d) {
			float z = pno.rand(x, y);
			std::cout << gradientBlock(z) << "";
		}
		std::cout << "\n";
	}
}

// Fractal Noise Test
inline void FracNoiseTest() {
	PerlinNoise2 pno(100);

	FractalNoise2 noise(&pno, 4);

	float d = 0.02;

	for (float x = -1; x <= 1; x += d) {
		for (float y = -1; y <= 1; y += d) {
			float z = noise.rand(x, y);
			std::cout << gradientBlock(z) << "";
		}
		std::cout << "\n";
	}
}

// Bezier Curve Test
inline void BezierTest() {
	std::vector<Vector3> ctrls({Vector3(1, 0, 0), Vector3(1, 1, 0), Vector3(0, 1, 0), Vector3(0, 0, 0)});
	BezierCurve b(ctrls);

	for (float i = 0; i < 1; i += 0.05) std::cout << i << " " << b.query(i).to_string() << "\n";
} 

// Noise Test
inline void noisetex() {

	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	Mesh mesh = cube(2);

	Transform back(Vector3(0, -2, -4), Rotation3(Vector3(0, 1, 0), -0.4));

	std::cout << "Animated\n";

	s.clearBuffer();

	mesh.Trans(back);

	int TEXLEN = 64;
	auto noiseoctave = PerlinNoise2(420);
	noiseoctave.spacing = 0.25;
	auto noise = FractalNoise2(&noiseoctave, 3, 0.5, 2.0);

	std::vector<std::vector<uint32_t>> noisetex(TEXLEN, std::vector<uint32_t>(TEXLEN, 0xFF0000FF));

	for (int i = 0; i < TEXLEN; i++) {
		for (int j = 0; j < TEXLEN; j++) {
			float ff = noise.rand(float(i) / TEXLEN, float(j) / TEXLEN);
			noisetex[i][j] = rgb(Vector3(1.0, 1.0, 1.0) * ff);
		}
	}

	ImageTexture* mat = new ImageTexture(noisetex);
	mat->specular = 64;

	s.fillMesh(mesh, mat);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete mat;
}


#endif