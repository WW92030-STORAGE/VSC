#ifndef BOUNDINGVOLUMEHIERARCHY_VSC_PHYC
#define BOUNDINGVOLUMEHIERARCHY_VSC_PHYC

#include <cstdint>
#include <cmath>
#include <string>
#include <vector>

#include "BoundingVolume.h"
#include "BoundingVolumeTests.h"
#include "../RigidBody.h"
#include <iostream>
/*

BVH system and system for storing potential collisions

*/

struct PossibleCollision {
    RigidBody* bodies[2] = {0, 0};
};

// BV must generic on BoundingVolume
template <class BV>
struct PhysBVHNode {
    PhysBVHNode* children[2] = {0, 0};
    BV volume;
    RigidBody* body = 0;
    PhysBVHNode* parent = 0;

    PhysBVHNode() {
        volume = BV();
    }

    PhysBVHNode(PhysBVHNode<BV>* pp, const BV& vv, RigidBody* bb = 0) {
        parent = pp;
        children[0] = children[1] = 0;

        volume = BV(vv);
        body = bb;
    }

    bool isLeaf() const { 
        return body;
    }

    bool overlaps(const PhysBVHNode<BV>* other) const {
        return volume.overlaps(&(other->volume));
    }

    // oh brother
    uint64_t getPossibleContacts(PossibleCollision* pcs, uint64_t limit) const {
        if (!limit || isLeaf()) return 0;

        auto count = children[0]->getPossibleContactsWith(children[1], pcs, limit);
        if (limit > count) count += children[0]->getPossibleContacts(pcs + count, limit - count);
        if (limit > count) count += children[1]->getPossibleContacts(pcs + count, limit - count);
        return count;
    }

    // recursive utility helper
    uint64_t getPossibleContactsWith(PhysBVHNode* node, PossibleCollision* pcs, uint64_t limit, bool verbose = false) {
        if (verbose) std::cout << to_string() << " | " << node->to_string() << "\n";
        if (!overlaps(node) || !limit) {
            if (verbose) std::cout << "NO OVERLAP OR BAD LIMIT\n";
            return 0;
        }
        if (isLeaf() && node->isLeaf()) {
            if (verbose) std::cout << "BOTH LEAF\n";
            pcs->bodies[0] = body;
            pcs->bodies[1] = node->body;
            return 1;
        }

        if (node->isLeaf() || (!isLeaf() && (volume.getSize() >= node->volume.getSize()))) {
            if (verbose) std::cout << "L LEFT RECURSE\n";
            uint64_t count = children[0]->getPossibleContactsWith(node, pcs, limit);
            if (limit > count) {
                if (verbose) std::cout << "L RIGHT RECURSE\n";
                return count + children[1]->getPossibleContactsWith(node, pcs + count, limit - count);
            }
            return count;
        } else {
            if (verbose) std::cout << "R LEFT RECURSE\n";
            uint64_t count = getPossibleContactsWith(node->children[0], pcs, limit);
            if (limit > count) {
                if (verbose) std::cout << "R RIGHT RECURSE\n";
                return count + getPossibleContactsWith(node->children[1], pcs + count, limit - count);
            }
            return count;
        }
    }

    // Insert another node?

    void insert(RigidBody* b, BV& v) {
        if (isLeaf()) {
            // if currently leaf then we divert our current volume into a subtree and the new one into another.
            children[0] = new PhysBVHNode<BV>(this, volume, body);
            children[1] = new PhysBVHNode<BV>(this, v, b);
            this->body = 0;
            

            recomputeBV();
        } else { // recurse down
            float g0 = children[0]->volume.getGrowth(v);
            float g1 = children[1]->volume.getGrowth(v);
            if (g0 < g1) children[0]->insert(b, v);
            else children[1]->insert(b, v);
        }
    }

    void recomputeBV() {
        if (isLeaf()) return;
        volume.merge(&(children[0]->volume), &(children[1]->volume));

        if (parent) parent->recomputeBV();
    }

    ~PhysBVHNode() {
        if (parent) {
            PhysBVHNode* sibling = parent->children[0];
            if (this == parent->children[0]) sibling = parent->children[1];

            parent->volume = sibling->volume;
            parent->body = sibling->body;
            parent->children[0] = sibling->children[0];
            parent->children[1] = sibling->children[1];

            sibling->parent = 0;
            sibling->body = 0;
            sibling->children[0] = 0;
            sibling->children[1] = 0;
            delete sibling;

            parent->recomputeBV();
        }

        if (children[0]) {
            children[0]->parent = 0;
            delete children[0];
        }
        if (children[1]) {
            children[1]->parent = 0;
            delete children[1];
        }
    }

    std::string to_string() {
        std::string res = "";
        if (body) res = "RigidBody/";
        res += volume.to_string();
        return res;
    }
};

template <class BV>
static std::vector<BV> PhysBVHNode_preorder(PhysBVHNode<BV>* node) {
    std::vector<BV> res;
    if (!node) return res;

    PhysBVHNode_preorder_recur(node, res, 0);
    return res;
}

template <class BV>
static void PhysBVHNode_preorder_recur(PhysBVHNode<BV>* node, std::vector<BV>& v, int depth = 0) {
    if (!node) return;
    if (node->isLeaf()) {
        v.push_back(node->volume);
    } else {
        v.push_back(node->volume);
        PhysBVHNode_preorder_recur(node->children[0], v, depth + 1);
        PhysBVHNode_preorder_recur(node->children[1], v, depth + 1);
    }
}

template <class BV>
static std::vector<std::string> PhysBVHNode_preorder_str(PhysBVHNode<BV>* node) {
    std::vector<std::string> res;
    if (!node) return res;

    PhysBVHNode_preorder_str_recur(node, res, 0);
    return res;
}

template <class BV>
static void PhysBVHNode_preorder_str_recur(PhysBVHNode<BV>* node, std::vector<std::string>& v, int depth = 0) {
    if (!node) return;
    std::string ds = std::to_string(depth) + ": ";
    if (node->isLeaf()) {
        v.push_back(ds + node->to_string());
    } else {
        v.push_back(ds + node->to_string());
        PhysBVHNode_preorder_str_recur(node->children[0], v, depth + 1);
        PhysBVHNode_preorder_str_recur(node->children[1], v, depth + 1);
    }
}

#endif