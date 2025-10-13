#ifndef MAIN_UTILS_EXT_EXT
#define MAIN_UTILS_EXT_EXT

#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

#include "texture_texture.h"


std::string MESHES = "misc";
std::string BUFFER_PATH = "BUFFER";
std::string VIDEO_PATH = "video";


using namespace std;

Scene scene_protocubes() {
	int N = 512;
	int D = 1;

	// RayTracer s(D, N, N);
	Scene s(N, N);

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

	s.addMesh(&dragon, &mat, false);
	s.addMesh(&cube1, &red, false);
	s.addMesh(&cube2, &cyan, false);
	s.addMesh(&floor, &white, false);
	s.addMesh(&proto, &texproto, false);
	
	s.morph(0, std::vector<float>{0, 1});

    return s;
}

#endif