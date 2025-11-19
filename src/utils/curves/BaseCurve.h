#ifndef BASECURVE_EXT
#define BASECURVE_EXT

#include <vector>
#include <string>
#include "../Math.h"
#include "../../objects/Mesh.h"


/*

Base curve class. This behaves exactly like a Bezier curve.

*/

class BaseCurve {
    public:
    std::vector<Vector3> controls;
    int size;

    BaseCurve() {
        controls = std::vector<Vector3>({Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1)});
        size = controls.size();
    }

    BaseCurve(std::vector<Vector3> v) {
        controls = std::vector<Vector3>(v);
        size = controls.size();
    }
    
    virtual inline Vector3 query(float t) {
        t = BASE::clamp(t, 0, 1);

        std::vector<Vector3> lerps(controls);
        std::vector<Vector3> interps(controls);
        for (int i = size - 1; i >= 1; i--) {
            for (int j = 0; j < i; j++) interps[j] = lerp(lerps[j], lerps[j + 1], t);

            lerps = interps;
        }
        return lerps[0];
    }

    inline virtual std::string to_string() {
        std::string res = "BaseCurve[";
        for (int i = 0; i < size; i++) {
            if (i) res = res + ", ";
            res = res + controls[i].to_string();
        }
        return res + "]";
    }
};

class BaseSurface {
    public:
    std::vector<std::vector<Vector3>> controls;
    int N, M;

    BaseSurface() {
        controls = std::vector<std::vector<Vector3>>({{Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(2, 0, 0)}, {Vector3(0, 1, 0), Vector3(1, 1, 0), Vector3(2, 1, 0)}});
        N = controls.size();
        M = controls[0].size();
    }

    BaseSurface(std::vector<std::vector<Vector3>> v) {
        controls = std::vector<std::vector<Vector3>>(v.size());
        for (int i = 0; i < v.size(); i++) {
            controls[i] = std::vector<Vector3>(v[i].size());
            for (int j = 0; j < controls[i].size(); j++) controls[i][j] = Vector3(v[i][j]);
        }

        N = controls.size();
        M = controls[0].size();
    }

    virtual inline std::vector<Vector3> getUCurve(float u) {
        std::vector<Vector3> thing(N);
        for (int i = 0; i < N; i++) {
            BaseCurve curve = BaseCurve(controls[i]);
            thing[i] = curve.query(u);
        }
        return thing;
    }
    
    virtual inline Vector3 query(float u, float v) {
        u = BASE::clamp(u, 0, 1);
        v = BASE::clamp(v, 0, 1);

        std::vector<Vector3> ucurve = getUCurve(u);
        return BaseCurve(ucurve).query(v);
    }

    inline Mesh ctrlmesh(Vector2 topleft = Vector2(0, 0), Vector2 topright = Vector2(1, 0), Vector2 botleft = Vector2(0, 1), Vector2 botright = Vector2(1, 1)) {
        std::vector<std::vector<Vector3>> vertgrid(N, std::vector<Vector3>(M));
        std::vector<std::vector<Vector2>> uvgrid(N, std::vector<Vector2>(M));
        
        std::vector<Vector3> verts;
        std::vector<Vector2> uvs;

        // std::cout << N << " " << M << " " << cellsU << " " << cellsV << "\n";
        // std::cout << verts.size() << " " << uvs.size() << " " << vertgrid.size() << " " << vertgrid[0].size() << "\n";

        for (int uu = 0; uu < N; uu++) {
            for (int vv = 0; vv < M; vv++) {
                // std::cout << "!" << uu << " " << vv << "\n";
                float u = (float)(uu) / (N - 1);
                float v = (float)(vv) / (M - 1);

                Vector2 TOPUV = lerp(topleft, topright, u);
                Vector2 BOTUV = lerp(botleft, botright, u);

                Vector3 point(controls[uu][vv]);

                // std::cout << uu << " " << vv << " | " << u << " " << v << " = " << point.to_string() << "\n";

                verts.push_back(point);

                Vector2 theuv = lerp(TOPUV, BOTUV, v);
                uvs.push_back(theuv);


                vertgrid[uu][vv] = point;
                uvgrid[uu][vv] = theuv;
            }
        }

        std::vector<std::vector<int>> triindices;
        std::vector<std::vector<int>> texcoords;

        /*
        0----0
        |   /|
        |  / |
        | /  |
        |/   |
        0----0
        
        */

        for (int uu = 0; uu < N - 1; uu++) {
            for (int vv = 0; vv < M - 1; vv++) {
                int vlen = M;
                std::vector<int> firstone({uu * vlen + vv, (uu + 1) * vlen + vv, (uu + 1) * vlen + (vv + 1)});
                std::vector<int> secondone({(uu + 1) * vlen + (vv + 1), uu * vlen + (vv + 1), uu * vlen + vv});

                triindices.push_back(firstone);
                texcoords.push_back(firstone);
                triindices.push_back(secondone);
                texcoords.push_back(secondone);
            }
        }

        return Mesh(verts, triindices, uvs, texcoords);
    }

    inline Mesh render(int cellsU = 1, int cellsV = 1, Vector2 topleft = Vector2(0, 0), Vector2 topright = Vector2(1, 0), Vector2 botleft = Vector2(0, 1), Vector2 botright = Vector2(1, 1)) {
        std::vector<std::vector<Vector3>> vertgrid(cellsU + 1, std::vector<Vector3>(cellsV + 1));
        std::vector<std::vector<Vector2>> uvgrid(cellsU + 1, std::vector<Vector2>(cellsV + 1));
        
        std::vector<Vector3> verts;
        std::vector<Vector2> uvs;

        // std::cout << N << " " << M << " " << cellsU << " " << cellsV << "\n";
        // std::cout << verts.size() << " " << uvs.size() << " " << vertgrid.size() << " " << vertgrid[0].size() << "\n";

        for (int uu = 0; uu <= cellsU; uu++) {
            for (int vv = 0; vv <= cellsV; vv++) {
                // std::cout << "!" << uu << " " << vv << "\n";
                float u = (float)(uu) / cellsU;
                float v = (float)(vv) / cellsV;

                Vector2 TOPUV = lerp(topleft, topright, u);
                Vector2 BOTUV = lerp(botleft, botright, u);

                Vector3 point = query(u, v);

                // std::cout << uu << " " << vv << " | " << u << " " << v << " = " << point.to_string() << "\n";

                verts.push_back(point);

                Vector2 theuv = lerp(TOPUV, BOTUV, v);
                uvs.push_back(theuv);


                vertgrid[uu][vv] = point;
                uvgrid[uu][vv] = theuv;
            }
        }

        std::vector<std::vector<int>> triindices;
        std::vector<std::vector<int>> texcoords;

        /*
        0----0
        |   /|
        |  / |
        | /  |
        |/   |
        0----0
        
        */

        for (int uu = 0; uu < cellsU; uu++) {
            for (int vv = 0; vv < cellsV; vv++) {
                int vlen = cellsV + 1;
                std::vector<int> firstone({uu * vlen + vv, (uu + 1) * vlen + vv, (uu + 1) * vlen + (vv + 1)});
                std::vector<int> secondone({(uu + 1) * vlen + (vv + 1), uu * vlen + (vv + 1), uu * vlen + vv});

                triindices.push_back(firstone);
                texcoords.push_back(firstone);
                triindices.push_back(secondone);
                texcoords.push_back(secondone);
            }
        }

        return Mesh(verts, triindices, uvs, texcoords);
    }
};

#endif