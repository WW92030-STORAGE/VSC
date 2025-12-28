#ifndef RIGIDBODYFORCEGEN_VSC_PHYS
#define RIGIDBODYFORCEGEN_VSC_PHYS

#include "RigidBody.h"
#include "Constants.h"

struct RigidBodyForceGenerator {
    virtual void applyForce(RigidBody *body, float delta = PHYS_CONST::DELTA_DEF) = 0;
};

class GravityForce : public RigidBodyForceGenerator {
    Vector3 gravity = Vector3(0, PHYS_CONST::g, 0);

    virtual void applyForce(RigidBody *body, float delta = PHYS_CONST::DELTA_DEF);
};


// Spring fixed to another body. The offset0 is where on the current body we affect, offset2 where on the other body we fixate to.
// All offsets are in local psace.
struct RigidBodySpringForce : public RigidBodyForceGenerator {
    RigidBody* other;
    float restLen = 1;
    float k = 1;
    Vector3 offset0 = Vector3(0, 0, 0);
    Vector3 offset1 = Vector3(0, 0, 0);

    RigidBodySpringForce(RigidBody* p, float r = 1, float l = 1, Vector3 o0 = Vector3(0, 0, 0), Vector3 o1 = Vector3(0, 0, 0));

    virtual void applyForce(RigidBody* body, float delta = PHYS_CONST::DELTA_DEF);
};

// Spring fixed to point
struct RigidBodyPointSpringForce : public RigidBodyForceGenerator {
    Vector3 other;
    float restLen = 1;
    float k = 1;
    Vector3 offset = Vector3(0, 0, 0);

    RigidBodyPointSpringForce(Vector3 p, float r = 1, float l = 1, Vector3 o = Vector3(0, 0, 0));

    virtual void applyForce(RigidBody* body, float delta = PHYS_CONST::DELTA_DEF);
};


#endif