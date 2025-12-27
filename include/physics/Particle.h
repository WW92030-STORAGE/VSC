#ifndef PARTICLE_VSC_PHYS
#define PARTICLE_VSC_PHYS

#include <string>
#include <algorithm>
#include <cstdint>
#include <vector>

#include "../utils/Math.h"

#include "Constants.h"

/*

An infinitely small, negligible-rotation, particle mass.

*/

struct Particle {
    Vector3 global_position = Vector3(0, 0, 0);
    Vector3 global_velocity = Vector3(0, 0, 0);

    Vector3 global_acceleration = Vector3(0, 0, 0);
    Vector3 global_net_force = Vector3(0, 0, 0);

    Vector3 gravity = Vector3(0, PHYS_CONST::g, 0);
    float natural_damping = 1; // how much velocity remains

    float mass_inv = 1.0; // the quantity 1 / mass.
    float mass = 1.0; // the quantity mass

    Particle();

    Particle(const Particle& other);

    void resetForces();

    void set_mass(float f = 1);

    // In each integration step: first zero forces, then compute, then do this next:

    Vector3 integrate_first_half(float delta = PHYS_CONST::DELTA_DEF);

    // Then zero and compute forces, then do this last:

    void integrate_second_half(float delta = PHYS_CONST::DELTA_DEF, Vector3 halfv = NILVEC3);

    void apply_impulse(Vector3 impulse);

    std::string to_string();
};

#endif