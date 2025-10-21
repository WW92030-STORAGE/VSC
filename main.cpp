#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

#include "texture_texture.h"

#include <unordered_set>
using namespace std;



#include "misctests.h"
#include "main_utils.h" // definitions of paths and some things
#include "black_box_tests.h"

#include <chrono>

void BoneTreeTest() {
	Scene s(512, 512);

std::vector<std::string> BONE_bones({"Bone", "Bone_001", "Bone_002", "Bone_004", "Bone_003", "Bone_005"});
std::unordered_map<std::string, std::string> BONE_parent({{"Bone_001", "Bone"}, {"Bone_002", "Bone_001"}, {"Bone_004", "Bone_001"}, {"Bone_003", "Bone_002"}, {"Bone_005", "Bone_004"}});
std::unordered_map<std::string, Matrix4> BONE_bonetrans({{"Bone", Matrix4(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0)}, {"Bone_001", Matrix4(0.8894976, 0.4569398, 8.84309e-08, 0.0, -0.4569398, 0.8894976, -1.78191e-08, 1.0, -8.68013e-08, -2.45576e-08, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)}, {"Bone_002", Matrix4(0.6794999, -0.7336756, -1.01477e-07, -2.98023e-08, 0.7336756, 0.6794999, -6.74618e-08, 0.9325978, 1.18449e-07, -2.86108e-08, 1.0, 1.77636e-15, 0.0, 0.0, 0.0, 1.0)}, {"Bone_004", Matrix4(0.6912999, 0.7225679, -1.17378e-07, -2.98023e-08, -0.7225679, 0.6912999, -4.7939e-08, 0.9325978, 4.65042e-08, 1.17954e-07, 1.0, 1.77636e-15, 0.0, 0.0, 0.0, 1.0)}, {"Bone_003", Matrix4(0.9396592, 0.3421122, 4.14499e-08, 0.0, -0.3421122, 0.9396592, 1.83863e-08, 1.245618, -3.26586e-08, -3.14574e-08, 1.0, 3.55271e-15, 0.0, 0.0, 0.0, 1.0)}, {"Bone_005", Matrix4(0.958605, 0.2847396, -6.81297e-08, 8.9407e-08, -0.2847396, 0.9586049, 5.97545e-08, 0.5986397, 8.2324e-08, -3.78817e-08, 1.0, 3.55271e-15, 0.0, 0.0, 0.0, 1.0)}});
std::unordered_map<std::string, Vector3> BONE_bonetips({{"Bone_003", Vector3(0.0, 0.0, 1.0)}, {"Bone_005", Vector3(1.0, 0.0, 0.0)}, {"Bone", Vector3(0.0, 1.0, 0.0)}, {"Bone_001", Vector3(-2.98023e-08, 0.9325978, 1.77636e-15)}, {"Bone_002", Vector3(0.0, 1.245618, 3.55271e-15)}, {"Bone_004", Vector3(8.9407e-08, 0.5986397, 3.55271e-15)}});
std::unordered_set<std::string> BONE_atips({"Bone_003", "Bone_005"});

string root = "Bone";

std::queue<std::string> q;

std::unordered_map<std::string, Vector3> a_joints;
std::unordered_map<std::string, Vector3> a_tips;
std::unordered_map<std::string, Transform> a_trans;

// init the root bone

a_trans[root] = Transform(BONE_bonetrans[root]);
a_joints[root] = Transform(BONE_bonetrans[root]).origin;
if (BONE_atips.find(root) != BONE_atips.end()) a_tips[root] = BONE_bonetips[root] + a_joints[root];
else a_tips[root] = a_trans[root] * BONE_bonetips[root];


q.push(root);

// BFS to get the rest of them

std::unordered_map<std::string, std::vector<std::string>> children;
for (auto i : BONE_parent) {
	if (children.find(i.second) == children.end()) children[i.second] = std::vector<std::string>();
	children[i.second].push_back(i.first);
}

std::cout << "START\n";

Transform t1(Matrix4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0));
Transform t2(Matrix4(0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 7.54979e-08, 1.0, 7.54979e-08, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0));

std::cout << t1.sprintf() << "\n\n" << t2.sprintf() << "\n\n" << (t1 * t2).sprintf() << "\n\n....\n";

while (q.size()) {
	std::string cur = q.front();
	q.pop();
	
	auto cur_trans = a_trans[cur];
	auto cur_joint = a_joints[cur];
	if (children.find(cur) == children.end()) continue;
	for (auto child : children[cur]) {
		a_trans[child] = cur_trans * Transform(BONE_bonetrans[child]);
		a_joints[child] = a_trans[child].origin;
		if (BONE_atips.find(child) != BONE_atips.end()) a_tips[child] = BONE_bonetips[child] + a_joints[child];
		else a_tips[child] = a_trans[child] * BONE_bonetips[child];

		q.push(child);
	}
}

cout << "children\n";
for (auto i : children) {
	cout << i.first << " = ";
	for (auto j : i.second) cout << j << " ";
	cout << endl;
}

cout << "a_joints\n";
for (auto i : a_joints) {
	cout << i.first << " = " << i.second.to_string() << endl;
}

cout << "a_tips\n";
for (auto i : a_tips) {
	cout << i.first << " = " << i.second.to_string() << endl;
}

cout << "a_trans\n";
for (auto i : a_trans) {
	cout << i.first << " = " << i.second.to_string() << endl;
}

	s.camera = Camera(M_PI / 2.0);
	s.camera.Trans(Transform(Vector3(0, -5, 0), Rotation3(Vector3(1, 0, 0), M_PI / 2)));
	s.cameras[0] = s.camera;

	float A = 0.1;

	PointLight PL(Vector3(1, 1, 1), A);
	PL.Trans(Transform(Vector3(-2, 2, 0)));
	// s.lights.push_back(PL);

	PointLight P2(Vector3(1, 1, 1), A);
	P2.Trans(Transform(Vector3(0, -2, 0)));
	s.lights.push_back(P2);

	PointLight P3(Vector3(1, 1, 1), A);
	P3.Trans(Transform(Vector3(2, 2, 0)));
	s.lights.push_back(P3);

for (auto name : BONE_bones) {
	Mesh m = icosphere(0.1, 0);
	m.Trans(Transform(a_joints[name]));
	s.addMesh(&m);

	Mesh m2 = icosphere(0.05, 0);
	m2.Trans(Transform(a_joints[name] * 0.75 + a_tips[name] * 0.25));
	s.addMesh(&m2);
	Mesh m3 = icosphere(0.05, 0);
	m3.Trans(Transform(a_joints[name] * 0.5 + a_tips[name] * 0.5));
	s.addMesh(&m3);
}

	std::cout << "Prepared\n";

	s.render(true);

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

}

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
	// AnimShader();

	// Collada1();

	BoneTreeTest();

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