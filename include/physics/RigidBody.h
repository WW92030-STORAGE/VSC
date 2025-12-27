#ifndef RIGIDBODY_VSC_PHYS
#define RIGIDBODY_VSC_PHYS

#include <string>
#include <cstdint>
#include <algorithm>

#include "../utils/Math.h"
#include "Constants.h"

#include <iostream>

struct RigidBody {
    Vector3 global_position = Vector3(0, 0, 0);
    Quaternion global_rotation = Quaternion(1, 0, 0, 0);

    Vector3 global_velocity = Vector3(0, 0, 0);
    Vector3 global_omega = Vector3(0, 0, 0); // angular velocity

    Vector3 global_acceleration = Vector3(0, 0, 0);
    Vector3 global_alpha = Vector3(0, 0, 0);
    Vector3 global_net_force = Vector3(0, 0, 0);
    Vector3 global_net_torque = Vector3(0, 0, 0);

    Vector3 gravity = Vector3(0, PHYS_CONST::g, 0);
    float linear_damping = 1; // how much velocity remains
    float angular_damping = 1; // How much omega remains
    float mass_inv = 1.0; // the quantity 1 / mass.
    float mass = 1.0; // the quantity mass

    Matrix3 inertia_inv; // inverse inertia tensor
    Matrix3 inertia;

    Matrix3 global_inertia;
    Matrix3 global_inertia_inv; // same thing in world coords

    // Derived values: these are not used for heavy duty stuff but rather for [REDACTED]

    Transform transform;
    float motion = 0;
    bool awake = true;

    RigidBody();

    RigidBody(const RigidBody& other);

    // Set the tensor
    void set_inertia(Matrix3 m);

    void set_global_transform();

    void set_global_inertia(Matrix3 basis);

    void set_mass(float f = 1);

    bool infinite_mass();

    // Compute derived values, such as the transform
    void compute_derived();

    void resetForces();

    // Force (direction, vector) in local coordinates, point in body coordinates
    void add_local_force_local_point(Vector3 f, Vector3 p);

    // Force in world coordinates, point in body coordinates
    void add_global_force_local_point(Vector3 f, Vector3 p);

    // Both force and point in global position
    void add_global_force_global_point(Vector3 f, Vector3 p);
    
    void integrate(float delta = PHYS_CONST::DELTA_DEF);

    std::pair<Vector3, Vector3> integrate_first_half(float delta = PHYS_CONST::DELTA_DEF);

    void integrate_second_half(float delta = PHYS_CONST::DELTA_DEF, std::pair<Vector3, Vector3> vo = {NILVEC3, NILVEC3});
};

#endif