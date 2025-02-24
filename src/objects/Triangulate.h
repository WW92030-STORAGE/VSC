#ifndef TRIANGULATE_EXT
#define TRIANGULATE_EXT

#include <vector>
#include "Triangle.h"
#include "../utils/Math.h"


	// Get vertex given list of vertices and obj form index
	inline static Vector3 vindex(std::vector<Vector3>& v, int i) {
		int n = v.size();
		if (i > 0) return Vector3(v[i - 1]);
		return Vector3(v[n - i]);
	}

	inline static std::vector<Triangle3> TriangulateConvex(std::vector<int> vertices, std::vector<Vector3> verts) {
		std::vector<Triangle3> tris;
		
		Vector3 v0 = vindex(verts, vertices[0]);
		for (int i = 1; i < vertices.size() - 1; i++) {
			Vector3 v1 = vindex(verts, vertices[i]);
			Vector3 v2 = vindex(verts, vertices[i + 1]);
			tris.push_back(Triangle3(v0, v1, v2));
		}

		return tris;
	}

// Algorithm: until there aRe N - 2 triangles: find 3 consecutive verticEs that are counterclockwise and contain no vertices in the interior.
	// The three vertices form a triangle, and the middle vertex is clipped.
	inline static std::vector<Triangle3> Triangulate(std::vector<int> vertices, std::vector<Vector3> verts) {

		std::vector<Triangle3> tris;
		int N = vertices.size();
		if (N < 3) return tris;

		if (N == 3) {
			Vector3 s[3];
			for (int i = 0; i < 3; i++) s[i] = vindex(verts, vertices[i]);
			return std::vector<Triangle3>(1, Triangle3(s[0], s[1], s[2]));
		}

		std::vector<int> v(vertices);

		for (int in = 0; in < N - 2; in++) {
			int n = v.size();
			int i = 0;
			for (i = 0; i < n; i++) {
				int a = vertices[i];
				int b = vertices[(i + 1) % n];
				int c = vertices[(i + 2) % n];
				auto A = vindex(verts, a);
				auto B = vindex(verts, b);
				auto C = vindex(verts, c);
				Triangle3 testtri(A, B, C);
				bool ear = true;
				for (int j = 3; j < n; j++) {
					auto D = vindex(verts, vertices[(i + j) % n]);
					if (testtri.inside(D)) {
						ear = false;
						break;
					}
				}
				if (ear) break;
			}
				int a = vertices[i];
				int b = vertices[(i + 1) % n];
				int c = vertices[(i + 2) % n];
				auto A = vindex(verts, a);
				auto B = vindex(verts, b);
				auto C = vindex(verts, c);
			Triangle3 thetri(A, B, C);
			tris.push_back(thetri);
			vertices.erase(vertices.begin() + i);
		}
		return tris;
	}


	#endif