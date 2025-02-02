#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

using namespace std;

std::string MESHES = "misc";
std::string BUFFER_PATH = "BUFFER";
std::string VIDEO_PATH = "video";

// INTERLUDE AND INTERMISSION - RENDERING AND RASTERIZATION

// TEST 0 - Yes these tests are named after DisneyXD characters.
inline void basictest() {
	int N = 128;

	Scene s(N, N);

	s.clearBuffer();

	Vector3 xxx(-10, -10, -20);
	Vector3 yyy(10, -10, -20);
	Vector3 zzz(-10, -10, -40);

	Triangle3 TTT(xxx, yyy, zzz);

	s.drawTriangle(TTT, 0xFFFFFFFF);

	xxx = Vector3(-10, -10, -40);
	yyy = Vector3(10, -10, -40);
	zzz = Vector3(10, -10, -20);

	Triangle3 UUU(xxx, yyy, zzz);

	s.fillTriangle(UUU, 0x808080FF);
}

// TEST 1 KULDE (Updated to use Shapes.h for the cube)
inline void cubetest() {
	Vector3 OFFSET(21, -61, -30);
	
	Mesh kulde = cube(40);
	kulde.Trans(OFFSET);

	Matrix3 rot1 = Rotation3(Vector3(0, 1, 0), M_PI / 4);

	Transform r1(Vector3(), rot1);

	kulde.Trans(r1);

	Matrix3 rot2 = Rotation3(Vector3(1, 0, 0), M_PI / 6);
	Transform r2(Vector3(), rot2);

	kulde.Trans(r2);

	int N = 256;

	Scene s(N, N);

	PointLight PL(Vector3(1, 1, 1), 0.1);
	PL.Trans(Transform(Vector3(-1, 1, 0)));
	s.lights.push_back(PL);

	s.clearBuffer();

	// for (int i = 0; i < kulde.size; i++) std::cout << i << " = " << kulde.tris[i].centroid().to_string() << "\n";

	s.fillMesh(kulde, rgb(255, 0, 0));

	std::ofstream outputFile("OUT");

	outputFile << s.to_string() << "\n";

	s.outputBuffer(BUFFER_PATH);

	outputFile.close();
}

// TEST 1.1 OBJ
inline void objtest() {
	std::string path = MESHES + "/mcrproto.obj";
	Mesh mesh = Mesh::fromOBJ(path);
	Transform movemeback(Vector3(0, -2, -2), Rotation3(Vector3(0, 1, 0), M_PI * 1.25));

	mesh.Trans(movemeback);

	/*
	
	Transform teapotoffset(Vector3(), Rotation3(Vector3(0, 1, 0), M_PI * 1.25));
	Transform offset2(Vector3(), Rotation3(Vector3(1, 0, 0), -M_PI / 2));
	Transform offset3(Vector3(0, -2, -2) * 2, Matrix3::eye());
	teapotoffset = offset3 * offset2 * teapotoffset;

	mesh.Trans(teapotoffset);

	*/

	std::cout << "Loaded" << mesh.size << "\n";

	int N = 1024;

	Scene s(N, N);

	std::cout << N << "\n";

	s.clearBuffer();

	for (int i = 0; i < 1; i++) {
		s.clearBuffer();
		s.drawMesh(mesh, rgb(255, 255, 255), true);
		s.fillMesh(mesh, rgb(255, 255, 255), true);

		std::cout << i << "\n";
	}

	

	std::cout << "Drawn\n";


	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 2 OMINA
inline void animtest() {
	Mesh omina = Mesh::fromOBJ(MESHES + "/cube.obj");

	Transform center(Vector3(-1, -1, -1) * 0.5, Rotation3(Vector3(1, 2, 1), M_PI / 8));

	omina.ForceTrans(center);

	Transform offset(Vector3(0, -2, -4) * 0.5, Matrix3::eye());
	Transform rotate(Vector3(), Rotation3(Vector3(1, 0, 0), M_PI / 4));
	omina.Trans(offset);

	int N = 512;

	Scene s(N, N);

	int LEN = 30;

	float ANG = 2 * M_PI / LEN;

	Transform smallrot(Vector3(), Rotation3(Vector3(0, 1, 0), ANG));

	for (int i = 0; i < LEN; i++) {
		std::cout << i << " ";
		s.clearBuffer();
		s.fillMesh(omina, rgb(255, 255, 255));

		Transform goback(omina.transform.origin, Matrix3::eye());

		omina.Trans(goback.inv());
		omina.Trans(smallrot);
		omina.Trans(goback);

		s.outputBuffer("video/frame" + std::to_string(i));
	}	

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// TEST 2.5 FILLED MESH ANIM
inline void animtest2() {
	Mesh omina = Mesh::fromOBJ(MESHES + "/mcrproto.obj");

	Transform offset(Vector3(0, -2, -3) * 0.5, Matrix3::eye());
	Transform rotate(Vector3(), Rotation3(Vector3(1, 0, 0), M_PI / 4));
	omina.Trans(offset);

	int N = 512;

	Scene s(N, N);

	int LEN = 45;
	float ANGLE = 2.0 / LEN;

	Transform smallrot(Vector3(), Rotation3(Vector3(0, 1, 0), M_PI * 2 / LEN));

	for (int i = 0; i < LEN; i++) {
		std::cout << i << " ";
		s.clearBuffer();
		s.fillMesh(omina, rgba(FullHue(i * ANGLE)));

		Transform goback(omina.transform.origin, Matrix3::eye());

		omina.Trans(goback.inv());
		omina.Trans(smallrot);
		omina.Trans(goback);

		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));
	}	

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// TEST 3 ANDAM
inline void multipletest() {
	Mesh andam = Mesh::fromOBJ(MESHES + "/cube.obj");

	float SSS = 0.25;

	Transform center(Vector3(-1, -1, -1) * 0.5, Matrix3::eye());

	andam.ForceTrans(center);

	Transform offset(Vector3(0, -4, -5) * SSS, Rotation3(Vector3(1, 1, 1), M_PI / 8));
	andam.Trans(offset);

	Mesh thing(andam);

	Transform offset2(Vector3(1, -7, -7) * SSS, Rotation3(Vector3(0, 1, 0), M_PI / 6));
	Transform offset3(Vector3(), Rotation3(Vector3(1, 0, 0), M_PI / 4));

	offset2 = offset3 * offset2;

	thing.Trans(offset2);

	Mesh proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");
	proto.ForceTrans(Transform(Vector3(-2, -2, -2), Rotation3(Vector3(0, 1, 0), M_PI * 1.2)));
	
	int N = 1024;

	Scene s(N, N);
	s.clearBuffer();

	auto start = std::chrono::high_resolution_clock::now();

	s.QueueMesh(andam, 0xFF0000FF, true);
	s.QueueMesh(thing, 0x00FF00FF);
	s.QueueMesh(proto, 0x0000FFFF);

	std::cout << "THING\n";

	s.drawQueue();

	auto finish = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()) / 1000000 << "ns\n";

	// s.outputFrags("OUT");

	s.outputBuffer(BUFFER_PATH);
}

