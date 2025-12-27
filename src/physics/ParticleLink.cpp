#include "../../include/utils/Math.h"
#include "../../include/physics/Constants.h"
#include "../../include/physics/Particle.h"
#include "../../include/physics/ParticleCollision.h"

#include "../../include/physics/ParticleLink.h"

    float ParticleLink::get_length() const {
        Vector3 disp = particles[0]->global_position - particles[1]->global_position;
        return disp.length();
    }

// Cable

    uint64_t Cable::create_collision(ParticleCollision* collision, uint64_t limit) const {
        float len = get_length();

        if (len < length) return 0;

        for (int i = 0; i < 2; i++) collision->particles[i] = particles[i];

        collision->normal = (particles[1]->global_position - particles[0]->global_position).normalized();
        collision->penetration = len - length;
        collision->restitution = restitution;

        return 1;
    }

// Rod

    uint64_t Rod::create_collision(ParticleCollision* collision, uint64_t limit) const {
        float len = get_length();
        if (BASE::fequal(len, length)) return 0;

        for (int i = 0; i < 2; i++) collision->particles[i] = particles[i];

        collision->normal = (particles[1]->global_position - particles[0]->global_position).normalized();
        collision->restitution = 0;
        collision->penetration = len - length;

        if (len < length) {
            collision->normal = collision->normal * -1;
            collision->penetration *= -1;
        }

        return 1;

    }
