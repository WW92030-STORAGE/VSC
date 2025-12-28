#include "../../include/physics/RigidBody.h"
#include "../../include/physics/Constants.h"

#include "../../include/physics/ForceGenerator.h"

    void GravityForce::applyForce(RigidBody *body, float delta) {
        if (body->infinite_mass()) return;
        body->global_net_force = body->global_net_force + gravity * body->mass;

        // body->add_global_force_local_point(gravity * body->mass, VEC3_ZERO);
    }

    RigidBodySpringForce::RigidBodySpringForce(RigidBody* p, float r, float l, Vector3 o0, Vector3 o1) {
        other = p;
        k = r;
        restLen = l;
        offset0 = o0;
        offset1 = o1;
    }
    
    void RigidBodySpringForce::applyForce(RigidBody* body, float delta) {
        Vector3 true_other_pos = other->global_position + other->transform.basis * offset1;
        Vector3 true_body_pos = body->global_position + body->transform.basis * offset0;
        Vector3 disp = true_other_pos - true_body_pos;
        float LEN = disp.length();
        // body->global_net_force = body->global_net_force + disp * (k * (LEN - restLen) / LEN);
        body->add_global_force_global_point(disp * (k * (LEN - restLen) / LEN), true_body_pos);
    }


    
    RigidBodyPointSpringForce::RigidBodyPointSpringForce(Vector3 p, float r, float l, Vector3 o) {
        other = p;
        k = r;
        restLen = l;
        offset = o;
    }

    void RigidBodyPointSpringForce::applyForce(RigidBody* body, float delta) {
        Vector3 true_body_pos = body->global_position + body->transform.basis * offset;
        Vector3 disp = other - true_body_pos;
        float LEN = disp.length();
        // body->global_net_force = body->global_net_force + disp * (k * (LEN - restLen) / LEN);
        body->add_global_force_global_point(disp * (k * (LEN - restLen) / LEN), true_body_pos);
    }