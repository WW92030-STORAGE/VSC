#include "../../include/physics/Constants.h"
#include "../../include/physics/Particle.h"

#include "../../include/physics/ParticleForceGenerator.h"

// ParticleForceGenerator

    void ParticleForceGenerator::applyForce(Particle* particle, float delta) {
        particle->global_net_force = particle->global_net_force + Vector3(0, PHYS_CONST::g * particle->mass, 0);
    }

// ParticleGravityForce

    void ParticleGravityForce::applyForce(Particle* particle, float delta) {
        particle->global_net_force = particle->global_net_force + Vector3(0, gravity * particle->mass, 0);
    }

// Spring fixed to another particle

    ParticleSpringForce::ParticleSpringForce(Particle* p, float r, float l) {
        other = p;
        k = r;
        restLen = l;
    }

    void ParticleSpringForce::applyForce(Particle* particle, float delta) {
        Vector3 disp = other->global_position - particle->global_position;
        float LEN = disp.length();
        particle->global_net_force = particle->global_net_force + disp * (k * (LEN - restLen) / LEN);
    }

// Spring fixed to another particle
    ParticlePointSpringForce::ParticlePointSpringForce(Vector3 p, float r, float l) {
        other = p;
        k = r;
        restLen = l;
    }

    void ParticlePointSpringForce::applyForce(Particle* particle, float delta) {
        Vector3 disp = other - particle->global_position;
        float LEN = disp.length();
        particle->global_net_force = particle->global_net_force + disp * (k * (LEN - restLen) / LEN);
    }
