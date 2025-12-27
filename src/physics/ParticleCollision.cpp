#include "../../include/utils/Math.h"
#include "../../include/physics/Constants.h"
#include "../../include/physics/Particle.h"

#include <vector>
#include <algorithm>
#include <cstdint>
#include <climits>

#include "../../include/physics/ParticleCollision.h"

// Particle collisions. Particles are treated as small spheres for now.

    // Collisions are resolved in increasing order of separating velocity
    void ParticleCollision::resolve(float delta) {
        resolve_velocity(delta);
        resolve_penetration(delta);
    }

    // separating velocity = contact normal * velocity of primary relative to secondary
    float ParticleCollision::compute_separating_velocity() const {
        Vector3 relv = particles[0]->global_velocity;
        if (particles[1]) relv = relv - particles[1]->global_velocity;
        return relv.dot(normal);
    }

    void ParticleCollision::resolve_velocity(float delta) {
        float v_s = compute_separating_velocity();
        if (v_s > 0) { // moving away from each other, nothing to do
            return;
        }

        float v_sprime = -v_s * restitution; // update the separating velocity

        // Collisions involving very small velocities can be neutralized

        Vector3 accel_buildup = particles[0]->global_acceleration;
        if (particles[1]) accel_buildup = accel_buildup - particles[1]->global_acceleration;

        float v_induced = (accel_buildup * normal) * delta;

        // remove any push-together from the buildup
        if (v_induced < 0) {
            v_sprime += restitution * v_induced;
            v_sprime = std::max(v_sprime, 0.0f);
        }

        float dv_s = v_sprime - v_s; // change in v_s

        // change in momentum weighted based on inverses of masses:

        float minv_sum = particles[0]->mass_inv;
        if (particles[1]) minv_sum += particles[1]->mass_inv;

        if (minv_sum <= 0) return; // absurd degeneration

        float impulse = dv_s / minv_sum;

        Vector3 impulse_unit = normal * impulse;

        particles[0]->global_velocity = particles[0]->global_velocity + impulse_unit * particles[0]->mass_inv;
        if (particles[1]) particles[1]->global_velocity = particles[1]->global_velocity - impulse_unit * particles[1]->mass_inv;
    }

    void ParticleCollision::resolve_penetration(float delta) {
        if (penetration <= 0) return;

        float minv_sum = particles[0]->mass_inv;
        if (particles[1]) minv_sum += particles[1]->mass_inv;
        if (minv_sum <= 0) return; // absurd degeneration

        Vector3 unit_displacement = normal * (penetration / minv_sum);

        displacements[0] = unit_displacement * particles[0]->mass_inv;

        particles[0]->global_position = particles[0]->global_position + displacements[0];
        if (particles[1]) {
            displacements[1] = unit_displacement * -particles[1]->mass_inv;
            particles[1]->global_position = particles[1]->global_position + displacements[1];
        }
    }

    std::string ParticleCollision::to_string() {
        std::string res = "ParticleCollision[NP=" + std::to_string(particles[1] ? 2 : 1) + ", R=" + std::to_string(restitution);
        res = res + ", N=" + normal.to_string() + ", P=" + std::to_string(penetration) + ", V_S=" + std::to_string(compute_separating_velocity()) + " | ";
        if (particles[0]) res = res + "P0=" + particles[0]->to_string();
        if (particles[1]) res = res + " P1=" + particles[1]->to_string();
        res = res + " | ";
        if (particles[0]) res = res + "D0=" + displacements[0].to_string();
        if (particles[1]) res = res + " D1=" + displacements[1].to_string();
        res = res + "]";
        return res;
    }

    ParticleCollisionResolver::ParticleCollisionResolver(uint64_t value) {
        MAX_ITER = value;
    }

    void ParticleCollisionResolver::resolveCollisions(ParticleCollision* collisions, uint64_t size, float delta, bool UPDATE_PEN) {
        // std::cout << "RESOLVING " << size << " COLLISIONS\n";
        ITERS = 0;
        for (; ITERS < MAX_ITER; ITERS++) {
            // Find the minimum severity
            int index = -1;
            float MIN_VS = 0;
            for (int i = 0; i < size; i++) {
                // std::cout << collisions[i].to_string() << "\n";
                float v_s = collisions[i].compute_separating_velocity();
                // std::cout << i << ": V_S = " << v_s << "\n";
                if (v_s < MIN_VS) {
                    MIN_VS = v_s;
                    index = i;
                }
            }

            // std::cout << ITERS << ": " << index << "\n";

            if (index >= 0) collisions[index].resolve(delta);
            else break;

            // Update penetration values

            if (UPDATE_PEN) {
                Vector3* disp = collisions[index].displacements;

                for (int i = 0; i < size; i++) {
                    if (collisions[i].particles[0] == collisions[index].particles[0]) {
                        collisions[i].penetration -= disp[0] * collisions[i].normal;
                    }
                    else if (collisions[i].particles[0] == collisions[index].particles[1]) {
                        collisions[i].penetration -= disp[1] * collisions[i].normal;
                    }

                    if (!collisions[i].particles[1]) continue;
                    if (collisions[i].particles[1] == collisions[index].particles[0]) {
                        collisions[i].penetration += disp[0] * collisions[i].normal;
                    }
                    else if (collisions[i].particles[1] == collisions[index].particles[1]) {
                        collisions[i].penetration += disp[1] * collisions[i].normal;
                    }
                }
            }
        }
    }