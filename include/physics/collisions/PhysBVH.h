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
        return volume.overlaps(other->volume);
    }
    
    // oh brother
    uint64_t getPossibleContacts(PossibleCollision* pcs, uint64_t limit) const {
        if (!limit || isLeaf()) return 0;

        return children[0]->getPossibleContactsWith(children[1], pcs, limit);
    }

    // recursive utility helper
    uint64_t getPossibleContactsWith(PhysBVHNode* node, PossibleCollision* pcs, uint64_t limit) {
        if (!overlaps(node) || !limit) return 0;
        if (isLeaf() && node->isLeaf()) {
            pcs->bodies[0] = body;
            pcs->bodies[1] = node->body;
            return 1;
        }

        bool noleaves = !isLeaf() && !node->isLeaf();

        if (node->isLeaf() || (noleaves && volume.getSize() >= node->volume.getSize())) {
            uint64_t count = children[0]->getPossibleContactsWith(node, pcs, limit);
            if (limit > count) return count + children[1]->getPotentialContactsWith(node, pcs + count, limit - count);
            return count;
        } else {
            uint64_t count = getPossibleContactsWith(node->children[0], pcs, limit);
            if (limit > count) return count + getPossibleContactsWith(node->children[1], pcs + count, limit - count);
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
};

template <class BV>
static std::vector<std::string> PhysBVHNode_preorder(PhysBVHNode<BV>* node) {
    std::vector<std::string> res;
    if (!node) return res;

    PhysBVHNode_preorder_recur(node, res, 0);
    return res;
}

template <class BV>
static void PhysBVHNode_preorder_recur(PhysBVHNode<BV>* node, std::vector<std::string>& v, int depth = 0) {
    if (!node) return;
    std::string ds = std::to_string(depth) + ": ";
    if (node->isLeaf()) {
        v.push_back(ds + "RigidBody/" + node->volume.to_string());
    } else {
        v.push_back(ds + node->volume.to_string());
        PhysBVHNode_preorder_recur(node->children[0], v, depth + 1);
        PhysBVHNode_preorder_recur(node->children[1], v, depth + 1);
    }
}

#endif