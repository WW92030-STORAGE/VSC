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

// TEST 9.85A TEMPEST (Bone extraction and visualization, requires collada parser)
void BoneTreeTest() {
	Scene s(512, 512);

// Replace me with the results from the collada parser
std::vector<Vector3> BONE_v({Vector3(-1.0, -1.0, -1.0), Vector3(-1.0, -1.0, 1.0), Vector3(-1.0, 1.0, -1.0), Vector3(-1.0, 1.0, 1.0), Vector3(1.0, -1.0, -1.0), Vector3(1.0, -1.0, 1.0), Vector3(1.0, 1.0, -1.0), Vector3(1.0, 1.0, 1.0), Vector3(-1.0, -1.0, 3.0), Vector3(-1.0, 1.0, 3.0), Vector3(1.0, 1.0, 3.0), Vector3(1.0, -1.0, 3.0), Vector3(-1.0, -1.0, 5.0), Vector3(-1.0, 1.0, 5.0), Vector3(1.0, 1.0, 5.0), Vector3(1.0, -1.0, 5.0), Vector3(3.0, 1.0, 1.0), Vector3(3.0, -1.0, 1.0), Vector3(3.0, 1.0, 3.0), Vector3(3.0, -1.0, 3.0)});
std::vector<std::vector<int>> BONE_t({{1, 2, 0}, {3, 6, 2}, {7, 4, 6}, {5, 0, 4}, {6, 0, 2}, {3, 10, 7}, {10, 15, 11}, {5, 19, 11}, {1, 9, 3}, {5, 8, 1}, {13, 15, 14}, {8, 13, 9}, {11, 12, 8}, {9, 14, 10}, {16, 19, 17}, {11, 18, 10}, {7, 17, 5}, {10, 16, 7}, {1, 3, 2}, {3, 7, 6}, {7, 5, 4}, {5, 1, 0}, {6, 4, 0}, {3, 9, 10}, {10, 14, 15}, {5, 17, 19}, {1, 8, 9}, {5, 11, 8}, {13, 12, 15}, {8, 12, 13}, {11, 15, 12}, {9, 13, 14}, {16, 18, 19}, {11, 19, 18}, {7, 16, 17}, {10, 18, 16}});
std::vector<Vector2> BONE_tex({Vector2(0.625, 0.0), Vector2(0.375, 0.25), Vector2(0.375, 0.0), Vector2(0.625, 0.25), Vector2(0.375, 0.5), Vector2(0.375, 0.25), Vector2(0.625, 0.5), Vector2(0.375, 0.75), Vector2(0.375, 0.5), Vector2(0.625, 0.75), Vector2(0.375, 1.0), Vector2(0.375, 0.75), Vector2(0.375, 0.5), Vector2(0.125, 0.75), Vector2(0.125, 0.5), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.625, 1.0), Vector2(0.875, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.625, 1.0), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.375, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.375, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.375, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.375, 1.0), Vector2(0.375, 0.5), Vector2(0.375, 0.75), Vector2(0.125, 0.75), Vector2(0.625, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.0), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.875, 0.5), Vector2(0.875, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.0), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.625, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5)});
std::vector<std::vector<int>> BONE_texco({{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}, {12, 13, 14}, {15, 16, 17}, {18, 19, 20}, {21, 22, 23}, {24, 25, 26}, {27, 28, 29}, {30, 31, 32}, {33, 34, 35}, {36, 37, 38}, {39, 40, 41}, {42, 43, 44}, {45, 46, 47}, {48, 49, 50}, {51, 52, 53}, {54, 55, 56}, {57, 58, 59}, {60, 61, 62}, {63, 64, 65}, {66, 67, 68}, {69, 70, 71}, {72, 73, 74}, {75, 76, 77}, {78, 79, 80}, {81, 82, 83}, {84, 85, 86}, {87, 88, 89}, {90, 91, 92}, {93, 94, 95}, {96, 97, 98}, {99, 100, 101}, {102, 103, 104}, {105, 106, 107}});
std::vector<std::string> BONE_bones({"Bone", "Bone_001", "Bone_002", "Bone_004", "Bone_003", "Bone_005"});
std::vector<int> BONE_parent({-1, 0, 1, 1, 2, 3});
std::vector<Matrix4> BONE_bonetrans({Matrix4(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0), Matrix4(0.8894976, 0.4569398, 8.84309e-08, 0.0, -0.4569398, 0.8894976, -1.78191e-08, 1.0, -8.68013e-08, -2.45576e-08, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0), Matrix4(0.6794999, -0.7336756, -1.01477e-07, -2.98023e-08, 0.7336756, 0.6794999, -6.74618e-08, 0.9325978, 1.18449e-07, -2.86108e-08, 1.0, 1.77636e-15, 0.0, 0.0, 0.0, 1.0), Matrix4(0.6912999, 0.7225679, -1.17378e-07, -2.98023e-08, -0.7225679, 0.6912999, -4.7939e-08, 0.9325978, 4.65042e-08, 1.17954e-07, 1.0, 1.77636e-15, 0.0, 0.0, 0.0, 1.0), Matrix4(0.9396592, 0.3421122, 4.14499e-08, 0.0, -0.3421122, 0.9396592, 1.83863e-08, 1.245618, -3.26586e-08, -3.14574e-08, 1.0, 3.55271e-15, 0.0, 0.0, 0.0, 1.0), Matrix4(0.958605, 0.2847396, -6.81297e-08, 8.9407e-08, -0.2847396, 0.9586049, 5.97545e-08, 0.5986397, 8.2324e-08, -3.78817e-08, 1.0, 3.55271e-15, 0.0, 0.0, 0.0, 1.0)});
std::vector<Vector3> BONE_bonetips({Vector3(0.0, 1.0, 0.0), Vector3(-2.98023e-08, 0.9325978, 1.77636e-15), Vector3(0.0, 1.245618, 3.55271e-15), Vector3(8.9407e-08, 0.5986397, 3.55271e-15), Vector3(0.0, 0.0, 1.0), Vector3(1.0, 0.0, 0.0)});
std::unordered_set<int> BONE_atips({4, 5});




