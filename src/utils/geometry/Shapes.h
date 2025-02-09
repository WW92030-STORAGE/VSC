#include "Primitives.h"
#include "../../objects/Mesh.h"
#include "../../objects/Triangle.h"

#include <map>
#include <vector>
#include <utility>

#include <iostream>

inline Mesh cube(float X = 16) {

	Vector3 OFFSET(-X, -X, X);
	OFFSET = OFFSET * 0.5;

	Vector3 FFF(0, 0, 0);
	Vector3 TFF(X, 0, 0);
	Vector3 FTF(0, X, 0);
	Vector3 TTF(X, X, 0);
	Vector3 FFT(0, 0, -X);
	Vector3 TFT(X, 0, -X);
	Vector3 FTT(0, X, -X);
	Vector3 TTT(X, X, -X);

	FFF = FFF + OFFSET;
	TFF = TFF + OFFSET;
	FTF = FTF + OFFSET;
	TTF = TTF + OFFSET;
	FFT = FFT + OFFSET;
	TFT = TFT + OFFSET;
	FTT = FTT + OFFSET;
	TTT = TTT + OFFSET;

	Triangle3* tris = new Triangle3[12] {Triangle3(FFF, TFF, FTF), 
	Triangle3(TFF, TTF, FTF),
	Triangle3(FFF, FTF, FFT),
	Triangle3(FTF, FTT, FFT),
	Triangle3(FFF, FFT, TFF),
	Triangle3(FFT, TFT, TFF),
	Triangle3(TTT, TFT, FTT),
	Triangle3(FTT, TFT, FFT),
	Triangle3(TTT, TTF, TFT),
	Triangle3(TFT, TTF, TFF),
	Triangle3(TTT, FTT, TTF),
	Triangle3(TTF, FTT, FTF)
	};

	Mesh res(tris, 12);

	return res;
}

// Methods for subdividing triangles
inline int VFE(std::vector<Vector3>& vertices, std::map<std::pair<int, int>, int>& lookup, int f, int s) {
	std::pair<int, int> p = {f, s};
	if (f > s) p = {s, f};

	auto inserted = lookup.insert({p, vertices.size()});
	if (inserted.second) {
		Vector3 e1 = vertices[f];
		Vector3 e2 = vertices[s];
		Vector3 pt = (e1 + e2).normalized();
		vertices.push_back(pt);
	}

	return inserted.first->second;
}

inline std::vector<std::vector<int>> subdivide(std::vector<Vector3>& verts, std::vector<std::vector<int>>& tris) {
	std::map<std::pair<int, int>, int> lookup;
	std::vector<std::vector<int>> result;

	for (auto each : tris) {
		int mid[3];
		for (int e = 0; e < 3; e++) {
			mid[e] = VFE(verts, lookup, each[e], each[(e + 1) % 3]);
		}

		result.push_back({each[0], mid[0], mid[2]});
    	result.push_back({each[1], mid[1], mid[0]});
    	result.push_back({each[2], mid[2], mid[1]});
    	result.push_back({mid[0], mid[1], mid[2]});
	}
	return result;
}

// // https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/
inline Mesh icosphere(float R = 1, int S = 1) {
	// Failsafe
	S = BASE::min(S, 5);


	const float X=.525731112119133606f;
	const float Z=.850650808352039932f;
	const float N=0.f;

	std::vector<Vector3> vertices=
{
  {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
  {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
  {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
};
	std::vector<std::vector<int>> triangles =
{
  {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
  {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
  {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
  {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
};

	for (int s = 0; s < S; s++) {
		triangles = subdivide(vertices, triangles);
	}

	for (int i = 0; i < vertices.size(); i++) vertices[i] = vertices[i] * R;

	int NL = triangles.size();

	/*
	Triangle3* res = new Triangle3[NL];
	for (int i = 0; i < NL; i++) {
		res[i] = Triangle3(vertices[triangles[i][0]], vertices[triangles[i][2]], vertices[triangles[i][1]]);
	}
	*/

	Mesh ret(vertices, triangles);

	// delete[] res;
	return ret;
}