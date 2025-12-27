#include "../../include/physics/RigidBody.h"
#include "../../include/physics/Constants.h"
#include "../../include/physics/ForceGenerator.h"


#include <vector>
#include <iostream>

#include "../../include/physics/RigidBodyGroup.h"

/*

Unlike Scene, the RigidBodyGroup does not own the force generators. The generators hold pointers to ForceGenerators.

*/

    RigidBodyGroup::RigidBodyGroup() {

    }

    RigidBodyGroup::RigidBodyGroup(const RigidBodyGroup& other) {
        bodies = std::vector<RigidBody*>(other.bodies.size());
        for (int i = 0; i < bodies.size(); i++) bodies[i] = new RigidBody(*(other.bodies[i]));

        forces = other.forces;
    }

    RigidBodyGroup::~RigidBodyGroup() {
        for (auto p : bodies) delete p;
    }

    // convention: force 0 is universal gravity. we can use a default to do this.
    void RigidBodyGroup::addRigidBody(RigidBody p) {
        bodies.push_back(new RigidBody(p));
    }

    void RigidBodyGroup::compute_forces(float delta) {
        int NP = bodies.size();
        int NF = forces.size();

        // 0. Establish the current transform
        for (int i = 0; i < NP; i++) bodies[i]->compute_derived();

        // 1. zero and compute forces
        for (int i = 0; i < NP; i++) bodies[i]->resetForces();

        for (int i = 0; i < NF; i++) forces[i].first->applyForce(bodies[forces[i].second], delta);        

        // for (int i = 0; i < NP; i++) std::cout << i << " = " << bodies[i]->global_net_force.to_string() << "\n";
    } 

    void RigidBodyGroup::integrate_forces(float delta) {
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