int root = 0;

std::queue<int> q;

std::unordered_map<int, Vector3> a_joints;
std::unordered_map<int, Vector3> a_tips;
std::unordered_map<int, Transform> a_trans;

// init the root bone

a_trans[root] = Transform(BONE_bonetrans[root]);
a_joints[root] = Transform(BONE_bonetrans[root]).origin;
if (BONE_atips.find(root) != BONE_atips.end()) a_tips[root] = BONE_bonetips[root] + a_joints[root];
else a_tips[root] = a_trans[root] * BONE_bonetips[root];


q.push(root);

// BFS to get the rest of them

std::unordered_map<int, std::vector<int>> children;
for (int i = 0; i < BONE_parent.size(); i++) {
	int pp = BONE_parent[i];
	if (pp < 0) continue;
	if (children.find(pp) == children.end()) children[pp] = std::vector<int>();
	children[pp].push_back(i);
}

std::cout << "START\n";

Transform t1(Matrix4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0));
Transform t2(Matrix4(0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 7.54979e-08, 1.0, 7.54979e-08, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0));

std::cout << t1.sprintf() << "\n\n" << t2.sprintf() << "\n\n" << (t1 * t2).sprintf() << "\n\n....\n";

while (q.size()) {
	int cur = q.front();
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

for (int name = 0; name < BONE_bones.size(); name++) {
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

void CylinderTest() {
	Scene s = scene_blank(true);
	Mesh m = cylinder(0.5, 2, 6);
	m.Trans(Transform(Vector3(0, -2, -4)));
	s.addMesh(&m);

	std::cout << "Prepared\n";

	s.render(true);

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

}

// TEST 9.85B DARKNESS (Bone rotation)
void SkeletonTest() {
std::vector<Vector3> BONE_v({Vector3(-1.0, -1.0, -1.0), Vector3(-1.0, -1.0, 1.0), Vector3(-1.0, 1.0, -1.0), Vector3(-1.0, 1.0, 1.0), Vector3(1.0, -1.0, -1.0), Vector3(1.0, -1.0, 1.0), Vector3(1.0, 1.0, -1.0), Vector3(1.0, 1.0, 1.0), Vector3(-1.0, -1.0, 3.0), Vector3(-1.0, 1.0, 3.0), Vector3(1.0, 1.0, 3.0), Vector3(1.0, -1.0, 3.0), Vector3(-1.0, -1.0, 5.0), Vector3(-1.0, 1.0, 5.0), Vector3(1.0, 1.0, 5.0), Vector3(1.0, -1.0, 5.0), Vector3(3.0, 1.0, 1.0), Vector3(3.0, -1.0, 1.0), Vector3(3.0, 1.0, 3.0), Vector3(3.0, -1.0, 3.0)});
std::vector<std::vector<int>> BONE_t({{1, 2, 0}, {3, 6, 2}, {7, 4, 6}, {5, 0, 4}, {6, 0, 2}, {3, 10, 7}, {10, 15, 11}, {5, 19, 11}, {1, 9, 3}, {5, 8, 1}, {13, 15, 14}, {8, 13, 9}, {11, 12, 8}, {9, 14, 10}, {16, 19, 17}, {11, 18, 10}, {7, 17, 5}, {10, 16, 7}, {1, 3, 2}, {3, 7, 6}, {7, 5, 4}, {5, 1, 0}, {6, 4, 0}, {3, 9, 10}, {10, 14, 15}, {5, 17, 19}, {1, 8, 9}, {5, 11, 8}, {13, 12, 15}, {8, 12, 13}, {11, 15, 12}, {9, 13, 14}, {16, 18, 19}, {11, 19, 18}, {7, 16, 17}, {10, 18, 16}});
std::vector<Vector2> BONE_tex({Vector2(0.625, 0.0), Vector2(0.375, 0.25), Vector2(0.375, 0.0), Vector2(0.625, 0.25), Vector2(0.375, 0.5), Vector2(0.375, 0.25), Vector2(0.625, 0.5), Vector2(0.375, 0.75), Vector2(0.375, 0.5), Vector2(0.625, 0.75), Vector2(0.375, 1.0), Vector2(0.375, 0.75), Vector2(0.375, 0.5), Vector2(0.125, 0.75), Vector2(0.125, 0.5), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.625, 1.0), Vector2(0.875, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.625, 1.0), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.375, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.375, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.375, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.375, 1.0), Vector2(0.375, 0.5), Vector2(0.375, 0.75), Vector2(0.125, 0.75), Vector2(0.625, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.0), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.875, 0.5), Vector2(0.875, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.0), Vector2(0.625, 0.0), Vector2(0.625, 0.25), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 1.0), Vector2(0.625, 0.25), Vector2(0.625, 0.25), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.75), Vector2(0.625, 0.5), Vector2(0.625, 0.5), Vector2(0.625, 0.5)});
std::vector<std::vector<int>> BONE_texco({{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}, {12, 13, 14}, {15, 16, 17}, {18, 19, 20}, {21, 22, 23}, {24, 25, 26}, {27, 28, 29}, {30, 31, 32}, {33, 34, 35}, {36, 37, 38}, {39, 40, 41}, {42, 43, 44}, {45, 46, 47}, {48, 49, 50}, {51, 52, 53}, {54, 55, 56}, {57, 58, 59}, {60, 61, 62}, {63, 64, 65}, {66, 67, 68}, {69, 70, 71}, {72, 73, 74}, {75, 76, 77}, {78, 79, 80}, {81, 82, 83}, {84, 85, 86}, {87, 88, 89}, {90, 91, 92}, {93, 94, 95}, {96, 97, 98}, {99, 100, 101}, {102, 103, 104}, {105, 106, 107}});
std::vector<std::string> BONE_bones({"Bone", "Bone_001", "Bone_002", "Bone_004", "Bone_003", "Bone_005"});
std::vector<int> BONE_parent({-1, 0, 1, 1, 2, 3});
std::vector<Matrix4> BONE_bonetrans({Matrix4(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0), Matrix4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0), Matrix4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0), Matrix4(0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 7.54979e-08, 1.0, 7.54979e-08, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0), Matrix4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0), Matrix4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)});
std::vector<Vector3> BONE_bonetips({Vector3(0.0, 1.0, 0.0), Vector3(0.0, 1.0, 0.0), Vector3(0.0, 1.0, 0.0), Vector3(0.0, 1.0, 0.0), Vector3(0.0, 0.0, 1.0), Vector3(1.0, 0.0, 0.0)});
std::unordered_set<int> BONE_atips({4, 5});
std::vector<std::unordered_map<int, float>> BONE_weights({std::unordered_map<int, float>({{0, 0.9123928}, {1, 0.0876072}}), std::unordered_map<int, float>({{0, 0.520787}, {1, 0.4718273}, {2, 0.000967168}, {4, 0.006418466}}), std::unordered_map<int, float>({{0, 0.9132175}, {1, 0.08678251}}), std::unordered_map<int, float>({{0, 0.522971}, {1, 0.4736596}, {4, 0.003369271}}), std::unordered_map<int, float>({{0, 0.8712919}, {1, 0.0564025}, {3, 0.03215259}, {5, 0.04015296}}), std::unordered_map<int, float>({{0, 0.2668086}, {1, 0.2441979}, {3, 0.2332977}, {5, 0.2556956}}), std::unordered_map<int, float>({{0, 0.8648418}, {1, 0.05643308}, {3, 0.03520345}, {5, 0.04352152}}), std::unordered_map<int, float>({{0, 0.2622015}, {1, 0.2389079}, {3, 0.237917}, {5, 0.2609735}}), std::unordered_map<int, float>({{0, 0.002806246}, {2, 0.475099}, {4, 0.5220947}}), std::unordered_map<int, float>({{0, 0.002911031}, {2, 0.4741396}, {4, 0.5229492}}), std::unordered_map<int, float>({{2, 0.0270819}, {3, 0.4278732}, {4, 0.07883459}, {5, 0.4662103}}), std::unordered_map<int, float>({{2, 0.4402776}, {3, 0.009076952}, {4, 0.4806121}, {5, 0.07003325}}), std::unordered_map<int, float>({{2, 0.09057366}, {4, 0.9094263}}), std::unordered_map<int, float>({{2, 0.08392393}, {4, 0.9160761}}), std::unordered_map<int, float>({{2, 0.008004128}, {3, 0.06594222}, {4, 0.8533447}, {5, 0.07270878}}), std::unordered_map<int, float>({{2, 0.08273655}, {4, 0.9172634}}), std::unordered_map<int, float>({{0, 0.04456901}, {1, 0.03612548}, {3, 0.05291193}, {5, 0.8663934}}), std::unordered_map<int, float>({{0, 0.04453325}, {1, 0.0362786}, {3, 0.03940868}, {5, 0.8797794}}), std::unordered_map<int, float>({{3, 0.07510328}, {5, 0.9248967}}), std::unordered_map<int, float>({{2, 0.06885421}, {4, 0.0760138}, {5, 0.8551321}})});


	Scene s = scene_blank(true);

	RiggedMesh rmesh(BONE_v, BONE_t, BONE_tex, BONE_texco, BONE_bones, BONE_parent, BONE_bonetrans, BONE_bonetips, BONE_atips, BONE_weights);

	rmesh.deformations[1] = Transform(Vector3(), Rotation3(Vector3(0, 0, 1), M_PI * -0.2)); // If the root transform is fucked up then all the rest will be also.
	rmesh.deformations[2] = Transform(Vector3(), Rotation3(Vector3(0, 0, 1), M_PI * 0.3)); // If the root transform is fucked up then all the rest will be also.
	rmesh.deformations[5] = Transform(Vector3(), Rotation3(Vector3(0, 0, 1), M_PI * 0.2)); // If the root transform is fucked up then all the rest will be also.
	rmesh.computeAbsoluteTransforms();
	rmesh.computeVertexLocations();

	s.camera.Trans(Transform(Vector3(0, -4, 2), Rotation3(Vector3(1, 0, 0), M_PI * 0.5)));

	std::vector<Mesh> bone_vis = rmesh.visBones();

	for (auto i : bone_vis) s.addMesh(&i);

	s.render();

	s.drawMesh(rmesh);

	std::cout << "Prepared\n";

	

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

int main() {
	std::cout << "BEGIN\n";
	auto start = std::chrono::high_resolution_clock::now();

	// BoneTreeTest();
	// CylinderTest();

	SkeletonTest();

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