// TEST 4 HALEY
// This is the same as TEST 3 ANDAM except with the camera rotated.
inline void camtest() {
	Mesh haley = Mesh::fromOBJ(MESHES + "/cube.obj");

	float SSS = 0.25;

	Transform center(Vector3(-1, -1, -1) * 0.5, Matrix3::eye());

	haley.ForceTrans(center);

	Transform offset(Vector3(0, -4, -5) * SSS, Rotation3(Vector3(1, 1, 1), M_PI / 8));
	haley.Trans(offset);

	Mesh thing(haley);

	Transform offset2(Vector3(1, -7, -7) * SSS, Rotation3(Vector3(0, 1, 0), M_PI / 6));
	Transform offset3(Vector3(), Rotation3(Vector3(1, 0, 0), M_PI / 4));

	offset2 = offset3 * offset2;

	thing.Trans(offset2);

	Mesh proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");
	proto.ForceTrans(Transform(Vector3(-2, -2, -2), Rotation3(Vector3(0, 1, 0), M_PI * 1.2)));
	
	int N = 1024;

	Scene s(N, N);

	float TURN = M_PI * 0.5;
	Transform movecam(Vector3(4, 0, -5), Rotation3(Vector3(0, 1, 0), TURN));

	s.camera.Trans(movecam);

	std::cout << s.camera.transform.matrix().sprintf() << "\n";
	std::cout << s.camera.transform.inv().matrix().sprintf() << "\n";

	s.clearBuffer();

	auto start = std::chrono::high_resolution_clock::now();

	s.QueueMesh(haley, 0xFF0000FF, true);
	s.QueueMesh(thing, 0x00FF00FF);
	s.QueueMesh(proto, 0x0000FFFF);

	std::cout << "THING\n";

	s.drawQueue();

	auto finish = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()) / 1000000 << "ns\n";

	// s.outputFrags("OUT");

	s.outputBuffer(BUFFER_PATH);
}

// TEST 5 LAO SHI
// This is similar to TEST 3 ANDAM except with a better light and usage of new shape methods to get premade meshes.
inline void lighttest() {
	Mesh lao = icosphere(1, 2);

	float SSS = 1;

	Transform offset(Vector3(0, -2, -4) * SSS, Rotation3(Vector3(1, 1, 1), M_PI / 8));
	lao.Trans(offset);

	Mesh shi(lao);

	shi.Trans(Transform(Vector3(2, 2, -4)));

	Mesh proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");
	proto.ForceTrans(Transform(Vector3(-2, -2, -2), Rotation3(Vector3(0, 1, 0), M_PI * 1.2)));
	
	int N = 1024;

	Scene s(N, N);

	PointLight PL(Vector3(1, 1, 1), 0.25);
	PL.Trans(Transform(Vector3(-4, 2, 0)));
	s.lights.push_back(PL);

	PointLight PL2(Vector3(0, 1, 1), 0.25);
	PL2.Trans(Transform(Vector3(4, -4, 1)));
	s.lights.push_back(PL2);

	s.clearBuffer();

	auto start = std::chrono::high_resolution_clock::now();

	s.fillMesh(lao, 0xFFFFFFFF, true);
	s.fillMesh(shi, 0xFFFFFFFF, false);
	// s.fillMesh(proto, 0xFFFFFFFF);

	std::cout << "THING\n";

	s.drawQueue();

	auto finish = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()) / 1000000 << "ns\n";

	// s.outputFrags("OUT");

	s.outputBuffer(BUFFER_PATH);
}


int main() {

	// basictest();

	// cubetest();

	// animtest();

	// objtest();

	// animtest2();

	// multipletest();

	// camtest();

	lighttest();
	return 0;
}