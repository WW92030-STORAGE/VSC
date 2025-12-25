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

    Particle() {}

    Particle(const Particle& other) {
        global_position = other.global_position;
        global_velocity = other.global_velocity;
        global_acceleration = other.global_acceleration;
        global_net_force = other.global_net_force;
        gravity = other.gravity;
        natural_damping = other.natural_damping;
        mass_inv = other.mass_inv;
        mass = other.mass;
    }

    void resetForces() { // You can do some basic force calculations here. For more generalization there is ForceGenerator.
        global_net_force = gravity * mass;
    }

    inline void set_mass(float f = 1) {
        if (BASE::fzero(f)) {
            mass = mass_inv = 0;
            return;
        }
        mass = f;
        mass_inv = 1.0 / mass;
    }

    // In each integration step: first zero forces, then compute, then do this next:

    Vector3 integrate_first_half(float delta = PHYS_CONST::DELTA_DEF) {
        // 0. compute forces (External)
        auto A_t = global_net_force * mass_inv;
        // 1. v(t + 0.5dt) : v(t) + 0.5A(t) dt
        auto halfv = global_velocity + A_t * 0.5 * delta;

        // 2. x(t + dt) : x(t) + v(t + 0.5dt) dt
        global_position = global_position + halfv * delta;
        global_acceleration = A_t;
        return halfv;
    }

    // Then zero and compute forces, then do this last:

    void integrate_second_half(float delta = PHYS_CONST::DELTA_DEF, Vector3 halfv = NILVEC3) {
        if (halfv == NILVEC3) halfv = global_velocity;

        // 3. compute new forces
        // linear
        auto A_tprime = global_net_force * mass_inv;

        // 4. v(t + dt) = halfv + 0.5A(t + dt)dt

        global_velocity = halfv + A_tprime * 0.5 * delta;

        global_acceleration = A_tprime;
    }

    void apply_impulse(Vector3 impulse) {
        global_velocity = global_velocity + impulse * mass_inv;
    }

    std::string to_string() {
        std::string res = "Particle[x=" + global_position.to_string() + ", v=" + global_velocity.to_string() + ", a=" + global_acceleration.to_string() + ", m=" + std::to_string(mass);
        res = res + ", g=" + gravity.to_string() + ", d=" + std::to_string(natural_damping);
        res = res + ", F=" + global_net_force.to_string();
        
        res = res + "]";
        return res;
    }
};

#endif