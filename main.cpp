#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

#include "texture_texture.h"

using namespace std;

std::string MESHES = "misc";
std::string BUFFER_PATH = "BUFFER";
std::string VIDEO_PATH = "video";

// TEST 7 FU
inline void textest() {
	int SUB = 0;
	Mesh FU = icosphere(1, SUB);
	FU = cube(1);
	float SSS = 1;

	Transform offset(Vector3(0, -0.75, -4) * SSS, Rotation3(Vector3(1, 2, 1), M_PI / 4));
	FU.Trans(offset);

	int N = 1024;

	Scene s(N, N / 2);

	float A = 00.2;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-4, 0, 0)));
	s.lights.push_back(PL);

	PointLight PL2(Vector3(1, 1, 1), A);
	PL2.Trans(Transform(Vector3(4, 0, 0)));
	s.lights.push_back(PL2);

	s.clearBuffer();

	auto start = std::chrono::high_resolution_clock::now();

	int SPE = 16;

	uint32_t col1 = 0x00FFFFFF;
	uint32_t col2 = 0xFF0000FF;

	std::vector<std::vector<uint32_t>> tex(5, std::vector<uint32_t>(5, col1));
	for (int i = 0; i < 5; i++) tex[i][i] = col2;

	ImageTexture* texx = new ImageTexture(tex);
	s.fillMesh(FU, texx, true, true);
	std::cout << "THING\n";

	s.drawQueue();

	std::cout << "CNT " << s.TRIANGLE_COUNT << "\n";

	auto finish = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()) / 1000000 << "ns\n";

	// s.outputFrags("OUT");

	s.outputBuffer(BUFFER_PATH);
}

// TEST 7.5 OBJ
inline void objtest() {
	std::string path = MESHES + "/mcrproto.obj";
	Mesh mesh = Mesh::fromOBJ(path);
	Transform movemeback(Vector3(0, -1, -1), Rotation3(Vector3(0, 1, 0), M_PI * 1.25));

	mesh.Trans(movemeback);

	std::cout << "Loaded" << mesh.size << "\n";

	int N = 1024;

	Scene s(N, N);

	std::cout << N << "\n";

	PointLight PL(Vector3(1, 1, 1), 0.4);
	PL.Trans(Transform(Vector3(-2, 1, 0)));
	s.lights.push_back(PL);

	s.clearBuffer();

	ImageTexture mat(mcrproto);

	auto start = std::chrono::high_resolution_clock::now();


	for (int i = 0; i < 1; i++) {
		s.clearBuffer();
		s.fillMesh(mesh, &mat, true, true, false);

		std::cout << i << "\n";
	}

	

	std::cout << "Drawn\n";

	auto finish = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()) / 1000000 << "ns\n";


	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

int main() {
	objtest();
	return 0;
}