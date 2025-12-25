#ifndef PARTICLE_LINK_VSC_PHYS
#define PARTICLE_LINK_VSC_PHYS

#include "../utils/Math.h"
#include "Constants.h"
#include "Particle.h"
#include "ParticleCollision.h"

struct ParticleLink {
    Particle* particles[2] = {0, 0};

    float get_length() const {
        Vector3 disp = particles[0]->global_position - particles[1]->global_position;
        return disp.length();
    }

    virtual uint64_t create_collision(ParticleCollision* collision, uint64_t limit = 1) const = 0;
};

struct Cable : public ParticleLink {
    float length = 1;
    float restitution = 1;

    virtual uint64_t create_collision(ParticleCollision* collision, uint64_t limit = 1) const {
        float len = get_length();

        if (len < length) return 0;

        for (int i = 0; i < 2; i++) collision->particles[i] = particles[i];

        collision->normal = (particles[1]->global_position - particles[0]->global_position).normalized();
        collision->penetration = len - length;
        collision->restitution = restitution;

        return 1;
    }
};

struct Rod : public ParticleLink {
    float length = 1;
    virtual uint64_t create_collision(ParticleCollision* collision, uint64_t limit = 1) const {
        float len = get_length();
        if (BASE::fequal(len, length)) return 0;

        for (int i = 0; i < 2; i++) collision->particles[i] = particles[i];

        collision->normal = (particles[1]->global_position - particles[0]->global_position).normalized();
        collision->restitution = 0;
        collision->penetration = len - length;

        if (len < length) {
            collision->normal = collision->normal * -1;
            collision->penetration *= -1;
        }

        return 1;

    }
};

#endif