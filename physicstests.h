#ifndef PHYSTESTS_VSC_PHYS
#define PHYSTESTS_VSC_PHYS

#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

#include "texture_texture.h"
#include "main_utils.h"

using namespace std;

// VSC 9.90A - Particle
void Particle0() {
	Scene s = scene_blank();

	Particle p;
	p.set_mass(40);
	p.global_velocity = Vector3(1, 1, 0);
	p.global_position = Vector3(0, 0, -2);

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 48;

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = Transform(p.global_position) * original.inv();
		s.meshes[0]->Trans(transform);
		std::cout << p.to_string() << "\n";
		std::cout << s.meshes[0]->transform.to_string() << "\n";
		s.render();

		p.resetForces();
		auto v = p.integrate_first_half(1.0 / 64.0); // The return value is halfv, which is to be placed into the second half.
		p.resetForces();
		p.integrate_second_half(1.0 / 64.0, v);
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

		std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.90B - Particle groups and force generators.
void Particle1() {
	Scene s = scene_blank();

	Particle p;
	p.set_mass(40);
	p.global_velocity = Vector3(1, 1, 0);
	p.global_position = Vector3(0, 0, -2);

	ParticleGroup pg;

	pg.addParticle(p);

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 48;

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = Transform(pg.particles[0]->global_position) * original.inv();
		s.meshes[0]->Trans(transform);
		std::cout << p.to_string() << "\n";
		std::cout << s.meshes[0]->transform.to_string() << "\n";
		s.render();

		pg.integrate_forces();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

		std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}


// VSC 9.90C - Interactions between particles (also a force generator test)
void Particle2() {
	Scene s = scene_blank();

	Particle p;
	p.set_mass(1);
	p.global_position = Vector3(0, 0, -2);
	p.gravity = Vector3(0, 0, 0);

	ParticleGroup pg;

	pg.addParticle(p);
	p.global_position = Vector3(0.8, 0.6, -2);
	p.gravity = Vector3(0, PHYS_CONST::g, 0);
	pg.addParticle(p); // second particle has spring force


	// SPRING FORCE!!!

	ParticleSpringForce psf = ParticleSpringForce(pg.particles[0], 1024.0f);

	pg.forces.push_back({&psf, 1}); // Force 0 (spring force) affects particle 1 (with gravity)

	std::cout << pg.particles.size() << " ";
	std::cout << pg.forces.size() << "\n";

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);
	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 96;

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = Transform(pg.particles[0]->global_position) * original.inv();
		s.meshes[0]->Trans(transform);

		Transform original2 = s.meshes[1]->transform;
		Transform transform2 = Transform(pg.particles[1]->global_position) * original2.inv();
		s.meshes[1]->Trans(transform2);

		s.render();

		pg.integrate_forces();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

		std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.90 - RAY

// VSC 9.90D - Springs between two particles
void RotatingSpring() {
	Scene s = scene_blank();

	Particle p;
	p.set_mass(1);
	p.global_position = Vector3(-0.5, 0, -2);
	p.gravity = Vector3(0, 0, 0);
	p.global_velocity = Vector3(-1, 1, 0) * 1;

	ParticleGroup pg;

	pg.addParticle(p);
	p.global_position = Vector3(0.5, 0, -2);
	p.global_velocity = Vector3(1, -1, 0) * 2;
	pg.addParticle(p); // second particle has spring force


	// SPRING FORCE!!!

	ParticleSpringForce psf1 = ParticleSpringForce(pg.particles[0],64.0f);
	ParticleSpringForce psf2 = ParticleSpringForce(pg.particles[1],64.0f);

	pg.forces.push_back({&psf1, 1}); // Force 0 (spring force) affects particle 1 (with gravity)
	pg.forces.push_back({&psf2, 0}); // Force 0 (spring force) affects particle 1 (with gravity)

	std::cout << pg.particles.size() << " ";
	std::cout << pg.forces.size() << "\n";

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);
	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 192;

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = Transform(pg.particles[0]->global_position) * original.inv();
		s.meshes[0]->Trans(transform);

		Transform original2 = s.meshes[1]->transform;
		Transform transform2 = Transform(pg.particles[1]->global_position) * original2.inv();
		s.meshes[1]->Trans(transform2);

		s.render();

		pg.integrate_forces();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

		std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.91A-B - EMMA

// VSC 9.91A - Simple Ground Collision
void GroundCollisionTest() {
	Scene s = scene_blank();

	Particle p;
	p.set_mass(1);
	p.global_position = Vector3(-0.5, 0, -2);;

	ParticleGroup pg;

	pg.addParticle(p);
	p.global_position = Vector3(0.5, 0, -2);
	p.global_velocity = Vector3(0, 1, 0);
	pg.addParticle(p); // second particle has spring force

	ParticleEngine engine;
	engine.particle_group = &pg;

	// Add a collision

	GroundCollision gc;
	gc.groundPos = -0.5;
	gc.particles = engine.particle_group->particles;
	engine.generators.push_back(&gc);

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);
	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 64;

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = Transform(pg.particles[0]->global_position) * original.inv();
		s.meshes[0]->Trans(transform);

		Transform original2 = s.meshes[1]->transform;
		Transform transform2 = Transform(pg.particles[1]->global_position) * original2.inv();
		s.meshes[1]->Trans(transform2);

		s.render();

		engine.integrate_and_run();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.91B - Ground Collision with Spring Force
void GroundSpring() {
	Scene s = scene_blank();

	Particle p;
	p.set_mass(1);
	p.global_position = Vector3(-0.5, 0, -2);;

	ParticleGroup pg;

	pg.addParticle(p);
	p.global_position = Vector3(0.5, 0, -2);
	p.global_velocity = Vector3(0, 1, 0);
	pg.addParticle(p); // second particle has spring force

	// but wait! there's more!

	ParticleSpringForce psf1 = ParticleSpringForce(pg.particles[0],64.0f);
	ParticleSpringForce psf2 = ParticleSpringForce(pg.particles[1],64.0f);

	pg.forces.push_back({&psf1, 1}); // Force 0 (spring force) affects particle 1 (with gravity)
	pg.forces.push_back({&psf2, 0}); // Force 0 (spring force) affects particle 1 (with gravity)

	ParticleEngine engine;
	engine.particle_group = &pg;

	// Add a collision

	GroundCollision gc;
	gc.groundPos = -0.5;
	gc.particles = vec_slice(engine.particle_group->particles, 0, 0);
	engine.generators.push_back(&gc);

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);
	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 192;

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = Transform(pg.particles[0]->global_position) * original.inv();
		s.meshes[0]->Trans(transform);

		Transform original2 = s.meshes[1]->transform;
		Transform transform2 = Transform(pg.particles[1]->global_position) * original2.inv();
		s.meshes[1]->Trans(transform2);

		s.render();

		engine.integrate_and_run();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.91C - Cable/Rod!
void CableTest() {
	Scene s = scene_blank();

	Particle p;
	p.set_mass(1);
	p.global_position = Vector3(-0.5, 0, -2);;

	ParticleGroup pg;

	pg.addParticle(p);
	p.global_position = Vector3(0.5, 0, -2);
	p.global_velocity = Vector3(0, -4, 0);
	pg.addParticle(p); // second particle has spring force

	ParticleEngine engine;
	engine.particle_group = &pg;

	// Add a collision

	GroundCollision gc;
	gc.groundPos = -0.5;
	gc.particles = vec_slice(engine.particle_group->particles, 0, 0);
	engine.generators.push_back(&gc);

	// Add a cable

	Rod cable;
	cable.particles[0] = engine.particle_group->particles[0];
	cable.particles[1] = engine.particle_group->particles[1];

	cable.length = 1;

	LinkGen linkgen;
	linkgen.link = &cable;
	engine.generators.push_back(&linkgen);

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);
	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 128;

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = Transform(pg.particles[0]->global_position) * original.inv();
		s.meshes[0]->Trans(transform);

		Transform original2 = s.meshes[1]->transform;
		Transform transform2 = Transform(pg.particles[1]->global_position) * original2.inv();
		s.meshes[1]->Trans(transform2);

		s.render();

		engine.integrate_and_run();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		if (i == LEN * 3 / 4) engine.get_particle(0)->apply_impulse(Vector3(0, 16, 0));

		std::cout << "FRAME " << i << "DONE\n";
	}

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.91D - Advanced cables/rods
void SquareRods() {
	Scene s = scene_blank();

	Particle p;
	p.set_mass(1);
	p.global_position = Vector3(0, 0, -2);;

	ParticleGroup pg;

	pg.addParticle(p);
	p.global_position = Vector3(0, 1, -2);
	p.global_velocity = Vector3(0, -4, 0);
	pg.addParticle(p); // second particle has spring force
	p.global_position = Vector3(1, 1, -2);
	p.global_velocity = Vector3(0, 0, -1);
	pg.addParticle(p); 
	p.global_position = Vector3(1, 0, -2);
	pg.addParticle(p); 

	ParticleEngine engine;
	engine.particle_group = &pg;

	// Add a collision

	GroundCollision gc;
	gc.groundPos = -0.5;
	gc.particles = vec_slice(engine.particle_group->particles, 0, 0);
	engine.generators.push_back(&gc);

	// Add a cables
	Rod rods[6];
	LinkGen linkgens[6];
	for (int i = 0; i < 4; i++) {
		rods[i].particles[0] = engine.get_particle(i);
		rods[i].particles[1] = engine.get_particle((i + 1) % 4);
		rods[i].length = 1;

		linkgens[i].link = &rods[i];
		engine.generators.push_back(&linkgens[i]);
	}

	for (int i = 4; i < 6; i++) {
		rods[i].particles[0] = engine.get_particle(i % 4);
		rods[i].particles[1] = engine.get_particle((i + 2) % 4);
		rods[i].length = sqrtf(2);
		linkgens[i].link = &rods[i];
		engine.generators.push_back(&linkgens[i]);
	}

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);
	s.addMesh(&m);
	s.addMesh(&m);
	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 96;

	for (int i = 0; i < LEN; i++) {
		for (int i = 0; i < 4; i++) {
			Transform original = s.meshes[i]->transform;
			Transform transform = Transform(pg.particles[i]->global_position) * original.inv();
			s.meshes[i]->Trans(transform);
		}



		s.render();

		engine.integrate_and_run();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		if (i == LEN * 3 / 4) {
			engine.get_particle(0)->apply_impulse(Vector3(0, 16, 0));
		}

		std::cout << "FRAME " << i << "DONE\n";
	}

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.92A - Rigid Body, direct force application
void RigidBody0() {
	Scene s = scene_blank();

	RigidBody p;
	p.set_mass(40);
	p.set_inertia(Inertia::sphere_solid(p.mass, 0.1));
	p.global_velocity = Vector3(1, 1, 0);
	p.global_position = Vector3(0, 0, -2);

	// p.global_omega = Vector3(0, 0, 1);

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 48;

	p.compute_derived(); // THIS IS VERY IMPORTANT -- IT MAKES SURE THE TRANSFORMS ARE CORRECT

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = p.transform * original.inv();
		s.meshes[0]->Trans(transform);
		s.render();

		p.resetForces();

		// Add forces here (do this 2x)

		p.add_global_force_local_point(Vector3(8, 0, 0) * p.mass, Vector3(0, 0, 0));

		// Done adding forces

		auto v = p.integrate_first_half(1.0 / 64.0); // The return value is halfv, which is to be placed into the second half.
		p.compute_derived(); // compute after integration

		std::cout << "X. " << v.first.to_string() << " " << v.second.to_string() << "\n";
		
		p.resetForces();

		// Add forces here (do this 2x)

		p.add_global_force_local_point(Vector3(8, 0, 0) * p.mass, Vector3(0, 0, 0));

		// Done adding forces

		p.integrate_second_half(1.0 / 64.0, v);
		p.compute_derived(); // compute after integration
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

		std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.92B - Rigid body, direct torque application
void RigidBody1() {
	Scene s = scene_blank();

	RigidBody p;
	p.set_mass(40);
	p.set_inertia(Inertia::sphere_solid(p.mass, 0.1));
	p.global_velocity = Vector3(0, 1, 0);
	p.global_position = Vector3(0, 0, -2);

	// p.global_omega = Vector3(0, 0, 1);

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 48;

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = p.transform * original.inv();
		s.meshes[0]->Trans(transform);
		s.render();

		p.resetForces();
		p.compute_derived(); // THIS IS VERY IMPORTANT -- IT MAKES SURE THE TRANSFORMS ARE CORRECT
		// Add forces here (do this 2x)

		bool NEUTRALIZE = false;
		float COEFF = NEUTRALIZE ? 1.5 : 3;

		p.add_global_force_global_point(Vector3(COEFF, 0, 0) * p.mass, p.global_position + Vector3(0, -0.1, 0));
		if (NEUTRALIZE) p.add_global_force_global_point(Vector3(-COEFF, 0, 0) * p.mass, p.global_position + Vector3(0, 0.1, 0));

		// Done adding forces

		auto v = p.integrate_first_half(1.0 / 64.0); // The return value is halfv, which is to be placed into the second half.

		std::cout << "X. " << v.first.to_string() << " " << v.second.to_string() << "\n";
		
		p.resetForces();
		p.compute_derived(); // compute BEFORE integration

		// Add forces here (do this 2x)

		p.add_global_force_global_point(Vector3(COEFF, 0, 0) * p.mass, p.global_position + Vector3(0, -0.1, 0));
		if (NEUTRALIZE) p.add_global_force_global_point(Vector3(-COEFF, 0, 0) * p.mass, p.global_position + Vector3(0, 0.1, 0));

		// Done adding forces

		p.integrate_second_half(1.0 / 64.0, v);
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

		std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.921 - Rigid body, Force generators
void RigidBody2() {
	Scene s = scene_blank();

	RigidBody p;
	p.set_mass(40);
	p.set_inertia(Inertia::sphere_solid(p.mass, 0.1));
	p.global_velocity = Vector3(0, 1, 0);
	p.global_position = Vector3(0, 1, -2);

	// p.global_omega = Vector3(0, 0, 1);

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 96;

	// Spring force!

	RigidBodyPointSpringForce psf = RigidBodyPointSpringForce(Vector3(0, 0, -2), 1024.0f, 1.0f);

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = p.transform * original.inv();
		s.meshes[0]->Trans(transform);
		s.render();

		p.resetForces();
		p.compute_derived(); // THIS IS VERY IMPORTANT -- IT MAKES SURE THE TRANSFORMS ARE CORRECT

		// Add forces here (do this 2x)

		psf.applyForce(&p);

		// Done adding forces

		auto v = p.integrate_first_half(1.0 / 64.0); // The return value is halfv, which is to be placed into the second half.

		std::cout << "X. " << v.first.to_string() << " " << v.second.to_string() << "\n";
		
		p.resetForces();
		p.compute_derived(); // compute BEFORE integration
		// Add forces here (do this 2x)

		psf.applyForce(&p);

		// Done adding forces

		p.integrate_second_half(1.0 / 64.0, v);
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

	std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.922 - Rigid body group
void RigidBody3() {
	Scene s = scene_blank();

	RigidBody p;
	p.set_mass(40);
	p.set_inertia(Inertia::sphere_solid(p.mass, 0.1));
	p.global_velocity = Vector3(0, 1, 0);
	p.global_position = Vector3(0, 0, -2);

	RigidBodyGroup pg;

	pg.addRigidBody(p);

	Mesh m = icosphere(0.1, 0);

	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 48;

	for (int i = 0; i < LEN; i++) {
		Transform original = s.meshes[0]->transform;
		Transform transform = Transform(pg.bodies[0]->global_position) * original.inv();
		s.meshes[0]->Trans(transform);
		std::cout << s.meshes[0]->transform.to_string() << "\n";
		s.render();

		pg.integrate_forces();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

		std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}



#endif