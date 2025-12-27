#include "../../include/physics/RigidBody.h"
#include "../../include/physics/Constants.h"

#include "../../include/physics/ForceGenerator.h"

    void GravityForce::applyForce(RigidBody *body, float delta) {
        if (body->infinite_mass()) return;
        body->global_net_force = body->global_net_force + gravity * body->mass;

        // body->add_global_force_local_point(gravity * body->mass, VEC3_ZERO);
    }

    RigidBodySpringForce::RigidBodySpringForce(RigidBody* p, float r, float l) {
        other = p;
        k = r;
        restLen = l;
    }
    
    void RigidBodySpringForce::applyForce(RigidBody* body, float delta) {
        Vector3 disp = other->global_position - body->global_position;
        float LEN = disp.length();
        body->global_net_force = body->global_net_force + disp * (k * (LEN - restLen) / LEN);
    }


    
    RigidBodyPointSpringForce::RigidBodyPointSpringForce(Vector3 p, float r, float l) {
        other = p;
        k = r;
        restLen = l;
    }

    void RigidBodyPointSpringForce::applyForce(RigidBody* body, float delta) {
        Vector3 disp = other - body->global_position;
        float LEN = disp.length();
        body->global_net_force = body->global_net_force + disp * (k * (LEN - restLen) / LEN);
    }