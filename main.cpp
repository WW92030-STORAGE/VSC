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



#include "misctests.h"



Transform rotatecam(Vector3(0, 1, 0), Rotation3(Vector3(1, 0, 0), M_PI * -0.2));

// TEST 8.2 CASSIE I (Texture map of the cube6)
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

	Transform back(Vector3(0, -2, -4), Rotation3(Vector3(0, 1, 0), -0.4));

	std::cout << "Animated\n";

	s.clearBuffer();

	cassie.Trans(back);

	ImageTexture* mat = new ImageTexture(cubemap);
	mat->specular = 64;

	s.fillMesh(cassie, mat, true, true);

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

	s.fillMesh(prism, mat, true, true);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete mat;
}

// TEST 9.5B NEXIE (Subdivision I)
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
	nexie = subdivideLoop(nexie, 0);

	Transform back(Vector3(0, -2, -4), Rotation3(Vector3(0, 1, 0), -M_PI * 0.9));

	std::cout << "Animated\n";

	s.clearBuffer();

	nexie.Trans(back);

	BaseMaterial* mat = new BaseMaterial(Vector3(0, 1, 0));
	mat->specular = 64;

	ImageTexture* imgtex = new ImageTexture(cubemap);
	imgtex->specular = 64;

	s.fillMesh(nexie, imgtex, true, true);

	std::cout << "Drawn\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete mat;
	delete imgtex;
}

#include <chrono>

int main() {
	auto start = std::chrono::high_resolution_clock::now();

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

	RTexBVH();

	// texmapref();

	std::cout << "End\n";



	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	int64_t count = duration.count();
    std::cout << "Time taken: " << count << " us" << std::endl;
	std::cout << "Time taken: " << count * 0.001 << " ms" << std::endl;
	std::cout << "Time taken: " << count * 0.000001 << " s" << std::endl;

	return 0;
}