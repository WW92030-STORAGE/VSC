#include "../../include/utils/Math.h"

#include <vector>
#include <algorithm>
#include <cstdint>
#include <climits>

#include "../../include/physics/Constants.h"
#include "../../include/physics/Particle.h"
#include "../../include/physics/ParticleCollision.h"
#include "../../include/physics/ParticleGroup.h"
#include "../../include/physics/ParticleLink.h"


#include "../../include/physics/ParticleEngine.h"

#include <iostream>

/*

Mass-Aggregate engine. A ParticleGroup suffices to show FORCES between particles but here we add the ability to manage COLLISIONS between particles.

*/

// System to generate collision information en masse
// see header

// Main engine

    ParticleEngine::ParticleEngine(uint64_t maxCOLLISION, uint64_t maxITER) {
        maxIter = maxITER;
        maxCollisions = maxCOLLISION;
        collisions = new ParticleCollision[maxCollisions];
    }

    ParticleEngine::~ParticleEngine() {
        delete[] collisions;
    }

    Particle* ParticleEngine::get_particle(int index) { return particle_group->particles[index]; }

    void ParticleEngine::start_frame() {
        for (auto i : particle_group->particles) i->resetForces();
    }

    // tries to create collisions, how many are done?
    uint64_t ParticleEngine::generate_collisions() {
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

    void ParticleEngine::integrate_and_run(float delta) {
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

    // Special collision types

    uint64_t GroundCollision::addCollision(ParticleCollision *collision, uint64_t limit) const {
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



    LinkGen::LinkGen() { }
    LinkGen::LinkGen(ParticleLink* LINK) { link = LINK; }
    uint64_t LinkGen::addCollision(ParticleCollision *collision, uint64_t limit) const {
        if (!link) return 0;
        uint64_t count = 0;

        count += link->create_collision(collision, limit);

        return count;
    }