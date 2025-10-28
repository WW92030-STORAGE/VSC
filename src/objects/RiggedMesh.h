#ifndef RIGGEDMESH_EXT
#define RIGGEDMESH_EXT

// #include <iostream>

#include <string>
#include <vector>
#include <utility>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "Triangle.h"
#include "Object.h"
#include "../utils/math/Matrices.h"
#include "../utils/math/Transform.h"

#include "../utils/stringutils.h"
#include "../utils/LinkedList.h"
#include "../utils/Utils.h"

#include "../utils/geometry/Shapes.h"



#include "Mesh.h"

class RiggedMesh : public Mesh {
	public:
	int size = 0;
	int nverts = 0;
	int nuv = 0;
	int nbones = 0;

    std::vector<std::string> bone_names;
    std::unordered_map<std::string, int> bone_names_inv;

    std::vector<int> parent;
    std::vector<std::vector<int>> children;
    std::vector<Transform> rest_transforms;
    std::vector<Vector3> tips;
    std::vector<Transform> deformations;

    std::vector<Transform> absolute_transforms;
    std::vector<Vector3> absolute_joints;
    std::vector<Vector3> absolute_tips;
    std::unordered_set<int> a_tips;

    std::vector<std::unordered_map<int, float>> vertex_weights;
    std::vector<std::unordered_map<int, Vector3>> vertex_displacements;

    std::vector<Vector3> original_verts;

    RiggedMesh(const RiggedMesh& other) : Mesh(other) {
		size = other.size;
		nverts = other.nverts;
		nuv = other.nuv;
		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size);
		texcoords = std::vector<std::vector<int>>(size);
		uv = std::vector<Vector2>(nuv);
		for (int i = 0; i < nverts; i++) verts[i] = Vector3(other.verts[i]);
		for (int i = 0; i < nuv; i++) uv[i] = Vector2(other.uv[i]);
		for (int i = 0; i < size; i++) {
			triindices[i] = std::vector<int>(other.triindices[i]);
			texcoords[i] = std::vector<int>(other.texcoords[i]);
		}

        nbones = other.nbones;

        bone_names = std::vector<std::string>(other.bone_names);
        bone_names_inv = std::unordered_map<std::string, int>(other.bone_names_inv);
        parent = std::vector<int>(other.parent);
        children = std::vector<std::vector<int>>(other.children);
        rest_transforms = std::vector<Transform>(other.rest_transforms);
        tips = std::vector<Vector3>(other.tips);
        deformations = std::vector<Transform>(other.deformations);
        absolute_transforms = std::vector<Transform>(other.absolute_transforms);
        absolute_joints = std::vector<Vector3>(other.absolute_joints);
        absolute_tips = std::vector<Vector3>(other.absolute_tips);
        a_tips = std::unordered_set<int>(other.a_tips);
        vertex_weights = std::vector<std::unordered_map<int, float>>(other.vertex_weights);
        vertex_displacements = std::vector<std::unordered_map<int, Vector3>>(other.vertex_displacements);
        original_verts = std::vector<Vector3>(other.original_verts);

