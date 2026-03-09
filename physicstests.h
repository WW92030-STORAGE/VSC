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

// VSC 9.92 - NORMAN

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

// VSC 9.922A - Rigid body group
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

// VSC 9.922B - Rigid body group
void RigidBody4() {
	Scene s = scene_blank();

	const bool FIX_ONE = false;

	float R = 0.2;

	RigidBody p;
	p.set_mass(40);
	p.set_inertia(Inertia::sphere_solid(p.mass, R));
	p.global_velocity = Vector3(0, 4, 0);
	p.global_position = Vector3(-1, 0, -2);
	p.gravity = Vector3(0, 0, 0);

	RigidBodyGroup pg;

	pg.addRigidBody(p);
	if (FIX_ONE) {
		p.global_velocity = Vector3(0, 0, 0);
		p.global_position = Vector3(0, 0, -2);
	} else {
		p.global_velocity = Vector3(0, -4, 0);
		p.global_position = Vector3(1, 0, -2);
	}
	pg.addRigidBody(p);

	// Forces!

	RigidBodySpringForce rsf0(pg.bodies[1], 64 * p.mass, 1.0, Vector3(0, R, 0), Vector3(0, -R, 0));
	RigidBodySpringForce rsf1(pg.bodies[0], 64 * p.mass, 1.0, Vector3(0, -R, 0), Vector3(0, R, 0));
	pg.forces.push_back({&rsf0, 0}); // Remember: the index is which body is affected.
	if (!FIX_ONE) pg.forces.push_back({&rsf1, 1});

	Mesh m = icosphere(R, 0);

	s.addMesh(&m);
	s.addMesh(&m);

	s.render();
	std::cout << "Prepared\n";

	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	int LEN = 144;

	for (int i = 0; i < LEN; i++) {
		for (int mi = 0; mi < pg.bodies.size(); mi++) {
			Transform original = s.meshes[mi]->transform;
			Transform transform = pg.bodies[mi]->transform * original.inv();
			s.meshes[mi]->Trans(transform);
		// std::cout << s.meshes[mi]->transform.to_string() << "\n";
		}
		s.render();

		pg.integrate_forces();
		s.outputBuffer(VIDEO_PATH + "/frame" + std::to_string(i));

		std::cout << "FRAME " << i << "DONE\n";
	}

		std::ofstream len(VIDEO_PATH + "/LEN");
	len << LEN;
	len.close();
}

// VSC 9.93 - Bounding Volumes and Collision Detection (ORBIT)

// 9.93A - Bounding Volumes
void BoundingVolumes1() {
	Scene s = scene_blank(true);
	s.camera.Trans(Transform(Vector3(0, 0, 5)));
	BoundingSphere s1(Vector3(0, 0, 0), 2.9);
	BoundingSphere s2(Vector3(4, 0, 0), 1);
	cout << testBVCollision(s1, s2);

	BoundingAABB aabb1(Vector3(-2, -3, -4), Vector3(1, 2, 3));
	BoundingAABB aabb2(Vector3(0, 0, -6), Vector3(1.9, 1, 1));
	cout << aabb1.overlaps(&aabb2) << endl;

	Mesh m1 = rectprism(aabb1.halfrad * 2);
	m1.Trans(Transform(aabb1.position));
	Mesh m2 = rectprism(aabb2.halfrad * 2);
	m2.Trans(Transform(aabb2.position));

	BaseMaterial mat1(BASEMAT_RED);
	BaseMaterial mat2(BASEMAT_CYAN);
	s.addMesh(&m1, &mat1);
	s.addMesh(&m2, &mat2);
	std::cout << "Prepared\n";

	s.render();
	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";
}

// 9.93B - BVH Test 1
void PhysBVH1() {
	Scene s = scene_blank(true);
	s.camera.Trans(Transform(Vector3(8, 0, 10)));

	PointLight p(0);
	p.Trans(Vector3(8, 0, 10));
	s.lights.push_back(p);

	std::vector<RigidBody> rb(16, RigidBody());
	std::vector<BoundingAABB> bs(16);

	for (int i = 0; i < bs.size(); i++) bs[i] = BoundingAABB(Vector3(i, 0, 0), Vector3(0.6, 0.6, 0.6));

	PhysBVHNode<BoundingAABB>* node = new PhysBVHNode<BoundingAABB>(0, bs[0], &rb[0]);

	std::vector<int> indices({1, 2, 4, 6, 7, 8, 9, 12, 13, 14, 15});

	for (int ii = 0; ii < indices.size(); ii++) {
		int i = indices[(ii * 7) % indices.size()];
		node->insert(&rb[i], bs[i]);
	}

	auto v = PhysBVHNode_preorder_str(node);
	auto b = PhysBVHNode_preorder(node);
	std::vector<Mesh> meshes(b.size());
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i] = rectprism(b[i].halfrad * 2);
		meshes[i].Trans(b[i].position);
	}
	for (auto i : meshes) {
		s.drawMesh(i);
	}

	for (auto i : v) std::cout << i << "\n";

	std::cout << "Prepared\n";
	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete node;
}

