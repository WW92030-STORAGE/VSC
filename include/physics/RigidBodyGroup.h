#ifndef RIGIDBODYGROUP_VSC_PHYS
#define RIGIDBODYGROUP_VSC_PHYS

#include "RigidBody.h"
#include "Constants.h"
#include "ForceGenerator.h"


#include <vector>
#include <iostream>

/*

Unlike Scene, the RigidBodyGroup does not own the force generators. The generators hold pointers to ForceGenerators.

*/

struct RigidBodyGroup {
    std::vector<RigidBody*> bodies;
    std::vector<std::pair<RigidBodyForceGenerator*, int>> forces; // index of a force generator, index of a particle

    RigidBodyGroup();

    RigidBodyGroup(const RigidBodyGroup& other);

    ~RigidBodyGroup();

    // convention: force 0 is universal gravity. we can use a default to do this.
    void addRigidBody(RigidBody p);

    void compute_forces(float delta = PHYS_CONST::DELTA_DEF);

    void integrate_forces(float delta = PHYS_CONST::DELTA_DEF);
};

#endif