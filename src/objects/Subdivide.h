#ifndef SUBDIV_EXT
#define SUBDIV_EXT

#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <climits>
#include <cfloat>

#include "Object.h"
#include "Mesh.h"
#include "Triangle.h"

#include "../utils/Math.h"

/*

Various subdivision algorithms. You plug in a mesh and you get out a mesh.
This only works for static meshes. If you want to subdivide a MorphedMesh you will need to individually subdivide the sub-meshes and then collate them.

*/

/*

The most important thing to remember is that EVERYTHING NEEDS TO BE CONSISTENT.
Listed below are the components of a Mesh:

int size = 0; // number of triangles
int nverts = 0; // number of vertices
int nuv = 0; // number of uv coordinates


std::vector<Vector3> verts; // Array of vertices
std::vector<std::vector<int>> triindices; // Triangle indices in the vertex array;
std::vector<std::vector<int>> texcoords; // Texture indices per triangle

std::vector<Vector3> vn; // Vertex normals. Initialized when we do setupvertsntris() but can also be manually set e.g. when looking thru a .obj file.
std::vector<Vector2> uv; // Texture coordinates

The main change to the mesh are the addition of new vertices, but be aware that with the new vertices come new triangles, normals, and texture coordinates.
Each subdivision algorithm should be deterministic with respect to the topology.
    
*/

#include <iostream>

namespace subdiv {

template <typename T>
inline int getIndexOrCreate(std::vector<T>& v, std::map<T, int>& inv, T i) {
    if (inv.find(i) == inv.end()) {
        inv.insert({i, v.size()});
        v.push_back(i);
        return inv.at(i);
    }

    return inv.at(i);
}

};

inline Mesh subdivide(Mesh m, int k = 1) {
    if (k <= 0) return m;
    if (k > 1) m = subdivide(m, k - 1);
    /*
    
    First we subdivide the vertices. This is done by adding vertices based on existing vertices and triangles.
    Obviously you might get overlapping vertices. So to reduce redundancy the first time we "mention" a vertex it gets stored into the array.
    All other times we simply reference the index.

    */
    
    std::vector<Vector3> verts;
    std::map<Vector3, int> verts_inv;

    std::vector<std::vector<int>> triindices;

    std::vector<std::vector<int>> texindices;

    std::vector<Vector2> uv;
    std::map<Vector2, int> uv_inv;

    for (int trii = 0; trii < m.size; trii++) {
        auto tri = m.makeTriangle(trii);
        Vector3 midverts[3];
        for (int i = 0; i < 3; i++) midverts[i] = (tri.p[i] + tri.p[(i + 1) % 3]) * 0.5; 
        // Therefore a vertex tri.p[i] is adjacent to midverts[i] and midverts[i - 1]

        // But wait! We still need to update the normals and all that good stuff.
        // Normals let's just let the Mesh class do it itself. But textures we do need to moderate.
        // Remember that meshes store a list of used texture coordinates, but just as with vertex indices, each triangle has a list of texture indices.
        // Texture indices are not necessarily constant per vertex position, so don't try to reduce redundancies.

        Vector2 corneruvs[3];
        for (int i = 0; i < 3; i++) corneruvs[i] = m.uv[m.texcoords[trii][i]];
        // So thus corneruv[i] is the uv of point tri.p[i]
        // Interpolate to get the midpoints

        Vector2 miduvs[3];
        for (int i = 0; i < 3; i++) miduvs[i] = (corneruvs[i] + corneruvs[(i + 1) % 3]) * 0.5;
            
        // Construct the triangles. First the triangles touching the edges of the original triangle

        for (int vi = 0; vi < 3; vi++) {
            int cornervertex = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, tri.p[vi]);
            int ev1 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[vi]);
            int ev0 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[(vi + 2) % 3]);

            std::vector<int> newtri({cornervertex, ev1, ev0});

            // But wait! We need to create the texture coordinates as well

            int corneruv = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, corneruvs[vi]);
            int euv1 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[vi]);
            int euv0 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[(vi + 2) % 3]);

            std::vector<int> newuv({corneruv, euv1, euv0});

            triindices.push_back(newtri);
            texindices.push_back(newuv);
        }
        // Then the central triangle
        int xx0 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[0]);
        int xx1 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[1]);
        int xx2 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[2]);
        std::vector<int> xx({xx0, xx1, xx2});
        triindices.push_back(xx);

        xx0 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[0]);
        xx1 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[1]);
        xx2 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[2]);
        
        xx = std::vector<int>({xx0, xx1, xx2});
        texindices.push_back(xx);


    }
    return Mesh(verts, triindices, uv, texindices);
}

