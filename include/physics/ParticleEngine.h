#ifndef PARTICLE_ENGINE_VSC_PHYS
#define PARTICLE_ENGINE_VSC_PHYS

#include "../utils/Math.h"

#include <vector>
#include <algorithm>
#include <cstdint>
#include <climits>

#include "Constants.h"
#include "Particle.h"
#include "ParticleCollision.h"
#include "ParticleGroup.h"


/*

Mass-Aggregate engine. A ParticleGroup suffices to show FORCES between particles but here we add the ability to manage COLLISIONS between particles.

*/

// System to generate collision information en masse
struct ParticleCollisionGen {
    virtual uint64_t addCollision(ParticleCollision *collision, uint64_t limit) const = 0;
};

// Main engine

struct ParticleEngine {
    ParticleGroup* particle_group; // serves as the particle list and the forces listing.
    ParticleCollisionResolver resolver; // Resolver for collisions 
    std::vector<ParticleCollisionGen*> generators; // Generators for collisions
    ParticleCollision* collisions; // collisions
    uint64_t maxCollisions = 65536; // max collisions
    uint64_t maxIter = 0;
    ParticleEngine(uint64_t maxCOLLISION = 65536, uint64_t maxITER = 0);

    ~ParticleEngine();

    Particle* get_particle(int index);

    void start_frame();

    // tries to create collisions, how many are done?
    uint64_t generate_collisions();
    void integrate_and_run(float delta = PHYS_CONST::DELTA_DEF);
};



struct GroundCollision : public ParticleCollisionGen {
    std::vector<Particle*> particles;
    float groundPos = 0.0f;

    virtual uint64_t addCollision(ParticleCollision *collision, uint64_t limit) const;
};

struct LinkGen : public ParticleCollisionGen {
    ParticleLink* link = 0;

    LinkGen();
    LinkGen(ParticleLink* LINK);
    virtual uint64_t addCollision(ParticleCollision *collision, uint64_t limit) const;
};


#endif