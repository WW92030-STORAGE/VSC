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

    ParticleGroup();

    ParticleGroup(const ParticleGroup& other);

    ~ParticleGroup();

    // convention: force 0 is universal gravity. we can use a default to do this.
    void addParticle(Particle p);

    void compute_forces(float delta = PHYS_CONST::DELTA_DEF);

    void integrate_forces(float delta = PHYS_CONST::DELTA_DEF);
};

#endif