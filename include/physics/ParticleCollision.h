#ifndef COLLISION_VSC_PHYS
#define COLLISION_VSC_PHYS

#include "../utils/Math.h"
#include "Constants.h"
#include "Particle.h"

#include <vector>
#include <algorithm>
#include <cstdint>
#include <climits>

// Particle collisions. Particles are treated as small spheres for now.

struct ParticleCollision {
    Particle* particles[2] = {0, 0}; // Everything is given from the FIRST particle's perspective. So the normal points towards the first particle, as if a normal force from the second.

    float restitution = 1; // coefficient of restitution

    Vector3 normal; // Collision normal

    float penetration = 0; // positive for colliding/intersecting objects

    Vector3 displacements[2];

    // Collisions are resolved in increasing order of separating velocity
    void resolve(float delta = PHYS_CONST::DELTA_DEF);

    // separating velocity = contact normal * velocity of primary relative to secondary
    float compute_separating_velocity() const;

    void resolve_velocity(float delta = PHYS_CONST::DELTA_DEF);

    void resolve_penetration(float delta = PHYS_CONST::DELTA_DEF);

    std::string to_string();
};

struct ParticleCollisionResolver {
    uint64_t MAX_ITER = 1024;
    uint64_t ITERS = 0; // for profiling

    ParticleCollisionResolver(uint64_t value = 4);

    void resolveCollisions(ParticleCollision* collisions, uint64_t size, float delta = PHYS_CONST::DELTA_DEF, bool UPDATE_PEN = true);
};


#endif