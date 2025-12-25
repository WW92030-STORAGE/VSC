#ifndef PARTICLEGROUP_VSC_PHYS
#define PARTICLEGROUP_VSC_PHYS

#include "Particle.h"
#include "Constants.h"
#include "ParticleForceGenerator.h"


#include <vector>
#include <iostream>

/*

Unlike Scene, the ParticleGroup does not own the force generators. The generators hold pointers to ParticleForceGenerators.

*/

struct ParticleGroup {
    std::vector<Particle*> particles;
    std::vector<std::pair<ParticleForceGenerator*, int>> forces; // index of a force generator, index of a particle

    ParticleGroup() {

    }

    ParticleGroup(const ParticleGroup& other) {
        particles = std::vector<Particle*>(other.particles.size());
        for (int i = 0; i < particles.size(); i++) particles[i] = new Particle(*(other.particles[i]));

        forces = other.forces;
    }

    ~ParticleGroup() {
        for (auto p : particles) delete p;
    }

    // convention: force 0 is universal gravity. we can use a default to do this.
    void addParticle(Particle p) {
        particles.push_back(new Particle(p));
    }

    void compute_forces(float delta = 1.0 / 64.0) {
        int NP = particles.size();
        int NF = forces.size();
        // 1. zero and compute forces
        for (int i = 0; i < NP; i++) particles[i]->resetForces();

        for (int i = 0; i < NF; i++) forces[i].first->applyForce(particles[forces[i].second], delta);        

        // for (int i = 0; i < NP; i++) std::cout << i << " = " << particles[i]->global_net_force.to_string() << "\n";
    } 

    void integrate_forces(float delta = PHYS_CONST::DELTA_DEF) {
        int NP = particles.size();
        int NF = forces.size();


        // 1. zero and compute forces
        compute_forces(delta);

        Vector3 halfvs[NP];

        // 2. first integration step
        for (int i = 0; i < NP; i++) halfvs[i] = particles[i]->integrate_first_half(delta);

        // 3. zero and compute forces
        compute_forces(delta);

        // 4. second integration step
        for (int i = 0; i < NP; i++) particles[i]->integrate_second_half(delta, halfvs[i]);
    }
};

#endif