#include <string>
#include <cstdint>
#include <algorithm>

#include "../../include/utils/Math.h"
#include "../../include/physics/Constants.h"

#include "../../include/physics/RigidBody.h"

    RigidBody::RigidBody() {
        inertia = Matrix3::eye();
        inertia_inv = Matrix3::eye();
    }

    RigidBody::RigidBody(const RigidBody& other) {
        global_position = other.global_position;
        global_rotation = other.global_rotation;
        global_velocity = other.global_velocity;
        global_omega = other.global_omega;
        global_acceleration = other.global_acceleration;
        global_alpha = (other.global_alpha);
        global_net_force = other.global_net_force;
        global_net_torque = other.global_net_torque;
        gravity = other.gravity;
        linear_damping = other.linear_damping;
        angular_damping = other.angular_damping;
        mass_inv = other.mass_inv;
        mass = other.mass;

        inertia = other.inertia;
        inertia_inv = other.inertia_inv;

        global_inertia = other.global_inertia;
        global_inertia_inv = other.global_inertia_inv;
        awake = other.awake;
        motion = other.motion;
    }

    // Set the tensor
    void RigidBody::set_inertia(Matrix3 m) {
        inertia = m;
        inertia_inv = m.inv();
    }

    void RigidBody::set_global_transform() {
        transform = Transform(global_position, global_rotation.toRotation());
    }

    void RigidBody::set_global_inertia(Matrix3 basis) {
        global_inertia = basis * inertia * basis.trans();
    }

    void RigidBody::set_mass(float f) {
        if (BASE::fzero(f)) {
            mass = mass_inv = 0;
            return;
        }
        mass = f;
        mass_inv = 1.0 / mass;
    }

    bool RigidBody::infinite_mass() {
        return BASE::fzero(mass_inv);
    }

    // Compute derived values, such as the transform
    void RigidBody::compute_derived() {
        set_global_transform();
        set_global_inertia(transform.basis);

        motion = global_velocity.normsquared() + global_omega.normsquared();
    }

    void RigidBody::resetForces() { // You can do some basic force calculations here. For more generalization there is ForceGenerator.
        global_net_force = gravity * mass;
        global_net_torque.override(0, 0, 0);
    }

    // Force (direction, vector) in local coordinates, point in body coordinates
    void RigidBody::add_local_force_local_point(Vector3 f, Vector3 p) {
        if (p == VEC3_ZERO) {
            global_net_force = global_net_force + transform.basis * f;
            return;
        }
        add_global_force_global_point(transform.basis * f, transform * p);
    }

    // Force in world coordinates, point in body coordinates
    void RigidBody::add_global_force_local_point(Vector3 f, Vector3 p) {
        if (p == VEC3_ZERO) {
            global_net_force = global_net_force + f;
            return;
        }
        add_global_force_global_point(f, transform * p);
    }

    // Both force and point in global position
    void RigidBody::add_global_force_global_point(Vector3 f, Vector3 p) {
        Vector3 disp = p - global_position;
        global_net_force = global_net_force + f;
        global_net_torque = global_net_torque + (disp.cross(f));
    }

    void RigidBody::integrate(float delta) {
        resetForces();
    }

    std::pair<Vector3, Vector3> RigidBody::integrate_first_half(float delta) {
        // 0. compute forces (External)
        auto A_t = global_net_force * mass_inv;
        auto AL_t = inertia_inv * global_net_torque;

        std::cout << "1. " << global_net_force.to_string() << " " << global_net_torque.to_string() << "\n";
        std::cout << "2. " << A_t.to_string() << " " << AL_t.to_string() << "\n";

        // 1. v(t + 0.5dt) : v(t) + 0.5A(t) dt
        auto halfv = global_velocity + A_t * 0.5 * delta;
        auto halfo = global_omega + AL_t * 0.5 * delta;

        // 2. x(t + dt) : x(t) + v(t + 0.5dt) dt
        global_position = global_position + halfv * delta;
        global_rotation = global_rotation.addScaled(halfo, delta).normalized();
        global_acceleration = A_t;
        return {halfv, halfo};
    }

    void RigidBody::integrate_second_half(float delta, std::pair<Vector3, Vector3> vo) {
        if (vo.first == NILVEC3) vo.first = global_velocity;
        if (vo.second == NILVEC3) vo.second = global_omega;

        // 3. compute new forces
        auto A_tprime = global_net_force * mass_inv;
        auto AL_tprime = inertia_inv * global_net_torque;

        std::cout << "3. " << global_net_force.to_string() << " " << global_net_torque.to_string() << "\n";
        std::cout << "4. " << A_tprime.to_string() << " " << AL_tprime.to_string() << "\n";

        // 4. v(t + dt) = halfv + 0.5A(t + dt)dt

        global_velocity = vo.first + A_tprime * 0.5 * delta;
        global_omega = vo.second + AL_tprime * 0.5 * delta;

        global_acceleration = A_tprime;
        global_alpha = AL_tprime;
    }
