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

// TEST 9 ORD II (RAY TRACING)
inline void RTXTest() {

	int N = 256;

	RayTracer s(0, N, N);

	s.camera = Camera(M_PI / 2.0);

	float A = 0.1;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	// s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), A);
	P2.Trans(Transform(Vector3(0, 2, 0)));
	s.lights.push_back(P2);

	int S = 2;

	Mesh ord = icosphere(1, 2);

	Mesh ord2 = cube(0.5);
	Mesh ord3 = cube(0.5);

	Transform back(Vector3(0, -2, -5), Rotation3(Vector3(0, 1, 0), -0.4));
	ord.Trans(back);

	Transform back2(Vector3(-0.5, -0.5, -3.5), Rotation3(Vector3(0, 1, 0), -0.4));
	ord2.Trans(back2);

	Transform back3(Vector3(1, -1, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	ord3.Trans(back3);

	float SP = 64;
	ImageTexture cubeMap(cubemap);
	BaseMaterial red(0xFF000000, SP, 0);
	BaseMaterial green(0x00FF0000, SP, 0);
	BaseMaterial blue(0x0000FF00, SP, 0);
	BaseMaterial white(0x80808000, SP, 0);

	s.addMesh(&ord, &cubeMap, true);
	s.addMesh(&ord2, &green, false);
	s.addMesh(&ord3, &blue, false);

	s.DEPTH = 0;

	s.render(true);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.1 CASSIE II (Reflection)
inline void ReflTest() {

	int N = 512;
	int D = 0;

	RayTracer s(D, N, N);

	s.camera = Camera(M_PI / 2.0);

	float A = 0.1;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	// s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), A);
	P2.Trans(Transform(Vector3(0, 2, 0)));
	s.lights.push_back(P2);

	int S = 2;

	Mesh test = Mesh::fromOBJ(MESHES + "/cubenomorph.obj");
	Mesh test2 = Mesh::fromOBJ(MESHES + "/cubemorph.obj");

	MorphedMesh cassie(test);
	cassie.copyTo(test2);

	//Mesh cassie = icosphere(1, 2);

	Mesh cassie2 = cube(0.5);
	Mesh cassie3 = cube(0.5);

	Transform back(Vector3(0, -1, -5), Rotation3(Vector3(0, 1, 0), -0.4));
	cassie.Trans(back);

	Transform back2(Vector3(-0.5, -0.5, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cassie2.Trans(back2);

	Transform back3(Vector3(1, -1, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cassie3.Trans(back3);

	Mesh floor = GridSquare(64, 1);
	floor.Trans(Transform(Vector3(0, -2.5, 0)));

	float SP = 64;
	ImageTexture mat(cubemap);
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial green(0x00FF0000, SP, 1);
	BaseMaterial blue(0x0000FF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	s.addMesh(&cassie, &red, false);
	s.addMesh(&cassie2, &green, false);
	s.addMesh(&cassie3, &blue, false);
	s.addMesh(&floor, &white, false);
	
	s.morph(0, std::vector<float>{0, 1});

	s.DEPTH = 2;

	std::cout << "Prepared\n";

	s.render(true);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

int main() {
	ReflTest();
	return 0;
}