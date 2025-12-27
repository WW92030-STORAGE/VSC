#ifndef PARTICLE_LINK_VSC_PHYS
#define PARTICLE_LINK_VSC_PHYS

#include "../utils/Math.h"
#include "Constants.h"
#include "Particle.h"
#include "ParticleCollision.h"

struct ParticleLink {
    Particle* particles[2] = {0, 0};

    float get_length() const;

    virtual uint64_t create_collision(ParticleCollision* collision, uint64_t limit = 1) const = 0;
};

struct Cable : public ParticleLink {
    float length = 1;
    float restitution = 1;

    virtual uint64_t create_collision(ParticleCollision* collision, uint64_t limit = 1) const;
};

struct Rod : public ParticleLink {
    float length = 1;
    virtual uint64_t create_collision(ParticleCollision* collision, uint64_t limit = 1) const;
};

#endif