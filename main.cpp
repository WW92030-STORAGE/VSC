#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

#include "texture_texture.h"

#include <unordered_set>
using namespace std;

#include "main_utils.h" // definitions of paths and some things

// #include "misctests.h"

#include "black_box_tests.h"
#include "physicstests.h"

#include <chrono>

int main() {
	std::cout << "BEGIN\n";
	auto start = std::chrono::high_resolution_clock::now();

	// RTexBVH();

	// BoneTreeTest();
	// CylinderTest();
	// SkeletonTest();
	// SkinTest();
	// SkinAnim();
	// MCRProtoAnim();

	// AnimShader();
	// AnimRTX();
	// spinningcube();

	// Particle0();
	// Particle1();
	// Particle2();
	// RotatingSpring();

	// GroundCollisionTest();
	// GroundSpring();
	// CableTest();

	// SquareRods();

	// RigidBody0();
	// RigidBody1();
	// RigidBody2();

	// RigidBody4();

	BoundingVolumes1();

	/*
	Matrix3 A(1, 3, 2, 3, 2, 1, 2, 1, 3);
	auto p = eigen3_sym(A);
	cout << p.first.to_string() << "\n" << p.second.sprintf() << endl;
	*/
	
	/*


	Matrix2 A(2, 4, -3, 1);
	std::cout << A.to_string() << "\n" << A.inv().to_string() << "\n" << (A.inv() * A).to_string() << std::endl;

	Vector3 v(1, 2, 3);

	Vector3 u = v;
	u.x = 100;
	std::cout << u.to_string() << " " << v.to_string() << "\n";

	Vector3 defaultconstest;

	std::cout << defaultconstest.to_string() << "\n";

	*/

	std::cout << "End\n";



	auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	int64_t count = duration.count();
    std::cout << "Time taken: " << count << " us" << std::endl;
	std::cout << "Time taken: " << count * 0.001 << " ms" << std::endl;
	std::cout << "Time taken: " << count * 0.000001 << " s" << std::endl;

	return 0;
}