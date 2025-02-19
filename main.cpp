#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

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

	Transform offset(Vector3(0, -2, -4) * SSS, Rotation3(Vector3(1, 1, 1), M_PI / 8));
	FU.Trans(offset);

	int N = 1024;

	Scene s(N, N / 2);

	float A = 00.5;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-4, 0, 0)));
	s.lights.push_back(PL);

	PointLight PL2(Vector3(1, 1, 1), A);
	PL2.Trans(Transform(Vector3(4, 0, 0)));
	s.lights.push_back(PL2);

	s.clearBuffer();

	auto start = std::chrono::high_resolution_clock::now();

	int SPE = 16;

	uint32_t col1 = 0xFFFFFFFF;
	uint32_t col2 = 0xFF0000FF;

	std::vector<std::vector<uint32_t>> tex({std::vector<uint32_t>{col1, col2}, std::vector<uint32_t>{col2, col1}});

	ImageTexture texx(tex);
	s.fillMesh(FU, texx, false);
	std::cout << "THING\n";

	s.drawQueue();

	std::cout << "CNT " << s.TRIANGLE_COUNT << "\n";

	auto finish = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()) / 1000000 << "ns\n";

	// s.outputFrags("OUT");

	s.outputBuffer(BUFFER_PATH);
}

int main() {
	textest();
	return 0;
}