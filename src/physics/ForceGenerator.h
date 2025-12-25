#ifndef RIGIDBODYFORCEGEN_VSC_PHYS
#define RIGIDBODYFORCEGEN_VSC_PHYS

#include "RigidBody.h"
#include "Constants.h"

struct RigidBodyForceGenerator {
    virtual void applyForce(RigidBody *body, float delta = PHYS_CONST::DELTA_DEF) = 0;
};

class GravityForce : public RigidBodyForceGenerator {
    Vector3 gravity = Vector3(0, PHYS_CONST::g, 0);

    virtual void applyForce(RigidBody *body, float delta = PHYS_CONST::DELTA_DEF) {
        if (body->infinite_mass()) return;
        body->global_net_force = body->global_net_force + gravity * body->mass;

        // body->add_global_force_local_point(gravity * body->mass, VEC3_ZERO);
    }
};


// Spring fixed to another body
struct RigidBodySpringForce : public RigidBodyForceGenerator {
    RigidBody* other;
    float restLen = 1;
    float k = 1;

    RigidBodySpringForce(RigidBody* p, float r = 1, float l = 1) {
        other = p;
        k = r;
        restLen = l;
    }

    virtual void applyForce(RigidBody* body, float delta = PHYS_CONST::DELTA_DEF) {
        Vector3 disp = other->global_position - body->global_position;
        float LEN = disp.length();
        body->global_net_force = body->global_net_force + disp * (k * (LEN - restLen) / LEN);
    }
};

// Spring fixed to point
struct RigidBodyPointSpringForce : public RigidBodyForceGenerator {
    Vector3 other;
    float restLen = 1;
    float k = 1;

    RigidBodyPointSpringForce(Vector3 p, float r = 1, float l = 1) {
        other = p;
        k = r;
        restLen = l;
    }

    virtual void applyForce(RigidBody* body, float delta = PHYS_CONST::DELTA_DEF) {
        Vector3 disp = other - body->global_position;
        float LEN = disp.length();
        body->global_net_force = body->global_net_force + disp * (k * (LEN - restLen) / LEN);
    }
};


#endif