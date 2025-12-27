#include <string>
#include <algorithm>
#include <cstdint>
#include <vector>

#include "../../include/utils/Math.h"

#include "../../include/physics/Constants.h"

#include "../../include/physics/Particle.h"

/*

An infinitely small, negligible-rotation, particle mass.

*/
    Particle::Particle() {}

    Particle::Particle(const Particle& other) {
        global_position = other.global_position;
        global_velocity = other.global_velocity;
        global_acceleration = other.global_acceleration;
        global_net_force = other.global_net_force;
        gravity = other.gravity;
        natural_damping = other.natural_damping;
        mass_inv = other.mass_inv;
        mass = other.mass;
    }

    void Particle::resetForces() { // You can do some basic force calculations here. For more generalization there is ForceGenerator.
        global_net_force = gravity * mass;
    }

    void Particle::set_mass(float f) {
        if (BASE::fzero(f)) {
            mass = mass_inv = 0;
            return;
        }
        mass = f;
        mass_inv = 1.0 / mass;
    }

    // In each integration step: first zero forces, then compute, then do this next:

    Vector3 Particle::integrate_first_half(float delta) {
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

    void Particle::integrate_second_half(float delta, Vector3 halfv) {
        if (halfv == NILVEC3) halfv = global_velocity;

        // 3. compute new forces
        // linear
        auto A_tprime = global_net_force * mass_inv;

        // 4. v(t + dt) = halfv + 0.5A(t + dt)dt

        global_velocity = halfv + A_tprime * 0.5 * delta;

        global_acceleration = A_tprime;
    }

    void Particle::apply_impulse(Vector3 impulse) {
        global_velocity = global_velocity + impulse * mass_inv;
    }

    std::string Particle::to_string() {
        std::string res = "Particle[x=" + global_position.to_string() + ", v=" + global_velocity.to_string() + ", a=" + global_acceleration.to_string() + ", m=" + std::to_string(mass);
        res = res + ", g=" + gravity.to_string() + ", d=" + std::to_string(natural_damping);
        res = res + ", F=" + global_net_force.to_string();
        
        res = res + "]";
        return res;
    }