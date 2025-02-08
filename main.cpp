#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

using namespace std;

std::string MESHES = "misc";
std::string BUFFER_PATH = "BUFFER";
std::string VIDEO_PATH = "video";

// Test 6 Long
inline void phongtest() {
	int SUB = 2;
	Mesh lao = icosphere(1, SUB);


	float SSS = 1;

	Transform offset(Vector3(0, -2, -4) * SSS, Rotation3(Vector3(1, 1, 1), M_PI / 8));
	lao.Trans(offset);

	Mesh shi(lao);
	

	shi.Trans(Transform(Vector3(2, 2, -1)));
	

	Mesh proto = icosphere(1, SUB);
	proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");
	proto = icosphere(1, SUB);
	proto.Trans(Transform(Vector3(-1, 0, -4), Rotation3(Vector3(0, 1, 0), M_PI * 1.2)));
	
	int N = 1024;

	Scene s(N, N / 2);

	float A = 0.5;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-4, 2, 2)));
	s.lights.push_back(PL);

	PointLight PL2(Vector3(1, 1, 1), A);
	PL2.Trans(Transform(Vector3(2, 1, 0)));
	s.lights.push_back(PL2);

	s.clearBuffer();

	auto start = std::chrono::high_resolution_clock::now();

	int SPE = 16;

	

	s.fillMesh(lao, BaseMaterial(0xFF0000FF, SPE * SPE), true, true);
	s.fillMesh(shi, BaseMaterial(0x00FF00FF, SPE), true, true);
	s.fillMesh(proto, BaseMaterial(0x0000FFFF, 1), false);

	std::cout << "THING\n";

	s.drawQueue();

	std::cout << "CNT " << s.TRIANGLE_COUNT << "\n";

	auto finish = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()) / 1000000 << "ns\n";

	// s.outputFrags("OUT");

	s.outputBuffer(BUFFER_PATH);
}

// TEST 7 debug lighting
inline void sss() {
	Mesh proto = cube(1);
	proto.Trans(Transform(Vector3(0, 0, -2), Rotation3(Vector3(0, 1, 2), M_PI * 1.2)));
	
	int N = 1024;

	Scene s(N, N / 2);

	float A = 0.5;

	PointLight PL(Vector3(1, 0, 0), A);
	PL.Trans(Transform(Vector3(-2, 1, 0)));
	s.lights.push_back(PL);

	PointLight PL2(Vector3(0, 1, 1), A);
	PL2.Trans(Transform(Vector3(2, 1, 0)));
	s.lights.push_back(PL2);

	s.clearBuffer();

	auto start = std::chrono::high_resolution_clock::now();

	int SPE = 16;

	s.fillMesh(proto, BaseMaterial(0xFFFFFFFF, 1), true, true);

	std::cout << "THING\n";

	s.drawQueue();

	std::cout << "CNT " << s.TRIANGLE_COUNT << "\n";

	auto finish = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()) / 1000000 << "ns\n";

	// s.outputFrags("OUT");

	s.outputBuffer(BUFFER_PATH);
}


int main() {
	sss();
	return 0;
}