#ifndef PARTICLEFORCEGEN_VSC_PHYS
#define PARTICLEFORCEGEN_VSC_PHYS

#include "Constants.h"
#include "Particle.h"

#include <iostream>

struct ParticleForceGenerator {
    virtual void applyForce(Particle* particle, float delta = PHYS_CONST::DELTA_DEF) {
        particle->global_net_force = particle->global_net_force + Vector3(0, PHYS_CONST::g * particle->mass, 0);
    }
};

struct ParticleGravityForce : public ParticleForceGenerator {
    float gravity = PHYS_CONST::g;

    virtual void applyForce(Particle* particle, float delta = PHYS_CONST::DELTA_DEF) {
        particle->global_net_force = particle->global_net_force + Vector3(0, gravity * particle->mass, 0);
    }
};

// Spring fixed to another particle
struct ParticleSpringForce : public ParticleForceGenerator {
    Particle* other;
    float restLen = 1;
    float k = 1;

    ParticleSpringForce(Particle* p, float r = 1, float l = 1) {
        other = p;
        k = r;
        restLen = l;
    }

    virtual void applyForce(Particle* particle, float delta = PHYS_CONST::DELTA_DEF) {
        Vector3 disp = other->global_position - particle->global_position;
        float LEN = disp.length();
        particle->global_net_force = particle->global_net_force + disp * (k * (LEN - restLen) / LEN);
    }
};

// Spring fixed to another particle
struct ParticlePointSpringForce : public ParticleForceGenerator {
    Vector3 other;
    float restLen = 1;
    float k = 1;

    ParticlePointSpringForce(Vector3 p, float r = 1, float l = 1) {
        other = p;
        k = r;
        restLen = l;
    }

    virtual void applyForce(Particle* particle, float delta = PHYS_CONST::DELTA_DEF) {
        Vector3 disp = other - particle->global_position;
        float LEN = disp.length();
        particle->global_net_force = particle->global_net_force + disp * (k * (LEN - restLen) / LEN);
    }
};

#endif