// 9.93C - BVH Test 2
void PhysBVH2() {
	int N = 2;
	int k = 3;

	float size = 0.6;

	Scene s = scene_blank(true);
	s.camera.Trans(Transform(Vector3((N - 1) * k * 0.5, (N - 1) * k * 0.5, 10)));

	PointLight p(0);
	p.Trans(Vector3((N - 1) * k * 0.5, (N - 1) * k * 0.5, 10));
	s.lights.push_back(p);



	std::vector<RigidBody> rb(N * N * N, RigidBody());
	std::vector<BoundingAABB> bs(N * N * N);

	for (int i = 0; i < bs.size(); i++) {
		bs[i] = BoundingAABB(Vector3((i % N) * k, ((i / N) % N) * k, (i / (N * N)) * k), Vector3(size, size, size));
		std::cout << bs[i].position.to_string() << "\n";
	}

	PhysBVHNode<BoundingAABB>* node = new PhysBVHNode<BoundingAABB>(0, bs[0], &rb[0]);

	std::vector<int> indices;
	for (int i = 1; i < bs.size(); i++) indices.push_back(i);

	for (int ii = 0; ii < indices.size(); ii++) {
		int i = indices[(ii * 3) % indices.size()];
		node->insert(&rb[i], bs[i]);
	}

	auto v = PhysBVHNode_preorder_str(node);
	auto b = PhysBVHNode_preorder(node);
	std::vector<Mesh> meshes(b.size());
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i] = rectprism(b[i].halfrad * 2);
		meshes[i].Trans(b[i].position);
	}
	for (auto i : meshes) {
		s.drawMesh(i);
	}

	for (auto i : v) std::cout << i << "\n";

	std::cout << "Prepared\n";
	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	delete node;
}


// 9.931A - Using the BVH to check collisions
void PhysBVH3() {
	Scene s = scene_blank(true);
	s.camera.Trans(Transform(Vector3(8, 0, 10)));

	PointLight p(0);
	p.Trans(Vector3(8, 0, 10));
	s.lights.push_back(p);

	// begin customization. keep x coordinates within [0, 16] and y and z coordinates within [-8, 8] for best results, and have no two object positions identical.

	std::vector<RigidBody> rb(16, RigidBody());
	for (int i = 0; i < 16; i++) rb[i].global_position = Vector3(i, 0, 0);
	std::vector<BoundingAABB> bs(16);

	for (int i = 0; i < bs.size(); i++) bs[i] = BoundingAABB(Vector3(i, 0.1 - 0.2 * (i & 1), 0), Vector3(0.6, 0.6, 0.6));

	PhysBVHNode<BoundingAABB>* node = new PhysBVHNode<BoundingAABB>(0, bs[0], &rb[0]);

	// end customization
	// std::vector<int> indices({2, 4, 5, 6, 7, 9, 12, 13, 15});
	std::vector<int> indices({2, 3, 5, 7, 9, 10, 12, 15});
	for (int ii = 0; ii < indices.size(); ii++) {
		int i = indices[(ii * 3) % indices.size()];
		node->insert(&rb[i], bs[i]);
	}

	PossibleCollision pcs[1024];

	int count = node->getPossibleContacts(pcs, 1024);

	std::cout << "possibles " << count << "\n";

	for (int i = 0; i < count; i++) {
		std::cout << pcs[i].bodies[0]->global_position.to_string() << " | " << pcs[i].bodies[1]->global_position.to_string() << "\n";
	}

	// What is at risk?
	std::set<Vector3> isPossiblyColliding;  

	for (int i = 0; i < count; i++) {
		for (int k = 0; k < 2; k++) isPossiblyColliding.insert(pcs[i].bodies[k]->global_position);
	}

	for (int ii = 0; ii < indices.size(); ii++) {
		int i = indices[ii];
		auto boxshape = rectprism(bs[i].halfrad * 2, bs[i].position);
		BaseMaterial mat(BASEMAT_WHITE);
		if (isPossiblyColliding.count(rb[i].global_position)) mat = BaseMaterial(BASEMAT_RED);
		s.addMesh(&boxshape, &mat);
	}

	std::cout << "Prepared\n";
	s.render();
	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	auto v = PhysBVHNode_preorder_str(node);
	for (auto i : v) std::cout << i << "\n";

	delete node;
}

