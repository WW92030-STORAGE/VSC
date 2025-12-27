#ifndef PARTICLEFORCEGEN_VSC_PHYS
#define PARTICLEFORCEGEN_VSC_PHYS

#include "Constants.h"
#include "Particle.h"

struct ParticleForceGenerator {
    virtual void applyForce(Particle* particle, float delta = PHYS_CONST::DELTA_DEF);
};

struct ParticleGravityForce : public ParticleForceGenerator {
    float gravity = PHYS_CONST::g;

    virtual void applyForce(Particle* particle, float delta = PHYS_CONST::DELTA_DEF);
};

// Spring fixed to another particle
struct ParticleSpringForce : public ParticleForceGenerator {
    Particle* other;
    float restLen = 1;
    float k = 1;

    ParticleSpringForce(Particle* p, float r = 1, float l = 1);

    virtual void applyForce(Particle* particle, float delta = PHYS_CONST::DELTA_DEF);
};

// Spring fixed to another particle
struct ParticlePointSpringForce : public ParticleForceGenerator {
    Vector3 other;
    float restLen = 1;
    float k = 1;

    ParticlePointSpringForce(Vector3 p, float r = 1, float l = 1);

    virtual void applyForce(Particle* particle, float delta = PHYS_CONST::DELTA_DEF);
};

#endif