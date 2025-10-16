#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

#include "texture_texture.h"

using namespace std;



#include "misctests.h"
#include "main_utils.h" // definitions of paths and some things



Transform rotatecam(Vector3(0, 1, 0), Rotation3(Vector3(1, 0, 0), M_PI * -0.2));

// Updated version of test 6
inline void phongtest() {
	int SUB = 2;
	Mesh lao = icosphere(1, SUB);


	float SSS = 1;

	Transform offset(Vector3(0, -2, -4) * SSS, Rotation3(Vector3(1, 1, 1), M_PI / 8));
	lao.Trans(offset);

	Mesh shi(lao);
	

	shi.Trans(Transform(Vector3(2, 2, -1)));
	

	Mesh proto = icosphere(1, SUB);
	// proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");
	proto = icosphere(1, SUB);
	proto.Trans(Transform(Vector3(-1, 0, -4), Rotation3(Vector3(0, 1, 0), M_PI * 1.2)));
	
	int N = 1024;

	Scene s(N, N / 2);

	float A = 0.3;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-4, 2, 2)));
	// s.lights.push_back(PL);

	PointLight PL2(Vector3(1, 1, 1), A);
	PL2.Trans(Transform(Vector3(4, 4, 0)));
	s.lights.push_back(PL2);

	s.clearBuffer();

	auto start = std::chrono::high_resolution_clock::now();

	int SPE = 16;

	

	s.fillMesh(lao, new BaseMaterial(0xFF0000FF, SPE));
	s.fillMesh(shi, new BaseMaterial(0x00FF00FF, SPE));
	s.fillMesh(proto, new BaseMaterial(0x0000FFFF, SPE), nullptr, false, false, false);

	std::cout << "THING\n";

	s.drawQueue();

	std::cout << "CNT " << s.TRIANGLE_COUNT << "\n";

	auto finish = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()) / 1000000 << "ns\n";

	// s.outputFrags("OUT");

	s.outputBuffer(BUFFER_PATH);
}