// 9.931B - Broad stress test
void PhysBVH4() {
	Scene s = scene_blank(true);
	s.camera.Trans(Transform(Vector3(8, 8, 10)));

	PointLight p(0);
	p.Trans(Vector3(8, 0, 10));
	s.lights.push_back(p);

	int N = 256; // (1<<16) for a real stress test
	// begin customization. keep x and y coordinates within [0, 16] and z coordinates within [-8, 8] for best results, and have no two object positions identical.

	MersenneTwister ms(3); // to keep things deterministic

	RigidBody* rb = new RigidBody[N];
	for (int i = 0; i < N; i++) {
		rb[i] = RigidBody();
		rb[i].global_position = Vector3(ms() * 16, ms() * 16, ms() * 16);
	}
	std::vector<BoundingAABB> bs(N);

	for (int i = 0; i < bs.size(); i++) bs[i] = BoundingAABB(rb[i].global_position, Vector3(0.6, 0.6, 0.6));

	PhysBVHNode<BoundingAABB>* node = new PhysBVHNode<BoundingAABB>(0, bs[0], &rb[0]);

	// end customization

	for (int i = 1; i < N; i++) {
		node->insert(&rb[i], bs[i]);
	}

	bool NAIVE = false; // use the naive method (quadratic) or the BVH? (BVH runs ~3x as fast for 1<<16 cubes.)

	

	int PCSZ = 1<<24;
	PossibleCollision* pcs = new PossibleCollision[PCSZ];

	int count = 0;

	if (!NAIVE) {

	count = node->getPossibleContacts(pcs, PCSZ);

	} else {
		for (int i = 0; i < N; i++) {
			for (int j = i + 1; j < N; j++) {
				if (bs[i].overlaps(&bs[j])) {
					pcs[count].bodies[0] = &rb[i];
					pcs[count].bodies[1] = &rb[j];
					count++;
				}
			}
		}
	}

	std::cout << "possibles " << count << "\n";

	bool DRAW = true;

	if (DRAW) {

	// What is at risk?
	std::set<Vector3> isPossiblyColliding;  

	for (int i = 0; i < count; i++) {
		for (int k = 0; k < 2; k++) isPossiblyColliding.insert(pcs[i].bodies[k]->global_position);
	}

	for (int i = 0; i < N; i++) {
		auto boxshape = rectprism(bs[i].halfrad * 2, bs[i].position);
		BaseMaterial mat(BASEMAT_WHITE);
		if (isPossiblyColliding.count(rb[i].global_position)) mat = BaseMaterial(BASEMAT_RED);
		s.addMesh(&boxshape, &mat);
	}

	std::cout << "Prepared\n";
	s.render();
	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	}

	delete node;

	delete[] rb;
	delete[] pcs;

}

void BoundingVolumeFromCollisionTest() {
	CollisionBox box(Vector3(), Vector3(1, 1, 1), Quaternion(1, 0.2, 0.4, 0.6).normalized());
	BoundingAABB aabb = CollisionBoxToBoundingAABB(box);
	cout << aabb.to_string() << endl;
}

// 9.932A - Bounding Volumes and Collision Volumes
void CollisionSphereTest() {
	Scene s = scene_blank(true);
	s.camera.Trans(Transform(Vector3(0, 0, 5)));

	PointLight p(0);
	p.Trans(Vector3(0, 0, 10));
	s.lights.push_back(p);

	CollisionSphere sphere1(Vector3(0, 0, 0), 1);
	auto bounding1 = CollisionSphereToBoundingAABB(sphere1);
	Mesh b1 = rectprism(bounding1.halfrad * 2, bounding1.position);

	Vector3 dir = Vector3(-8, -4, 1).normalized() * 1.6; // change me
	CollisionSphere sphere2(dir, 1);
	auto bounding2 = CollisionSphereToBoundingAABB(sphere2);
	Mesh b2 = rectprism(bounding2.halfrad * 2, bounding2.position);

	bool DRAW = true;

	if (DRAW) {

	cout << "BV " << bounding2.overlaps(&bounding1) << endl;
	CollisionData overlap = checkCollision(sphere1, sphere2);
	cout << overlap.to_string() << "\n";

	BaseMaterial mat(BASEMAT_RED);
	if (overlap.exists()) mat = BaseMaterial(BASEMAT_GREEN);

	Mesh m = sphere1.getMesh();
	s.addMesh(&m, &mat);
	Mesh m2 = sphere2.getMesh();
	s.addMesh(&m2, &mat);

	std::cout << "Prepared\n";
	s.render();
	s.drawMesh(b1);
	s.drawMesh(b2);
	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	}

}