        init();
    }

    void init() {
        // Setup bone_names_inv

        for (int i = 0; i < bone_names.size(); i++) bone_names_inv[bone_names[i]] = i;

        deformations = std::vector<Transform>(nbones, Transform(Matrix4::eye()));
        absolute_transforms = std::vector<Transform>(rest_transforms);
        absolute_tips = std::vector<Vector3>(tips);
        absolute_joints = std::vector<Vector3>(nbones);

        computeAbsoluteTransforms();

        // Need to re-offset the absolute tips!
        for (int i = 0; i < nbones; i++) {
            if (a_tips.find(i) != a_tips.end()) tips[i] = absolute_transforms[i].inv() * (tips[i] + absolute_joints[i]);
        }
        
        computeAbsoluteTransforms();

        // For each vertex V and its influencers [B], compute the position of V in B's rest frame.
        vertex_displacements = std::vector<std::unordered_map<int, Vector3>>(nverts, std::unordered_map<int, Vector3>());
        for (int i = 0; i < nverts; i++) {
            for (auto p : vertex_weights[i]) {
                int bone_index = p.first;
                auto googoogaga = absolute_transforms[bone_index].inv() * verts[i];
                vertex_displacements[i][bone_index] = googoogaga;
            }
        }

        original_verts = std::vector<Vector3>(verts);

        setupvns();
    }

    void Trans(Transform t) override {
        transform = t * transform;
		Matrix4 trans = t.matrix();
		for (int i = 0; i < nverts; i++) {
            verts[i] = vec3(trans * fromPoint(verts[i]));
            original_verts[i] = vec3(trans * fromPoint(original_verts[i]));
        }

        for (int i = 0; i < nbones; i++) {
            if (is_root(i)) rest_transforms[i] = t * rest_transforms[i];
        }
		setupvns();
    }

    void ForceTrans(Transform t) override {
		Matrix4 trans = t.matrix();
		for (int i = 0; i < nverts; i++) {
            verts[i] = vec3(trans * fromPoint(verts[i]));
            original_verts[i] = vec3(trans * fromPoint(original_verts[i]));
        }

        for (int i = 0; i < nbones; i++) {
            if (is_root(i)) rest_transforms[i] = t * rest_transforms[i];
        }
		setupvns();
    }

    RiggedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t) : Mesh(v, t){
        init();
    }

    RiggedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco) : Mesh(v, t, tex, texco){
        size = t.size();
		nverts = v.size();
		nuv = tex.size();

		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));

		uv = std::vector<Vector2>(nuv);

		for (int i = 0; i < nverts; i++) verts[i] = Vector3(v[i]);
		for (int i = 0; i < nuv; i++) uv[i] = Vector2(tex[i]);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3 && j < t[i].size(); j++) {
				triindices[i][j] = t[i][j];
				if (i < texco.size()) texcoords[i][j] = texco[i][j];
			}
		} 
        init();
    }

    RiggedMesh(std::vector<Vector3>& v, std::vector<std::vector<int>>& t,std::vector<Vector2> tex, std::vector<std::vector<int>>& texco, 
    std::vector<std::string>& names, std::vector<int>& par, std::vector<Matrix4>& transforms, std::vector<Vector3>& tipp, std::unordered_set<int>& atips, std::vector<std::unordered_map<int, float>> weights) : Mesh(v, t, tex, texco){
        size = t.size();
		nverts = v.size();
		nuv = tex.size();

		verts = std::vector<Vector3>(nverts);
		triindices = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));
		texcoords = std::vector<std::vector<int>>(size, std::vector<int>(3, 0));

		uv = std::vector<Vector2>(nuv);

		for (int i = 0; i < nverts; i++) verts[i] = Vector3(v[i]);
		for (int i = 0; i < nuv; i++) uv[i] = Vector2(tex[i]);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3 && j < t[i].size(); j++) {
				triindices[i][j] = t[i][j];
				if (i < texco.size()) texcoords[i][j] = texco[i][j];
			}
		} 
        
        
        
        nbones = names.size();

        bone_names = std::vector<std::string>(names);
        parent = std::vector<int>(par);
        children = std::vector<std::vector<int>>(nbones, std::vector<int>());
        
        // compute children
        for (int i = 0; i < parent.size(); i++) {
            if (parent[i] >= 0) children[parent[i]].push_back(i);
        }

        tips = std::vector<Vector3>(tipp);
        a_tips = std::unordered_set<int>(atips);

        rest_transforms = std::vector<Transform>(nbones);
        for (int i = 0; i < nbones; i++) rest_transforms[i] = Transform(transforms[i]);

        vertex_weights = std::vector<std::unordered_map<int, float>>(weights);


        init();
    }

    inline bool is_root(int i) {
        return parent[i] < 0;
    }


    void computeAbsoluteTransforms() {
        std::queue<int> q;
        // std::cout << nbones << " " << absolute_transforms.size() << " " << rest_transforms.size() << " " << absolute_tips.size() << " " << absolute_joints.size() << " " << tips.size() << "\n";
        for (int i = 0; i < nbones; i++) {
            if (is_root(i)) {
                q.push(i);
                absolute_transforms[i] = rest_transforms[i] * deformations[i];
                absolute_joints[i] = absolute_transforms[i].origin;
                absolute_tips[i] = absolute_transforms[i] * tips[i];
            }
        }

        while (q.size()) {
            int cur = q.front();
            q.pop();

            Transform cur_trans = absolute_transforms[cur];
            Vector3 cur_joint = absolute_joints[cur];
            // std::cout << cur << " = " << cur_trans.to_string() << " / " << cur_joint.to_string() << "\n";
            for (auto child : children[cur]) {
                absolute_transforms[child] = cur_trans * rest_transforms[child] * deformations[child];
                absolute_joints[child] = absolute_transforms[child].origin;
                absolute_tips[child] = absolute_transforms[child] * tips[child];
                q.push(child);
            }
        }
    }

    // QuOTD - I have a baby brother... (wow!) Blue has a baby brother... (she does?) I have a baby brother... I wonder who he is... (me too!) -Various (Blue's Room, 2006)
    void computeVertexLocations() {
        for (int i = 0; i < nverts; i++) {
            float sum = 0.0;
            Vector3 new_pos(0, 0, 0);
            for (auto p : vertex_weights[i]) {
                auto bone_index = p.first;
                auto weight = p.second;
                new_pos = new_pos + (absolute_transforms[bone_index] * vertex_displacements[i][bone_index]) * weight;
                // std::cout << weight << "\n";
                sum += weight;
            }
            if (!BASE::fzero(sum)) verts[i] = new_pos * 1.0 / sum;
        }

        setupvns();
    }

    std::vector<Mesh> visBones(float R = 0.2) {
        std::vector<Mesh> res;

        for (int i = 0; i < nbones; i++) {
            Mesh m = icosphere(R, 0);
	        m.Trans(Transform(absolute_joints[i]));
	        res.push_back(m);

	        Mesh m2 = icosphere(R * 0.5, 0);
	        m2.Trans(Transform(absolute_joints[i] * 0.75 + absolute_tips[i] * 0.25));
	        res.push_back(m2);
	        Mesh m3 = icosphere(R * 0.5, 0);
	        m3.Trans(Transform(absolute_joints[i] * 0.5 + absolute_tips[i] * 0.5));
	        res.push_back(m3);
        }
        return res;
    }

    inline void deform(std::string name, Transform t) {
        if (bone_names_inv.find(name) == bone_names_inv.end()) return;
        int index = bone_names_inv[name];
        deformations[index] = t;
    }
};

#endif