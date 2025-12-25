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

    RigidBodyGroup() {

    }

    RigidBodyGroup(const RigidBodyGroup& other) {
        bodies = std::vector<RigidBody*>(other.bodies.size());
        for (int i = 0; i < bodies.size(); i++) bodies[i] = new RigidBody(*(other.bodies[i]));

        forces = other.forces;
    }

    ~RigidBodyGroup() {
        for (auto p : bodies) delete p;
    }

    // convention: force 0 is universal gravity. we can use a default to do this.
    void addRigidBody(RigidBody p) {
        bodies.push_back(new RigidBody(p));
    }

    void compute_forces(float delta = 1.0 / 64.0) {
        int NP = bodies.size();
        int NF = forces.size();

        // 0. Establish the current transform
        for (int i = 0; i < NP; i++) bodies[i]->compute_derived();

        // 1. zero and compute forces
        for (int i = 0; i < NP; i++) bodies[i]->resetForces();

        for (int i = 0; i < NF; i++) forces[i].first->applyForce(bodies[forces[i].second], delta);        

        // for (int i = 0; i < NP; i++) std::cout << i << " = " << bodies[i]->global_net_force.to_string() << "\n";
    } 

    void integrate_forces(float delta = PHYS_CONST::DELTA_DEF) {
        int NP = bodies.size();
        int NF = forces.size();


        // 1. zero and compute forces
        compute_forces(delta);

        std::pair<Vector3, Vector3> halfvs[NP];

        // 2. first integration step
        for (int i = 0; i < NP; i++) halfvs[i] = bodies[i]->integrate_first_half(delta);

        // 3. zero and compute forces
        compute_forces(delta);

        // 4. second integration step
        for (int i = 0; i < NP; i++) bodies[i]->integrate_second_half(delta, halfvs[i]);
    }
};

#endif