// 9.932B
void CollisionBoxTest() {
	Scene s = scene_blank(true);
	s.camera.Trans(Transform(Vector3(0, 0, 5)));

	PointLight p(0);
	p.Trans(Vector3(0, 0, 10));
	s.lights.push_back(p);

	Vector3 box1_pos(0, 0, 0);
	// box1_pos = Vector3(0.3, -1.6, 0);
	CollisionBox box1(box1_pos, Vector3(1, 1, 1), Quaternion(1, 0.2, 0.4, 0.6).normalized());
	auto bounding1 = CollisionBoxToBoundingAABB(box1);
	Mesh b1 = rectprism(bounding1.halfrad * 2, bounding1.position);

	CollisionBox box2(Vector3(-2.4, 0, 0), Vector3(1, 1, 1), Quaternion(1, 1, 0.9, 0.8).normalized());
	cout << box2.halfrad.to_string() << endl;
	auto bounding2 = CollisionBoxToBoundingAABB(box2);
	Mesh b2 = rectprism(bounding2.halfrad * 2, bounding2.position);

	bool DRAW = true;

	if (DRAW) {

	cout << "BV " << bounding2.overlaps(&bounding1) << endl;
	CollisionData overlap = checkCollision(box1, box2);
	cout << overlap.to_string() << "\n";

	BaseMaterial mat(BASEMAT_RED);
	if (overlap.exists()) mat = BaseMaterial(BASEMAT_GREEN);
	Mesh m = box1.getMesh();
	s.addMesh(&m, &mat);
	Mesh m2 = box2.getMesh();
	s.addMesh(&m2, &mat);



	std::cout << "Prepared\n";
	s.render();
	s.drawMesh(b1);
	s.drawMesh(b2);
	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	}

}

// 9.932C
void CollisionBoxTest2() {
	Scene s = scene_blank(true);
	s.camera.Trans(Transform(Vector3(0, 0, 5)));

	PointLight p(0);
	p.Trans(Vector3(0, 0, 10));
	s.lights.push_back(p);

	Vector3 box1_pos(0, 0, 0);
	// box1_pos = Vector3(0.3, -1.6, 0);
	CollisionBox box1(box1_pos, Vector3(1, 1.2, 1.4), Quaternion(1, 0.2, 0.4, 0.6).normalized());
	auto bounding1 = CollisionBoxToBoundingAABB(box1);
	Mesh b1 = rectprism(bounding1.halfrad * 2, bounding1.position);

	// change the y position of box2
	CollisionBox box2(Vector3(-2.4, 1.2, 0), Vector3(0.67, 0.42, 0.69), Quaternion(1, 1, 0.9, 0.8).normalized());
	cout << box2.halfrad.to_string() << endl;
	auto bounding2 = CollisionBoxToBoundingAABB(box2);
	Mesh b2 = rectprism(bounding2.halfrad * 2, bounding2.position);

	bool DRAW = true;

	if (DRAW) {

	cout << "BV " << bounding2.overlaps(&bounding1) << endl;
	CollisionData overlap = checkCollision(box1, box2);
	cout << overlap.to_string() << "\n";

	BaseMaterial mat(BASEMAT_RED);
	if (overlap.exists()) mat = BaseMaterial(BASEMAT_GREEN);
	Mesh m = box1.getMesh();
	s.addMesh(&m, &mat);
	Mesh m2 = box2.getMesh();
	s.addMesh(&m2, &mat);



	std::cout << "Prepared\n";
	s.render();
	s.drawMesh(b1);
	s.drawMesh(b2);
	std::cout << "Drawn " << s.countTriangles() << " Triangles\n";

	s.outputBuffer(BUFFER_PATH);

	std::cout << "Stored\n";

	}

}

#endif