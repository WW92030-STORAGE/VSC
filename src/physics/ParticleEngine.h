#ifndef PARTICLE_ENGINE_VSC_PHYS
#define PARTICLE_ENGINE_VSC_PHYS

#include "../utils/Math.h"

#include <vector>
#include <algorithm>
#include <cstdint>
#include <climits>

#include "Constants.h"
#include "Particle.h"
#include "ParticleCollision.h"
#include "ParticleGroup.h"


/*

Mass-Aggregate engine. A ParticleGroup suffices to show FORCES between particles but here we add the ability to manage COLLISIONS between particles.

*/

// System to generate collision information en masse
struct ParticleCollisionGen {
    virtual uint64_t addCollision(ParticleCollision *collision, uint64_t limit) const = 0;
};

// Main engine

struct ParticleEngine {
    ParticleGroup* particle_group; // serves as the particle list and the forces listing.
    ParticleCollisionResolver resolver; // Resolver for collisions 
    std::vector<ParticleCollisionGen*> generators; // Generators for collisions
    ParticleCollision* collisions; // collisions
    uint64_t maxCollisions = 65536; // max collisions
    uint64_t maxIter = 0;
    ParticleEngine(uint64_t maxCOLLISION = 65536, uint64_t maxITER = 0) {
        maxIter = maxITER;
        maxCollisions = maxCOLLISION;
        collisions = new ParticleCollision[maxCollisions];
    }

    ~ParticleEngine() {
        delete[] collisions;
    }

    inline Particle* get_particle(int index) { return particle_group->particles[index]; }

    inline void start_frame() {
        for (auto i : particle_group->particles) i->resetForces();
    }

    // tries to create collisions, how many are done?
    uint64_t generate_collisions() {
        uint64_t limit = maxCollisions;
        ParticleCollision* collision = collisions;

        // generate a collision
        for (auto gen : generators) {
            // std::cout << "LIMIT " << limit << "\n";
            uint64_t used = gen->addCollision(collision, limit);
            // std::cout << "END CONTACT ADD " << used << "\n";
            collision += used;
            limit -= used;
            if (limit <= 0) break;
        }

        // std::cout << "ADDED COLLISIONS " << maxCollisions - limit << ": \n";
        // for (int i = 0; i < maxCollisions - limit; i++) std::cout << (collisions + i)->to_string() << "\n";

        return maxCollisions - limit;
    }

    void integrate_and_run(float delta = PHYS_CONST::DELTA_DEF) {
        std::cout << "BEGIN RUN STEP\n";
        particle_group->integrate_forces();

        for (auto p : particle_group->particles) std::cout << &p << " ";
        std::cout << "\n";

        std::cout << "BEGIN COLLISION STEP\n";

        uint64_t generated_collisions = generate_collisions();
        std::cout << "COLLISIONS: " << generated_collisions << "\n";

        if (maxIter <= 0) resolver.MAX_ITER = generated_collisions<<1;
        else resolver.MAX_ITER = maxIter;

        std::cout << "MAX_ITERS: " << resolver.MAX_ITER << "\n";

        resolver.resolveCollisions(collisions, generated_collisions, delta);
    }
};



struct GroundCollision : public ParticleCollisionGen {
    std::vector<Particle*> particles;
    float groundPos = 0.0f;

    virtual uint64_t addCollision(ParticleCollision *collision, uint64_t limit) const {
        uint64_t count = 0;
        for (auto p : particles) {
            float y = p->global_position.y;
            if (y < groundPos) {
                // std::cout << "PARTICLE COLLISION DETECTED " << p << ": " << p->to_string() << " " << groundPos << "\n";
                collision->normal = Vector3(0, 1, 0);
                collision->particles[0] = p;
                collision->particles[1] = 0;
                collision->penetration = groundPos - y;
                collision->restitution = 0.2;
                collision++;
                count++;
            }
            if (count >= limit) return count;
        }
        return count;
    }
};

struct LinkGen : public ParticleCollisionGen {
    ParticleLink* link = 0;

    LinkGen() { }
    LinkGen(ParticleLink* LINK) { link = LINK; }
    virtual uint64_t addCollision(ParticleCollision *collision, uint64_t limit) const {
        if (!link) return 0;
        uint64_t count = 0;

        count += link->create_collision(collision, limit);

        return count;
    }
};


#endif