inline Mesh subdivideLoop(Mesh m, int k = 1) {
    if (k <= 0) return m;
    if (k > 1) m = subdivideLoop(m, k - 1);
    /*
    
    First we subdivide the vertices. This is done by adding vertices based on existing vertices and triangles.
    Obviously you might get overlapping vertices. So to reduce redundancy the first time we "mention" a vertex it gets stored into the array.
    All other times we simply reference the index.

    */
    
    std::vector<Vector3> verts;
    std::map<Vector3, int> verts_inv;

    std::vector<std::vector<int>> triindices;

    std::vector<std::vector<int>> texindices;

    std::vector<Vector2> uv;
    std::map<Vector2, int> uv_inv;

    std::cout << "MESH " << m.size << " TRIS " << m.nverts << " VERTS\n";

    for (int trii = 0; trii < m.size; trii++) {
        auto tri = m.makeTriangle(trii);

        Vector3 cornerverts[3];
        for (int i = 0; i < 3; i++) cornerverts[i] = tri.p[i];

        Vector3 midverts[3];
        for (int i = 0; i < 3; i++) midverts[i] = (cornerverts[i] + cornerverts[(i + 1) % 3]) * 0.5; 
        // Therefore a vertex tri.p[i] is adjacent to midverts[i] and midverts[i - 1]

        /*
        
        BUT THIS TIME IT'S A BIT DIFFERENT
        In linear subdivision the vertices were kept in their original positions. 
        But this time the vertices are modified based on the original positions of surrounding vertices.

        For an edge vertex, this depends on the two triangles it lies on, and the four vertices of those two triangles.

        Three of those vertices are known, they are the triangle (tri) we reference. The 4th vertex requires searching for another triangle with the given edge.
        midverts[i] is between cornerverts[i] and cornerverts[i + 1] (mod 3). 
        In indices that's m.triindices[trii][i] and m.triindices[trii][i + 1]. So search for a triangle with those two vertices but in reverse order.

        Because meshes are "watertight" you can just search for a vertex by its index in the Mesh::verts array.

        */

        for (int i = 0; i < 3; i++) {
            int firstver = m.triindices[trii][i];
            int secondver = m.triindices[trii][(i + 1) % 3];
            int otherindex = -1;
            int offset = 0;
            for (int other = 0; other < m.size; other++) {
                if (other == trii) continue;
                for (int s = 0; s < 3; s++) {
                    int firsttest = m.triindices[other][s];
                    int secondtest = m.triindices[other][(s + 2) % 3];
                    if (firsttest == firstver && secondtest == secondver) {
                        offset = s;
                        otherindex = other;
                        break;
                    }
                }
                if (otherindex >= 0) break;
            }

            if (otherindex >= 0) {
                // The fourth vertex is at index m.triindices[otherindex][(offset + 1) % 3].
                int thirdver = m.triindices[trii][(i + 2) % 3];
                int fourthver = m.triindices[otherindex][(offset + 1) % 3];

                Vector3 newpos(0, 0, 0);

                float SMOL = 1.0 / 8.0;
                float BIG = 0.5 - SMOL;
                newpos = newpos + m.verts[firstver] * BIG;
                newpos = newpos + m.verts[secondver] * BIG;
                newpos = newpos + m.verts[thirdver] * SMOL;
                newpos = newpos + m.verts[fourthver] * SMOL;

                midverts[i] = newpos;
            }
        }

        // For a corner vertex, we use the other corners of the triangles that share it.
        // Corner vertex indices are m.triindices[trii][i]

        for (int i = 0; i < 3; i++) {
            int cornerindex = m.triindices[trii][i];
            std::vector<int> triangles;
            for (int T = 0; T < m.size; T++) {
                bool Tinc = false;
                // std::cout << cornerindex << " : " << m.triindices[T][0] << " " << m.triindices[T][1] << " " << m.triindices[T][2] << "\n";
                for (int j = 0; j < 3; j++) {
                    if (m.triindices[T][j] == cornerindex) Tinc = true;
                }
                if (Tinc) triangles.push_back(T);
            }

            // std::cout << triangles.size() << " TRIANGLES ADJ " << "\n";

            std::set<int> others;
            for (auto tri2 : triangles) {
                for (int vi = 0; vi < 3; vi++) {
                    int index = m.triindices[tri2][vi];
                    if (index == cornerindex) continue;
                    others.insert(index);
                }
            }

            // std::cout << others.size() << " VERTS " << "\n";
            // std::cout << m.verts[cornerindex].to_string() << ": \n";
            // for (auto ii : others) std::cout << m.verts[ii].to_string() << "\n";

            float beta = 3.0 / 16.0;
            if (others.size() > 3) beta = 3.0 / (8.0 * others.size());
            Vector3 newpos = m.verts[cornerindex] * (1.0 - others.size() * beta);
            for (auto ii : others) newpos = newpos + (m.verts[ii]) * beta;

            cornerverts[i] = newpos;
        }


        // But wait! We still need to update the normals and all that good stuff.
        // Normals let's just let the Mesh class do it itself. But textures we do need to moderate.
        // Remember that meshes store a list of used texture coordinates, but just as with vertex indices, each triangle has a list of texture indices.
        // Texture indices are not necessarily constant per vertex position, so don't try to reduce redundancies.

        Vector2 corneruvs[3];
        for (int i = 0; i < 3; i++) corneruvs[i] = m.uv[m.texcoords[trii][i]];
        // So thus corneruv[i] is the uv of point tri.p[i]
        // Interpolate to get the midpoints

        Vector2 miduvs[3];
        for (int i = 0; i < 3; i++) miduvs[i] = (corneruvs[i] + corneruvs[(i + 1) % 3]) * 0.5;
            
        // Construct the triangles. First the triangles touching the edges of the original triangle

        for (int vi = 0; vi < 3; vi++) {
            int cornervertex = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, cornerverts[vi]);
            int ev1 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[vi]);
            int ev0 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[(vi + 2) % 3]);

            std::vector<int> newtri({cornervertex, ev1, ev0});

            // But wait! We need to create the texture coordinates as well

            int corneruv = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, corneruvs[vi]);
            int euv1 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[vi]);
            int euv0 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[(vi + 2) % 3]);

            std::vector<int> newuv({corneruv, euv1, euv0});

            triindices.push_back(newtri);
            texindices.push_back(newuv);
        }
        // Then the central triangle
        int xx0 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[0]);
        int xx1 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[1]);
        int xx2 = subdiv::getIndexOrCreate<Vector3>(verts, verts_inv, midverts[2]);
        std::vector<int> xx({xx0, xx1, xx2});
        triindices.push_back(xx);

        xx0 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[0]);
        xx1 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[1]);
        xx2 = subdiv::getIndexOrCreate<Vector2>(uv, uv_inv, miduvs[2]);
        
        xx = std::vector<int>({xx0, xx1, xx2});
        texindices.push_back(xx);


    }
    return Mesh(verts, triindices, uv, texindices);
}

#endif