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
	for (int i = 0; i < 3; i++) tex[i][i] = col2;

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

// TEST 8 DARK
inline void morphtest() {
	Mesh test = Mesh::fromOBJ(MESHES + "/cubenomorph.obj");
	Mesh test2 = Mesh::fromOBJ(MESHES + "/cubemorph.obj");

	MorphedMesh darkdragon(test);

	darkdragon.copyTo(test2);



	std::cout << darkdragon.nstates << " " << darkdragon.nverts << " = " << darkdragon.size << "\n";

	Transform back(Vector3(0, -2, -4), Rotation3(Vector3(0, 1, 0), M_PI / 4));

	darkdragon.Trans(back);
	
	darkdragon.morph(std::vector<float>({1, 0}));

	int N = 512;

	Scene s(N, N);

	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	std::cout << "Loaded" << " " << darkdragon.uv.size() << "\n";

	s.clearBuffer();

	int LEN = 60;

	Transform rot(Vector3(), Rotation3(Vector3(0, 1, 0), M_PI * 2 / LEN));

	ImageTexture* mat = new ImageTexture(rgbcube2);

	for (int i = 0; i < LEN; i++) {
		float sine = 1 + cosf(i * M_PI * 2 / LEN);
		std::cout << i << " " << sine << "\n";
		darkdragon.morph({2 - sine, sine});

		Transform origin(darkdragon.transform.origin);
		darkdragon.Trans(origin.inv());
		// darkdragon.Trans(rot);
		darkdragon.Trans(origin);

		s.clearBuffer();
		s.fillMesh(darkdragon, mat, true, true);

		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));
	}

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();

	darkdragon.morphToState(1);

	std::cout << "Animated\n";

	s.clearBuffer();

	test.Trans(back);

	s.fillMesh(darkdragon, mat);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete mat;
}

// i'm running out of ideas for codenames from disneyxd let's switch to pbs kids
// TEST 8.1 ORD (Menger Sponge)
inline void mengersponge() {
	std::vector<Mesh> ord = MengerSponge(2, 4, true);

	std::cout << ord.size() << " Sponge\n";

	Transform back = Transform(Vector3(0, 0, -2));

	for (int i = 0; i < ord.size(); i++) {
		ord[i].Trans(back);
	}
	
	int N = 512;
	Scene s(N, N);

	PointLight PL(Vector3(1, 1, 1) * 0.5, 0);
	PL.Trans(Transform(Vector3(-2, 2, 2)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0.4);
	P2.Trans(Transform(back.origin));
	s.lights.push_back(P2);
	
	s.clearBuffer();

	ImageTexture* mat = new ImageTexture(rgbcube2);

	Transform rotabit = RotationAroundPoint(back.origin, Vector3(2, 1, -1), 0.8);

	for (auto i : ord) {
		i.Trans(rotabit);
		s.fillMesh(i, mat, true, true);
	}

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete mat;
}

// TEST 8.2 CASSIE (Texture map of the cube6)
inline void texmapref() {

	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);

	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	Mesh cassie = Mesh::fromOBJ(MESHES + "/cubenomorph.obj");
	cassie = cube6(1);

	Transform back(Vector3(0, -2, -4), Rotation3(Vector3(0, 1, 0), -0.4));

	std::cout << "Animated\n";

	s.clearBuffer();

	cassie.Trans(back);

	ImageTexture* mat = new ImageTexture(cubemap);

	s.fillMesh(cassie, mat, true, true);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete mat;
}

// TEST 9 ORD II (RAY TRACING)
inline void RTXTest() {

	int N = 512;
	int D = 0;

	RayTracer s(D, N, N);

	s.camera = Camera(M_PI / 2.0);

	PointLight PL(Vector3(1, 1, 1), 0.2);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	// s.lights.push_back(P2);

	Mesh ord = cube(1);
	Mesh ord2 = icosphere(0.5, 2);

	Transform back(Vector3(0, -2, -5), Rotation3(Vector3(0, 1, 0), -0.4));
	ord.Trans(back);

	Transform back2(Vector3(-1, -0.5, -3.5), Rotation3(Vector3(0, 1, 0), -0.4));
	ord2.Trans(back2);

	ImageTexture mat(cubemap);
	BaseMaterial red(0xFF000000, 64);
	BaseMaterial green(0x00FF0000, 64);

	s.addMesh(ord, red, false);
	s.addMesh(ord2, green, true);

	s.render(true);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

int main() {
	RTXTest();
	return 0;
}