// TEST 8.2 CASSIE I (Texture map of the cube6, now also a test of triangle rasterization)
inline void texmapref() {

	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);
	s.camera.Trans(rotatecam);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	Mesh cassie = Mesh::fromOBJ(MESHES + "/cubemorph.obj");
	cassie = cube6(1);

	Transform back(Vector3(0, -2, -3.5), Rotation3(Vector3(0, 1, 0), -0.4));

	std::cout << "Animated\n";

	s.clearBuffer();

	cassie.Trans(back);

	ImageTexture* mat = new ImageTexture(cubemap);
	mat->specular = 64;

	BaseMaterial WHITE(BASEMAT_WHITE);

	s.drawMesh(cassie, &WHITE);
	s.fillMesh(cassie, mat, nullptr, true, true);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete mat;
}

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

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

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

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.2 ZAK (Raytracer Textures)
inline void RTexTest() {

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

	Mesh proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");
	Mesh zak(test2);

	// MorphedMesh zak(proto);
	// zak.copyTo(test2);

	// Mesh zak(test2);

	Mesh zak2 = cube(0.5);
	Mesh zak3 = cube(0.5);

	Transform back(Vector3(0, -1, -5), Rotation3(Vector3(0, 1, 0), -0.4));
	zak.Trans(back);

	Transform back2(Vector3(-0.5, -0.5, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	zak2.Trans(back2);

	Transform back3(Vector3(1, -1, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	zak3.Trans(back3);

	Mesh floor = GridSquare(64, 1);
	floor.Trans(Transform(Vector3(0, -2.5, 0)));

	float SP = 64;
	ImageTexture mat(cubemap);
	ImageTexture texproto(mcrproto); // use this on the proto mesh
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial green(0x00FF0000, SP, 1);
	BaseMaterial blue(0x0000FF00, SP, 1);
	BaseMaterial cyan(0x00FFFF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	s.addMesh(&zak, &mat, false);
	s.addMesh(&zak2, &red, false);
	s.addMesh(&zak3, &cyan, false);
	s.addMesh(&floor, &white, false);
	
	// s.morph(0, std::vector<float>{0, 1});

	s.DEPTH = 2;

	std::cout << "Prepared\n";

	s.render(true);

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.3 WHEEZIE (Raytracer Textures + Morphed Mesh + BVH)
inline void RTexBVH() {
	int N = 512;
	int D = 0;

	RayTracer s(D, N, N);

	s.camera = Camera(M_PI / 2.0);
	s.camera.Trans(Transform(Vector3(0, 0, -1)));

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

	Mesh proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");

	MorphedMesh wheezie(test);
	wheezie.copyTo(test2);

	Mesh cube1 = cube(0.5);
	Mesh cube2 = cube(0.5);

	Transform back(Vector3(0, -1, -5), Rotation3(Vector3(0, 1, 0), -0.4 + M_PI));
	wheezie.Trans(back);

	Transform back2(Vector3(-0.5, -0.5, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube1.Trans(back2);

	Transform back3(Vector3(1, -1, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube2.Trans(back3);

	Transform back4(Vector3(0, -0.4, -5), Rotation3(Vector3(0, 1, 0), M_PI * 0.9));
	proto.Trans(back4);

	Mesh floor = GridSquare(64, 1);
	floor.Trans(Transform(Vector3(0, -2.5, 0)));

	float SP = 64;
	ImageTexture mat(cubemap);
	ImageTexture texproto(mcrproto); // use this on the proto mesh
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial green(0x00FF0000, SP, 1);
	BaseMaterial blue(0x0000FF00, SP, 1);
	BaseMaterial cyan(0x00FFFF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	s.addMesh(&wheezie, &mat, false);
	s.addMesh(&cube1, &red, false);
	s.addMesh(&cube2, &cyan, false);
	s.addMesh(&floor, &white, false);
	s.addMesh(&proto, &texproto, false);
	
	s.morph(0, std::vector<float>{0, 1});

	s.DEPTH = 1;

	std::cout << "Prepared\n";

	s.UseBVH = true; // feel free to toggle this if uncertain

	s.render(true);

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	// Animation example
	return; // REMOVE ME IF YOU WANT ANIMATIONS.

	int LEN = 24;

	for (int i = 0; i < LEN; i++) {
		float DISP = 0.5 * (1 + cosf(float(i) * 2 * M_PI / LEN));
		s.morph(0, std::vector<float>({1 - DISP, DISP}));



		Transform protoorigin = s.meshes[4]->transform.inv();
		Transform ppp = s.meshes[4]->transform;
		Transform protoback(Vector3(0, -0.4 * DISP, -5), Rotation3(Vector3(0, 1, 0), M_PI * 0.9 + 0.25 * DISP));
		s.meshes[4]->Trans(protoback * protoorigin);
		

		s.render();

		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// Bounding Box Test
inline void BoxTest() {
	AABB aabb(Vector3(0, 0, 0), Vector3(1, 2, 3));

	Vector3 point(2, 1, 1);
	Vector3 end(2, 4, 1);

	Ray r(point, end - point);
	std::cout << aabb.IntersectRay(r);
}

// Simple BVH Test with Few Objects
inline void BVHSimpleObject() {
	int N = 512;
	int D = 0;

	RayTracer s(D, N, N);

	s.camera = Camera(M_PI / 2.0);

	float A = 0.1;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-1, 2, 0)));
	// s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), A);
	P2.Trans(Transform(Vector3(1, -2, 0)));
	s.lights.push_back(P2);

	int S = 2;

	Triangle3* wheezie = new Triangle3[4];
	wheezie[0] = Triangle3(Vector3(0, 0, -3), Vector3(1, 0, -2), Vector3(0, 1, -2));
	wheezie[1] = Triangle3(Vector3(0, 0, -3), Vector3(-1, 0, -2), Vector3(0, -1, -2));
	wheezie[2] = Triangle3(Vector3(0, 0, -3), Vector3(1, 0, -2), Vector3(0, -1, -2));
	wheezie[3] = Triangle3(Vector3(0, 0, -3), Vector3(-1, 0, -2), Vector3(0, 1, -2));

	wheezie[2].flipNormal();
	wheezie[3].flipNormal();

	Mesh* m = new Mesh(wheezie, 4);
	BaseMaterial mat(BASEMAT_WHITE);

	std::cout << "Mesh created\n";
	s.addMesh(m, &mat);

	s.UseBVH = true;

	s.DEPTH = 2;

	std::cout << "Prepared\n";

	s.render(true);

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.4A QUETZAL (BVH Test With More Triangles)
inline void BVHTest() {
	
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

	Mesh test = Mesh::fromOBJ(MESHES + "/cubenomorph.obj");
	Mesh test2 = Mesh::fromOBJ(MESHES + "/cubemorph.obj");

	MorphedMesh quetzal(test);
	quetzal.copyTo(test2);

	Mesh quetzal2 = cube(0.5);
	Mesh quetzal3 = cube(0.5);

	int S = 2;
	
	quetzal2 = icosphere(0.5, S); // each sphere has 20 * 4^4 = 5120 triangles at 4 levels of subdivision.
	quetzal3 = icosphere(0.5, S); // or 20 * 4^2 = 320 triangles at 2 levels of subdivision.

	Transform back(Vector3(0, -1, -5), Rotation3(Vector3(0, 1, 0), -0.4));
	quetzal.Trans(back);

	Transform back2(Vector3(-0.5, -0.5, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	quetzal2.Trans(back2);

	Transform back3(Vector3(1, -1, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	quetzal3.Trans(back3);

	Mesh floor = GridSquare(64, 4);
	floor.Trans(Transform(Vector3(0, -2.5, 0)));

	float SP = 128;
	ImageTexture mat(cubemap);
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial green(0x00FF0000, SP, 1);
	BaseMaterial blue(0x0000FF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	s.addMesh(&quetzal, &red, false);
	s.addMesh(&quetzal2, &green, true);
	s.addMesh(&quetzal3, &blue, true);
	s.addMesh(&floor, &white, false);
	
	s.morph(0, std::vector<float>{0, 1});

	/*

	BVH bvh = create(s.meshes, s.materials, s.NormInterps);

	auto vv = bvh.traverse();
	std::cout << "VV " << vv.size() << "\n";
	std::cout << "ROOT " << bvh.root->size() << "\n";
	for (auto i : vv) {
		if (i->isObject()) std::cout << i->obj.tri << " ";
	}

	*/

	s.UseBVH = true;

	s.DEPTH = 2;

	std::cout << "Prepared\n";

	s.render(true);

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.4B Menger Sponge (BVH Test With More Triangles)
inline void BVHSponge() {
	
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

	auto spong = MengerSponge(2, 3);

	int S = 2;

	Transform back(Vector3(0, 0, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	for (int i = 0; i < spong.size(); i++) spong[i].Trans(back);

	Mesh floor = GridSquare(64, 4);
	floor.Trans(Transform(Vector3(0, -2.5, 0)));

	float SP = 128;
	ImageTexture mat(cubemap);
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial cyan(0x00FFFF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	for (auto i : spong) s.addMesh(&i, &red, false);
	s.addMesh(&floor, &cyan, false);

	s.UseBVH = true;

	s.DEPTH = 2;

	std::cout << "Prepared\n";

	s.render(true);

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.4C Stanford Models (BVH Test With More Triangles)
inline void BVHStanford() {
	
	int N = 128;
	int D = 1;

	RayTracer s(D, N, N);

	s.camera = Camera(M_PI / 2.0);

	float A = 0.1;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	// s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), A);
	P2.Trans(Transform(Vector3(0, 2, 0)));
	s.lights.push_back(P2);

	auto model = Mesh::fromOBJ(MESHES + "/" + "stanford-bunny.obj");

	int S = 2;

	float X = 8;
	Transform theScale(Matrix4(X, 0, 0, 0, 0, X, 0, 0, 0, 0, X, 0, 0, 0, 0, 1));

	Transform back(Vector3(0, -0.8, -1.2), Rotation3(Vector3(0, 1, 0), -0.4));
	model.Trans(theScale);

	model.Trans(back);

	Mesh floor = GridSquare(64, 4);
	floor.Trans(Transform(Vector3(0, -2.5, 0)));

	float SP = 128;
	ImageTexture mat(cubemap);
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial cyan(0x00FFFF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	s.addMesh(&model, &red, false);
	s.addMesh(&floor, &cyan, false);

	s.UseBVH = true;

	s.DEPTH = 0;

	std::cout << "Prepared " << s.countTriangles() << " Triangles\n";

	auto start = std::chrono::high_resolution_clock::now();

	s.createBVH();

	auto end = std::chrono::high_resolution_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	std::cout << dur.count() << " uS TO CREATE BVH\n";

	s.render(true);

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.5A PRISM (Subdivision I)
inline void Subdiv1() {

	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	Mesh prism = Mesh::fromOBJ(MESHES + "/cubemorph.obj");
	prism = subdivide(prism, 2);

	Transform back(Vector3(0, -2, -4), Rotation3(Vector3(0, 1, 0), 0.7));

	std::cout << "Animated\n";

	s.clearBuffer();

	prism.Trans(back);

	ImageTexture* mat = new ImageTexture(cubemap);
	mat->specular = 64;

	s.fillMesh(prism, mat, nullptr);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete mat;
}

// TEST 9.5B NEXIE (Subdivision II)
inline void Subdiv2() {

	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	Mesh nexie = Mesh::fromOBJ(MESHES + "/cubemorph.obj");
	nexie = subdivideLoop(nexie, 1);

	Transform back(Vector3(0, -2, -4), Rotation3(Vector3(0, 1, 0), -M_PI * 0.9));

	std::cout << "Animated\n";

	s.clearBuffer();

	nexie.Trans(back);

	BaseMaterial* mat = new BaseMaterial(Vector3(0, 1, 0));
	mat->specular = 64;

	ImageTexture* imgtex = new ImageTexture(cubemap);
	imgtex->specular = 64;

	s.fillMesh(nexie, imgtex);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete mat;
	delete imgtex;
}

// TEST 9.5C AZION (Bezier Curves)
inline void BezierMeshTest() {
	
	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	// s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0.25);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	std::vector<std::vector<Vector3>> controls({
		{Vector3(0, -1.5, 0), Vector3(1, 0, 0), Vector3(2, 0, 0), Vector3(3, 0, 0)},
		{Vector3(0, -1, 1), Vector3(1, 0, 1), Vector3(2, 0, 1), Vector3(3, 0.5, 1)},
		{Vector3(0, -0.5, 2), Vector3(1, 0, 2), Vector3(2, 0, 2), Vector3(3, 1, 2)},
		{Vector3(0, 0, 3), Vector3(1, 0, 3), Vector3(2, 0, 3), Vector3(3, 1.5, 3)}
	});

	for (int i = 0; i < controls.size(); i++) {
		for (int j = 0; j < controls[i].size(); j++) controls[i][j] = controls[i][j] - Vector3(1.5, 0, 1.5);
	}
	BezierSurface surface(controls);

	int I = 32;
	Mesh azion = surface.render(I, I);
	Mesh azion2 = surface.ctrlmesh();

	azion.flipNormals();

	Transform back(Vector3(0, -1.5, -5), Rotation3(Vector3(0, 1, 0), -M_PI));

	s.clearBuffer();

	azion.Trans(back);
	azion2.Trans(back);

	Vector3 col(0, 0.7, 1);

	BaseMaterial mat = BaseMaterial(col);
	mat.specular = 64;

	BaseMaterial white(BASEMAT_WHITE);
	white.specular = 64;

	int TD = 8;
	std::vector<std::vector<uint32_t>> checkerboard(TD, std::vector<uint32_t>(TD));
	for (int i = 0; i < TD; i++) {
		for (int j = 0; j < TD; j++) {
			checkerboard[i][j] = 0x000000FF;
			if ((i + j) % 2 == 0) checkerboard[i][j] = rgb(col);
		}
	}
	ImageTexture imgtex = ImageTexture(checkerboard);
	imgtex.baseColor = Vector3(col); // in case you want to use drawMesh instead of fillMesh
	imgtex.specular = 64;

	s.fillMesh(azion, &imgtex);
	s.drawMesh(azion2, &white);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.5C AZION (Bezier Curves) - RayTracing
inline void BezierMeshRTX() {
	
	int N = 512;
	int D = 2;

	RayTracer s(D, N, N);

	s.camera = Camera(M_PI / 2.0);

	float A = 0.2;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), A);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	std::vector<std::vector<Vector3>> controls({
		{Vector3(0, -1.5, 0), Vector3(1, 0, 0), Vector3(2, 0, 0), Vector3(3, 0, 0)},
		{Vector3(0, -1, 1), Vector3(1, 0, 1), Vector3(2, 0, 1), Vector3(3, 0.5, 1)},
		{Vector3(0, -0.5, 2), Vector3(1, 0, 2), Vector3(2, 0, 2), Vector3(3, 1, 2)},
		{Vector3(0, 0, 3), Vector3(1, 0, 3), Vector3(2, 0, 3), Vector3(3, 1.5, 3)}
	});

	for (int i = 0; i < controls.size(); i++) {
		for (int j = 0; j < controls[i].size(); j++) controls[i][j] = controls[i][j] - Vector3(1.5, 0, 1.5);
	}
	BezierSurface surface(controls);

	int I = 16;
	Mesh azion = surface.render(I, I);
	
	Mesh azion2(azion);
	azion.flipNormals();

	Transform back(Vector3(0, -1.5, -5), Rotation3(Vector3(0, 1, 0), M_PI));


	s.clearBuffer();

	azion.Trans(back);
	azion2.Trans(Transform(Vector3(0, -0.1, 0)) * back);

	Vector3 col(0, 0.7, 1);

	float SP = 64;
	BaseMaterial mat = BaseMaterial(col, SP, 1);
	BaseMaterial grey(0x808080FF, SP, 1);

	int TD = 8;
	std::vector<std::vector<uint32_t>> checkerboard(TD, std::vector<uint32_t>(TD));
	for (int i = 0; i < TD; i++) {
		for (int j = 0; j < TD; j++) {
			checkerboard[i][j] = 0x000000FF;
			if ((i + j) % 2 == 0) checkerboard[i][j] = rgb(col);
		}
	}
	ImageTexture imgtex = ImageTexture(checkerboard);
	imgtex.baseColor = Vector3(col); // in case you want to use drawMesh instead of fillMesh
	imgtex.specular = 64;



	Mesh floor = GridSquare(64, 1);
	floor.Trans(Transform(Vector3(0, -4, 0)));

	s.addMesh(&azion, &imgtex, true);
	s.addMesh(&azion2, &imgtex, true);
	s.addMesh(&floor, &grey, false);


	s.UseBVH = true;
	s.DEPTH = 3;

	s.render(true, 0);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.6A PIXEL (Quadmesh)
inline void QuadMeshTest() {
	
	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	int I = 32;
	QuadMesh qm = QuadMesh::fromOBJ(MESHES + "/simplequad.obj");
	Mesh pixel = qm.convert();

	// quadmesh = cube(2);

	Transform back(Vector3(0, -3, -7), Rotation3(Vector3(0, 1, 0), -M_PI * 0.75));
	s.clearBuffer();

	pixel.Trans(back);

	Vector3 col(1, 1, 1);

	BaseMaterial mat = BaseMaterial(col);
	mat.specular = 64;

	BaseMaterial white(BASEMAT_WHITE);
	white.specular = 64;

	uint32_t R = 0xFF0000FF;
	uint32_t G = 0x00FF00FF;
	uint32_t B = 0x0000FFFF;
	uint32_t K = 0x000000FF;

	std::vector<std::vector<uint32_t>> checkerboard({
		{B, R, K, K, G}, 
		{R, R, R, G, G}, 
		{G, G, R, B, B}, 
		{K, K, K, K, B},
		{K, G, R, B, B}
	});


	ImageTexture imgtex = ImageTexture(simplequadRGB2);
	imgtex.baseColor = Vector3(col); // in case you want to use drawMesh instead of fillMesh
	imgtex.specular = 64;

	s.fillMesh(pixel, &imgtex);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.6B PLUSH (Quadmesh subdivision)
inline void SubdivideCCTest() {
	
	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	int I = 32;
	QuadMesh qm = QuadMesh::fromOBJ(MESHES + "/simplequad.obj");
	qm = subdivideCC(qm, 4);
	Mesh plush = qm.convert();

	Transform back(Vector3(0, -3, -7), Rotation3(Vector3(0, 1, 0), -M_PI * 0.75));
	s.clearBuffer();

	plush.Trans(back);

	Vector3 col(1, 1, 1);

	BaseMaterial mat = BaseMaterial(col);
	mat.specular = 64;

	BaseMaterial white(BASEMAT_WHITE);
	white.specular = 64;

	uint32_t R = 0xFF0000FF;
	uint32_t G = 0x00FF00FF;
	uint32_t B = 0x0000FFFF;
	uint32_t K = 0x000000FF;


	ImageTexture imgtex = ImageTexture(simplequadRGB2);
	imgtex.baseColor = Vector3(col); // in case you want to use drawMesh instead of fillMesh
	imgtex.specular = 64;

	s.fillMesh(plush, &imgtex);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.6A-B PIXEL/PLUSH (Quadmesh subdiv and RTX)
inline void QuadRTX() {
	
	int N = 512;

	RayTracer s(0, N, N);

	s.camera = Camera(M_PI / 2.0);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	int I = 32;
	QuadMesh qm = QuadMesh::fromOBJ(MESHES + "/simplequad.obj");
	// qm = subdivideCC(qm, 2);
	Mesh plush = qm.convert();

	Transform back(Vector3(0, -3, -7), Rotation3(Vector3(0, 1, 0), -M_PI * 0.75));

	plush.Trans(back);

	Vector3 col(1, 1, 1);

	BaseMaterial mat = BaseMaterial(col);
	mat.specular = 64;

	BaseMaterial white(BASEMAT_WHITE);
	white.specular = 64;

	uint32_t R = 0xFF0000FF;
	uint32_t G = 0x00FF00FF;
	uint32_t B = 0x0000FFFF;
	uint32_t K = 0x000000FF;

	ImageTexture imgtex = ImageTexture(simplequadRGB2);
	imgtex.baseColor = Vector3(col); // in case you want to use drawMesh instead of fillMesh
	imgtex.specular = 64;
	imgtex.reflective = 1;

	Mesh floor = GridSquare(64, 1);
	floor.Trans(Transform(Vector3(0, -4, 0)));

	BaseMaterial grey(0x808080FF, 64, 1);
	s.addMesh(&floor, &grey, false);


	s.addMesh(&plush, &imgtex, false);


	s.UseBVH = true;
	s.DEPTH = 2;
	s.render();

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// Triangle Clipping Test
inline void TriClip() {
	Triangle3 t(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0));
	Plane p(2, -1, 0, -1);

	auto res = TriSplit(t, p);

	std::cout << "POSITIVE SIDE\n";
	for (auto i : res.first) std::cout << i.to_string() << "\n";

	std::cout << "NEGATIVE SIDE\n";
	for (auto i : res.second) std::cout << i.to_string() << "\n";
}

// Attribute Clipping Test
inline void TriClipAttr() {
	Triangle3 t(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0));
	std::vector<Vector2> uv({Vector2(0, 0), Vector2(1, 0), Vector2(0, 1)});
	Plane p(2, -1, 0, -1);

	auto res = TriSplitAttr(t, p, uv);

	std::cout << "POSITIVE SIDE\n";
	for (auto i : res.first) {
		std::cout << "[ ";
		for (auto j : i) std::cout << j.to_string() << " ";
		std::cout << "]\n";
	}

	std::cout << "NEGATIVE SIDE\n";
	for (auto i : res.second) {
		std::cout << "[ ";
		for (auto j : i) std::cout << j.to_string() << " ";
		std::cout << "]\n";
	}
}

// TEST 9.7A VALLEY (Clipping. In addition to edge clipping i.e. off the view frustum we also artificially clip a plane through the model.)
inline void ClipTest() {

	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0, 0.5, 1024);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	// s.lights.push_back(P2);
	
	BaseMaterial mat(BASEMAT_WHITE);

	int LEN = 4;
	int R = 2;

	// Remember that a Menger sponge is a collection of models.
	auto valley = MengerSponge(LEN, R);
	auto v2 = MengerSponge(LEN, R);
	for (auto i : v2) {
		Mesh m(i);
		m.flipNormals();
		valley.push_back(m); // This allows us to see inside cubes that are cut open
	}

	std::vector<Triangle3> valley2;

	Vector3 po(0, -LEN * 0.25, -LEN * 1.5);
	Transform back(po, Rotation3(Vector3(0, 1, 0), -M_PI * 0.25));

	for (int i = 0; i < valley.size(); i++) valley[i].Trans(back);

	Plane plane(po, Vector3(0, 1, 1) * -1);

	for (auto mesh : valley) {
		for (int i = 0; i < mesh.size; i++) {
			Triangle3 tri = mesh.makeTriangle(i); 
			for (auto p : TriSplit(tri, plane).first) valley2.push_back(Triangle3(p)); // Add all triangles on the positive side of the splitting plane.
		}
	}

	Mesh m2(valley2);

	s.clearBuffer();

	// for (auto i : valley) s.fillMesh(i, &mat, true, true);
	s.fillMesh(m2, &mat);


	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.7B LUNAR ABYSS (Better clipping showcase.)
inline void MengerSpongeDual() {

	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0, 0.5, 1024);



	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	// s.lights.push_back(P2);
	
	BaseMaterial mat(BASEMAT_WHITE);

	int LEN = 4;
	int R = 3;

	// Remember that a Menger sponge is a collection of models.
	auto sponge = MengerSponge(LEN, R);

	Plane plane(Vector3(0, 0, 0), Vector3(3, 1, 0));
	Plane plan2(Vector3(0, 0, 0), Vector3(1, -3, 0));

	std::vector<Triangle3> a, b, c, d, pos, neg;
	
	// Two way split
	for (auto mesh : sponge) {
		for (int i = 0; i < mesh.size; i++) {
			Triangle3 tri = mesh.makeTriangle(i); 
			auto split = TriSplit(tri, plane);
			for (auto p : split.first) pos.push_back(Triangle3(p)); // Add all triangles on the positive side of the splitting plane.
			for (auto p : split.second) neg.push_back(Triangle3(p)); // Add all triangles on the positive side of the splitting plane.
		}
	}

	// Four way split
	for (auto mesh : sponge) {
		for (int i = 0; i < mesh.size; i++) {
			Triangle3 tri = mesh.makeTriangle(i); 
			auto split = TriSplit(tri, plane);
			for (auto p : split.first) {
				auto sp2 = TriSplit(p, plan2);
				for (auto pp : sp2.first) a.push_back(Triangle3(pp));	
				for (auto pp : sp2.second) b.push_back(Triangle3(pp));	
			}
			for (auto p : split.second) {
				auto sp2 = TriSplit(p, plan2);
				for (auto pp : sp2.first) c.push_back(Triangle3(pp));	
				for (auto pp : sp2.second) d.push_back(Triangle3(pp));	
			}
		}
	}

	BaseMaterial CYAN(BASEMAT_CYAN);
	BaseMaterial WHITE(BASEMAT_WHITE);

	Mesh abyss(pos);
	Mesh irrid(neg);

	Mesh A(a);
	Mesh B(b);
	Mesh C(c);
	Mesh D(d);

	Vector3 po(0, -LEN * 0.25, -LEN * 1.5);
	Transform back(po, Rotation3(Vector3(0, 1, 0), -M_PI * 0.1));

	abyss.Trans(back);
	irrid.Trans(back);

	A.Trans(back);
	B.Trans(back);
	C.Trans(back);
	D.Trans(back);

	s.clearBuffer();

	
	// for (auto i : valley) s.fillMesh(i, &mat, true, true);

	const bool twoway = true;
	if (twoway) {
		s.fillMesh(abyss, &CYAN);
		s.fillMesh(irrid, &WHITE);
	} else {
		s.fillMesh(A, &CYAN);
		s.fillMesh(B, &WHITE);
		s.fillMesh(C, &WHITE);
		s.fillMesh(D, &CYAN);
	}


	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// Cubic B-Spline
inline void BSPTest() {
	CubicBSpline spline(std::vector<Vector3>{Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(1, 1, 0), Vector3(1, 0, 0), Vector3(2, 0, 0)});

	for (float i = 0; i <= 1; i += 0.01) std::cout << i << " " << spline.query(i).to_string() << "\n";

	std::cout << spline.to_string() << "\n";
}

// TEST 9.8A AZION II (B-Spline Patch)
inline void BSPMeshTest() {
	
	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);

	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	// s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0.25);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);

	std::vector<std::vector<Vector3>> controls({
		{Vector3(0, -2, 0), Vector3(1, -1.5, 0), Vector3(2, -1, 0), Vector3(3, -0.5, 0), Vector3(4, 0, 0)},
		{Vector3(0, -1.5, 1), Vector3(1, 0, 1), Vector3(2, 0, 1), Vector3(3, 0, 1), Vector3(4, 0.5, 1)},
		{Vector3(0, -1, 2), Vector3(1, 0, 2), Vector3(2, -1, 2), Vector3(3, 0, 2), Vector3(4, 1, 2)},
		{Vector3(0, -0.5, 3), Vector3(1, 0, 3), Vector3(2, 0, 3), Vector3(3, 0, 3), Vector3(4, 1.5, 3)},
		{Vector3(0, 0, 4), Vector3(1, 0.5, 4), Vector3(2, 1, 4), Vector3(3, 1.5, 4), Vector3(4, 2, 4)}
		
	});

	float thing = (controls[0].size() - 1) * 0.5;
	for (int i = 0; i < controls.size(); i++) {
		for (int j = 0; j < controls[i].size(); j++) controls[i][j] = controls[i][j] - Vector3(thing, 0, thing);
	}
	CubicBSplineSurface surface(controls);

	int I = 32;
	Mesh azion = surface.render(I, I);
	Mesh azion2 = surface.ctrlmesh();

	azion.flipNormals();

	Transform back(Vector3(0, -1.5, -5), Rotation3(Vector3(0, 1, 0), -M_PI));

	s.clearBuffer();

	azion.Trans(back);
	azion2.Trans(back);

	Vector3 col(0, 0.7, 1);

	BaseMaterial mat = BaseMaterial(col);
	mat.specular = 64;

	BaseMaterial white(BASEMAT_WHITE);
	white.specular = 64;

	int TD = 8;
	std::vector<std::vector<uint32_t>> checkerboard(TD, std::vector<uint32_t>(TD));
	for (int i = 0; i < TD; i++) {
		for (int j = 0; j < TD; j++) {
			checkerboard[i][j] = 0x000000FF;
			if ((i + j) % 2 == 0) checkerboard[i][j] = rgb(col);
		}
	}
	ImageTexture imgtex = ImageTexture(checkerboard);
	imgtex.baseColor = Vector3(col); // in case you want to use drawMesh instead of fillMesh
	imgtex.specular = 64;

	s.fillMesh(azion, &imgtex, nullptr, true, true, true);
	s.drawMesh(azion2, &white);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.8B PLUSH II (Subdivision with Sharpness)
inline void SubdivideCCSharp() {
	
	int N = 512;

	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);

	PointLight PL(Vector3(1, 1, 1), 0);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), 0);
	P2.Trans(Transform(Vector3(2, 2, -2)));
	s.lights.push_back(P2);



	// Notice the sharp mesh designation
	SharpQuadMesh qm(QuadMesh::fromOBJ(MESHES + "/simplequad.obj"));

	Vector3 panel[4] = {Vector3(1, 3, 1), Vector3(-1, 3, 1), Vector3(-1, 3, -1), Vector3(1, 3, -1)};
	Vector3 panel2[4] = {Vector3(3, 1, 1), Vector3(3, -1, 1), Vector3(3, -1, -1), Vector3(3, 1, -1)};
	Vector3 panel3[4] = {Vector3(1, 1, -3), Vector3(-1, 1, -3), Vector3(-1, -1, -3), Vector3(1, -1, -3)};

	for (int i = 0; i < 4; i++) { // Mark certain edges and vertices as sharp. These get transformed with the mesh when you transform it.

		// x axis: sharp edges only
		qm.shedges.insert({panel2[i], panel2[(i + 1) % 4]});

		// y axis: sharp vertices only
		qm.shverts.insert(panel[i]);

		// z axis: sharp edges and vertices
		qm.shverts.insert(panel3[i]);
		qm.shedges.insert({panel3[i], panel3[(i + 1) % 4]});
	}

	qm = subdivideCC(qm, 4); // Subdivide <-- THIS IS TO BE CHANGED DURING DEMO
	Mesh plush = qm.convert();

	std::cout << plush.size << " TRIANGLES\n";

	Transform back(Vector3(0, -3, -7), Rotation3(Vector3(0, 1, 0), -M_PI * 0.75));
	s.clearBuffer();

	plush.Trans(back);

	Vector3 col(1, 1, 1);

	BaseMaterial mat = BaseMaterial(col);
	mat.specular = 64;

	BaseMaterial white(BASEMAT_WHITE);
	white.specular = 64;

	uint32_t R = 0xFF0000FF;
	uint32_t G = 0x00FF00FF;
	uint32_t B = 0x0000FFFF;
	uint32_t K = 0x000000FF;


	ImageTexture imgtex = ImageTexture(simplequadRGB2);
	imgtex.baseColor = Vector3(col); // in case you want to use drawMesh instead of fillMesh
	imgtex.specular = 64;

	s.fillMesh(plush, &imgtex);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.81 ??????????? I (Add mesh to rasterized scene in the same way you use RayTracer)
// This uses the same meshes as 9.3.
inline void FormalRaster() {
	Scene s = scene_protocubes();

	std::cout << "Prepared\n";

	// s.UseBVH = true; // Do not enable if s is a Scene and not a RayTracer
	s.render(true);

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	// Animation example
	return; // REMOVE ME IF YOU WANT ANIMATIONS.

	int LEN = 24;

	for (int i = 0; i < LEN; i++) {
		float DISP = 0.5 * (1 + cosf(float(i) * 2 * M_PI / LEN));
		s.morph(0, std::vector<float>({1 - DISP, DISP}));



		Transform protoorigin = s.meshes[4]->transform.inv();
		Transform ppp = s.meshes[4]->transform;
		Transform protoback(Vector3(0, -0.4 * DISP, -5), Rotation3(Vector3(0, 1, 0), M_PI * 0.9 + 0.25 * DISP));
		s.meshes[4]->Trans(protoback * protoorigin);
		

		s.render();

		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// TEST 9.82 PROTOTRACER II (Animation Sanity Check)
// This uses the same meshes as 9.3.
inline void AnimSan() {
	Scene s = scene_protocubes();

	std::cout << "Prepared\n";
	for (auto i : s.names) std::cout << i << " ";
	std::cout << "\n";
	// Animation example
	Animation animation(&s);

	// Red cube moves in a square
	animation.setTranslation(1, 12, Vector3(-1.5, -0.5, -3), ANIMI::SINE);
	animation.setTranslation(1, 24, Vector3(-1.5, 0.5, -3));
	animation.setTranslation(1, 36, Vector3(-0.5, 0.5, -3));
	animation.setTranslation(1, 48, Vector3(-0.5, -0.5, -3));
	for (int i = 0; i < animation.getLength(); i++) {
		animation.animate(i);
		s.render();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));
		std::cout << "FRAME " << i << "DONE\n";
	}
	animation.encodeLength(VIDEO_PATH + "/LEN");
	std::cout << "Stored\n";

}

// TEST 9.83A Quaternion Fixes
inline void QuaternionTest() {
	Matrix3 rot = Rotation3(Vector3(2, 10, 11), 0.69);
	Quaternion qq(rot);
	std::cout << rot.sprintf() << "\n" << qq.to_string() << "\n--\n";

	Matrix3 rot2 = qq.toRotation();
	std::cout << rot2.sprintf() << "\n" << Quaternion(rot2).to_string() << "\n";
}

// TEST 9.83B PROTOTRACER III (Rotation and Morph Animations)
// This uses the same meshes as 9.3.
inline void FullAnimation() {
	Scene s = scene_protocubes();

	std::cout << "Prepared\n";
	for (auto i : s.names) std::cout << i << " ";
	std::cout << "\n";
	// Animation example
	Animation animation(&s);

	// Red cube moves in a square and spins
	animation.setTranslation(1, 12, Vector3(-1.5, -0.5, -3), ANIMI::SINE);
	animation.setRotation(1, 12, QuaternionAA(Vector3(0, 1, 0), -0.4 + M_PI / 2));
	animation.setTranslation(1, 24, Vector3(-1.5, 0.5, -3));
	animation.setRotation(1, 24, QuaternionAA(Vector3(0, 1, 0), -0.4 + M_PI));
	animation.setTranslation(1, 36, Vector3(-0.5, 0.5, -3));
	animation.setRotation(1, 36, QuaternionAA(Vector3(0, 1, 0), -0.4 + M_PI * 1.5));
	animation.setTranslation(1, 48, Vector3(-0.5, -0.5, -3));
	animation.setRotation(1, 48, QuaternionAA(Vector3(0, 1, 0), -0.4));

	// Protogen standing on the cube moves up and down, and swings
	animation.setTranslation(4, 24, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 24, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 48, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 48, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));

	// Large cube morphs up and down
	animation.setMorph(0, 0, ket(2, 0));
	animation.setMorph(0, 24, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 48, ket(2, 0), ANIMI::SINE_OUT);

	if (true) {
			// Protogen standing on the cube moves up and down, and swings
	animation.setTranslation(4, 8, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 8, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 16, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 16, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));
	animation.setTranslation(4, 24, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 24, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 32, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 32, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));
	animation.setTranslation(4, 40, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 40, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 48, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 48, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));

	// Large cube morphs up and down
	animation.setMorph(0, 0, ket(2, 0));
	animation.setMorph(0, 8, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 16, ket(2, 0), ANIMI::SINE_OUT);
	animation.setMorph(0, 24, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 32, ket(2, 0), ANIMI::SINE_OUT);
	animation.setMorph(0, 40, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 48, ket(2, 0), ANIMI::SINE_OUT);
	}

	for (int i = 0; i < animation.getLength(); i++) {
		animation.animate(i);
		s.render();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));
		std::cout << "FRAME " << i << "DONE\n";
	}
	animation.encodeLength(VIDEO_PATH + "/LEN");
	std::cout << "Stored\n";
}

// TEST 9.83C PROTOTRACER IV (Multiple cameras)
inline void MulticamTest() {
	int N = 512;
	int D = 1;

	// RayTracer s(D, N, N);
	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);
	s.camera.Trans(Transform(Vector3(0, 0, -1)));
	s.cameras[0] = s.camera;

	Camera camera2(M_PI / 2.0);
	camera2.Trans(Transform(Vector3(4, 0, -4), Rotation3(Vector3(0, 1, 0), M_PI * 0.5)));
	s.cameras.push_back(camera2);

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

	Mesh proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");

	MorphedMesh dragon(test);
	dragon.copyTo(test2);

	Mesh cube1 = cube(0.5);
	Mesh cube2 = cube(0.5);

	Transform back(Vector3(0, -1, -5), Rotation3(Vector3(0, 1, 0), -0.4 + M_PI));
	dragon.Trans(back);

	Transform back2(Vector3(-0.5, -0.5, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube1.Trans(back2);

	Transform back3(Vector3(1, -1, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube2.Trans(back3);

	Transform back4(Vector3(0, 0, -5), Rotation3(Vector3(0, 1, 0), M_PI * 0.9)); // Start from the noncompressed state
	proto.Trans(back4);

	Mesh floor = GridSquare(64, 1);
	floor.Trans(Transform(Vector3(0, -2.5, 0)));

	float SP = 64;
	ImageTexture mat(cubemap);
	ImageTexture texproto(mcrproto); // use this on the proto mesh
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial green(0x00FF0000, SP, 1);
	BaseMaterial blue(0x0000FF00, SP, 1);
	BaseMaterial cyan(0x00FFFF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	s.addMesh(&dragon, &mat, false, "CubeMorphed");
	s.addMesh(&cube1, &red, false, "CubeRed");
	s.addMesh(&cube2, &cyan, false, "CubeCyan");
	s.addMesh(&floor, &white, false, "Floor");
	s.addMesh(&proto, &texproto, false, "Protogen");
	s.morph(0, std::vector<float>{0, 1});

	std::cout << "Prepared\n";
	for (auto i : s.names) std::cout << i << " ";
	std::cout << "\n";
	// Animation example
	Animation animation(&s);

	// Red cube moves in a square and spins
	animation.setTranslation(1, 12, Vector3(-1.5, -0.5, -3), ANIMI::SINE);
	animation.setRotation(1, 12, QuaternionAA(Vector3(0, 1, 0), -0.4 + M_PI / 2));
	animation.setTranslation(1, 24, Vector3(-1.5, 0.5, -3));
	animation.setRotation(1, 24, QuaternionAA(Vector3(0, 1, 0), -0.4 + M_PI));
	animation.setTranslation(1, 36, Vector3(-0.5, 0.5, -3));
	animation.setRotation(1, 36, QuaternionAA(Vector3(0, 1, 0), -0.4 + M_PI * 1.5));
	animation.setTranslation(1, 48, Vector3(-0.5, -0.5, -3));
	animation.setRotation(1, 48, QuaternionAA(Vector3(0, 1, 0), -0.4));

	// Protogen standing on the cube moves up and down, and swings
	animation.setTranslation(4, 24, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 24, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 48, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 48, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));

	// Large cube morphs up and down
	animation.setMorph(0, 0, ket(2, 0));
	animation.setMorph(0, 24, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 48, ket(2, 0), ANIMI::SINE_OUT);

	if (true) {
			// Protogen standing on the cube moves up and down, and swings
	animation.setTranslation(4, 8, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 8, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 16, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 16, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));
	animation.setTranslation(4, 24, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 24, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 32, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 32, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));
	animation.setTranslation(4, 40, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 40, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 48, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 48, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));

	// Large cube morphs up and down
	animation.setMorph(0, 0, ket(2, 0));
	animation.setMorph(0, 8, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 16, ket(2, 0), ANIMI::SINE_OUT);
	animation.setMorph(0, 24, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 32, ket(2, 0), ANIMI::SINE_OUT);
	animation.setMorph(0, 40, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 48, ket(2, 0), ANIMI::SINE_OUT);
	}

	// First camera moves inwards, second camera rotates a bit. Second camera also swaps in after 30 frames.
	if (true) {
		animation.setCamOrigin(0, 60, Vector3(0, 0, -2));
		animation.setCamOrigin(1, 60, Vector3(4, 1, 0), ANIMI::SINE);
		animation.setCamBasis(1, 60, Rotation3(Vector3(0, 1, 0), M_PI / 4) * Rotation3(Vector3(1, 0, 0), -M_PI / 6), ANIMI::SINE);
		animation.setCamActive(30, 1);
	}

	for (int i = 0; i < animation.getLength(); i++) {
		animation.animate(i);
		s.render();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));
		std::cout << "FRAME " << i << "DONE\n";
	}
	animation.encodeLength(VIDEO_PATH + "/LEN");
	std::cout << "Stored\n";
}


// TEST 9.83D PROTOTRACER V (Shaders)
Fragment example_shader(Fragment f) {
	Vector2 suv = f.screenUV;
	Vector2 uv = f.uv;
	int CZ = 8;
	int sx = int(suv.x * CZ);
	int sy = int(suv.y * CZ);

	int uvx = int(uv.x * CZ);
	int uvy = int(uv.y * CZ);

	if ((uvx + uvy) & 1) return FragShaders::invert(f);
	return FragShaders::identity(f);
}
Fragment octant_shader(Fragment f) {
	if (((f.wspos.x > 0) + (f.wspos.y > 0) + (f.wspos.z > 0)) & 1) return f;
	return FragShaders::invert(f);
}

// For shader functions that only use the fragment, simply constructing the shader with the function passed will do.
FragShader exampleShader(example_shader);
FragShader octantShader(octant_shader);

inline void ShaderTest() {
	Scene s = scene_protocubes();
	std::cout << "Prepared\n";

	s.render(true, 0, FragShader([](Fragment f){
		int sx = (int)(f.screenUV.x * 8);
		int sy = (int)(f.screenUV.y * 8);
		Fragment f2(f);
		if (sx == 4) f2 = FragShaders::hue_offset(f2, 1.0 / 3.0);
		if (sy == 1) f2 = FragShaders::hue_offset(f2, 1.0 / 3.0);
		return f2;
	}) ); // You need anonymous functions to use additional parameters to the shader function, or to composite multiple shaders together. 
	// In short, the anonymous function can serve as a playground for shader creation.

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

}

inline void ShaderTest2() {
	Scene s = scene_protocubes();
	std::cout << "Prepared\n";

	s.render(true, 0, FragShader([](Fragment f){
		Fragment f2(f);
		f2 = FragShaders::rainbow_concentric(f2, Vector2(0.5, 0.5), -8);
		f2 = FragShaders::rainbow_concentric(f2, Vector2(0.5, 0.2), 8);
		return f2;
	}) ); 

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

inline void ShaderTest3() {
	int N = 512;
	int D = 1;

	// RayTracer s(D, N, N);
	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);
	s.camera.Trans(Transform(Vector3(0, 0, -1)));
	s.cameras[0] = s.camera;

	Camera camera2(M_PI / 2.0);
	camera2.Trans(Transform(Vector3(4, 0, -4), Rotation3(Vector3(0, 1, 0), M_PI * 0.5)));
	s.cameras.push_back(camera2);

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

	Mesh proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");

	MorphedMesh dragon(test);
	dragon.copyTo(test2);

	Mesh cube1 = cube(0.5);
	Mesh cube2 = cube(0.5);

	Transform back(Vector3(0, -1, -5), Rotation3(Vector3(0, 1, 0), -0.4 + M_PI));
	dragon.Trans(back);

	Transform back2(Vector3(-0.5, -0.5, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube1.Trans(back2);

	Transform back3(Vector3(1, -1, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube2.Trans(back3);

	Transform back4(Vector3(0, 0, -5), Rotation3(Vector3(0, 1, 0), M_PI * 0.9)); // Start from the noncompressed state
	proto.Trans(back4);

	Mesh floor = GridSquare(64, 1);
	floor.Trans(Transform(Vector3(0, -2.5, 0)));

	float SP = 64;
	ImageTexture mat(cubemap);
	ImageTexture texproto(mcrproto); // use this on the proto mesh
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial green(0x00FF0000, SP, 1);
	BaseMaterial blue(0x0000FF00, SP, 1);
	BaseMaterial cyan(0x00FFFF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	s.addMesh(&dragon, &mat, false, "CubeMorphed", FragShader([](Fragment f) {
		return FragShaders::rainbow_spiral(f, Vector2(0.5, 0.3), 16);
	})); // !!!!
	s.addMesh(&cube1, &red, false, "CubeRed");
	s.addMesh(&cube2, &cyan, false, "CubeCyan");
	s.addMesh(&floor, &white, false, "Floor", FragShader([](Fragment f){
		float freq = 4;

		if ((BASE::ifloor(f.wspos.x * freq) + BASE::ifloor(f.wspos.y * freq) + BASE::ifloor(f.wspos.z * freq)) & 1) return FragShaders::invert(f);
		return Fragment(f);
	}));
	s.addMesh(&proto, &texproto, false, "Protogen");
	s.morph(0, std::vector<float>{0, 1});
	std::cout << "Prepared\n";

	s.render(true, 0); 

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// TEST 9.83E PROTOTRACER VI (Multiple cameras + shaders, also a stress test if needed)
inline void AnimShader() {
	int N = 512;
	int D = 1;

	// RayTracer s(D, N, N);
	Scene s(N, N);

	s.camera = Camera(M_PI / 2.0);
	s.camera.Trans(Transform(Vector3(0, 0, -1)));
	s.cameras[0] = s.camera;

	Camera camera2(M_PI / 2.0);
	camera2.Trans(Transform(Vector3(4, 0, -4), Rotation3(Vector3(0, 1, 0), M_PI * 0.5)));
	s.cameras.push_back(camera2);

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

	Mesh proto = Mesh::fromOBJ(MESHES + "/mcrproto.obj");

	MorphedMesh dragon(test);
	dragon.copyTo(test2);

	Mesh cube1 = icosphere(0.5, 4);
	Mesh cube2 = icosphere(0.5, 4);

	Transform back(Vector3(0, -1, -5), Rotation3(Vector3(0, 1, 0), -0.4 + M_PI));
	dragon.Trans(back);

	Transform back2(Vector3(-0.5, -0.5, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube1.Trans(back2);

	Transform back3(Vector3(1, -1, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube2.Trans(back3);

	Transform back4(Vector3(0, 0, -5), Rotation3(Vector3(0, 1, 0), M_PI * 0.9)); // Start from the noncompressed state
	proto.Trans(back4);

	Mesh floor = GridSquare(64, 1);
	floor.Trans(Transform(Vector3(0, -2.5, 0)));

	float SP = 64;
	ImageTexture mat(cubemap);
	ImageTexture texproto(mcrproto); // use this on the proto mesh
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial green(0x00FF0000, SP, 1);
	BaseMaterial blue(0x0000FF00, SP, 1);
	BaseMaterial cyan(0x00FFFF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	s.addMesh(&dragon, &mat, false, "CubeMorphed", exampleShader); // !!!!
	s.addMesh(&cube1, &red, false, "CubeRed");
	s.addMesh(&cube2, &cyan, false, "CubeCyan");
	s.addMesh(&floor, &white, false, "Floor");
	s.addMesh(&proto, &texproto, false, "Protogen");
	s.morph(0, std::vector<float>{0, 1});

	std::cout << "Prepared\n";
	for (auto i : s.names) std::cout << i << " ";
	std::cout << "\n";
	// Animation example
	Animation animation(&s);

	// Red cube moves in a square and spins
	animation.setTranslation(1, 12, Vector3(-1.5, -0.5, -3), ANIMI::SINE);
	animation.setRotation(1, 12, QuaternionAA(Vector3(0, 1, 0), -0.4 + M_PI / 2));
	animation.setTranslation(1, 24, Vector3(-1.5, 0.5, -3));
	animation.setRotation(1, 24, QuaternionAA(Vector3(0, 1, 0), -0.4 + M_PI));
	animation.setTranslation(1, 36, Vector3(-0.5, 0.5, -3));
	animation.setRotation(1, 36, QuaternionAA(Vector3(0, 1, 0), -0.4 + M_PI * 1.5));
	animation.setTranslation(1, 48, Vector3(-0.5, -0.5, -3));
	animation.setRotation(1, 48, QuaternionAA(Vector3(0, 1, 0), -0.4));

	// Protogen standing on the cube moves up and down, and swings
	animation.setTranslation(4, 24, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 24, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 48, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 48, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));

	// Large cube morphs up and down
	animation.setMorph(0, 0, ket(2, 0));
	animation.setMorph(0, 24, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 48, ket(2, 0), ANIMI::SINE_OUT);

	if (true) {
			// Protogen standing on the cube moves up and down, and swings
	animation.setTranslation(4, 8, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 8, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 16, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 16, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));
	animation.setTranslation(4, 24, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 24, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 32, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 32, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));
	animation.setTranslation(4, 40, Vector3(0, -0.4, -5), ANIMI::SINE_IN);
	animation.setRotation(4, 40, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9 + 0.25));
	animation.setTranslation(4, 48, Vector3(0, 0, -5), ANIMI::SINE_OUT);
	animation.setRotation(4, 48, QuaternionAA(Vector3(0, 1, 0), M_PI * 0.9));

	// Large cube morphs up and down
	animation.setMorph(0, 0, ket(2, 0));
	animation.setMorph(0, 8, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 16, ket(2, 0), ANIMI::SINE_OUT);
	animation.setMorph(0, 24, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 32, ket(2, 0), ANIMI::SINE_OUT);
	animation.setMorph(0, 40, ket(2, 1), ANIMI::SINE_IN);
	animation.setMorph(0, 48, ket(2, 0), ANIMI::SINE_OUT);
	}

	// First camera moves inwards, second camera rotates a bit. Second camera also swaps in after 30 frames.
	if (true) {
		animation.setCamOrigin(0, 48, Vector3(0, 0, -2));
		animation.setCamOrigin(1, 48, Vector3(4, 1, 0), ANIMI::SINE);
		animation.setCamBasis(1, 48, Rotation3(Vector3(0, 1, 0), M_PI / 4) * Rotation3(Vector3(1, 0, 0), -M_PI / 6), ANIMI::SINE);
		animation.setCamActive(24, 1);
	}

	for (int i = 0; i < animation.getLength(); i++) {
		animation.animate(i);
		s.render(true, 0, octantShader);
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));
		std::cout << "FRAME " << i << "DONE\n";
	}
	animation.encodeLength(VIDEO_PATH + "/LEN");
	std::cout << "Stored\n";
}

#include <chrono>

int main() {
	std::cout << "BEGIN\n";
	auto start = std::chrono::high_resolution_clock::now();

	// phongtest();
	// texmapref();

	// BVHStanford();
	// BVHSponge();
	// BVHTest();
	// BVHSimpleObject();
	// BoxTest();

	// FracNoiseTest();
	// BezierTest();

	// Subdiv2();

	// noisetex();

	// RTexTest();

	// RTexBVH();

	// texmapref();

	// BezierMeshTest();
	// BezierMeshRTX();

	// QuadMeshTest();
	// SubdivideCCTest();
	// QuadRTX();

	// TriClip();
	// TriClipAttr();
	// ClipTest();
	// MengerSpongeDual();

	// BSPTest();
	// BSPMeshTest();

	// SubdivideCCSharp();

	// FormalRaster();
	// AnimSan();

	// QuaternionTest();
	// FullAnimation();

	// MulticamTest();

	// ShaderTest();
	// ShaderTest2();
	AnimShader();
	std::cout << "End\n";



	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	int64_t count = duration.count();
    std::cout << "Time taken: " << count << " us" << std::endl;
	std::cout << "Time taken: " << count * 0.001 << " ms" << std::endl;
	std::cout << "Time taken: " << count * 0.000001 << " s" << std::endl;

	return 0;
}