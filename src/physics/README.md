# VSC: SUBSUMING VORTEX

This is it guys. It's the final 1% (9.9 - 10) of VSC. This will attempt to create a rudimentary physics engine that integrates nicely with the current renderer.

This is a part of VSC however it really should be treated like a submodule. Its scope is possibly as large as the other 99% (0 - 9.9) combined.
Everything in this folder is standalone (with only `utils` as a dependency). Its files will use the special suffix `VSC_PHYS` in the macros.

Based on a certain book on Game Physics by Ian Millington, and the accompanying source code `https://github.com/idmillington/cyclone-physics`

We will not do exactly the same thing however, and many things will be ... rearranged.

Unlike most other things, there is very little concept of ownership within individual modules. Mostly because this is something that is integrated into base VSC.
As a result, pointer usage and pointer arrays are increased here.

# NOTES

# 3 - 6. PARTICLE MOTION

- Particle class, Particle Force Generators, Particle Groups
- Force of gravity (built in), Velocity Verlet (scattering of methods), Spring Forces

# 7 - COLLISIONS AND CONSTRAINTS

- Collision information packet (ParticleCollision), which encodes information about certain constraints on particles.

# 8 - MASS AGGREGATE SYSTEM

- ParticleEngine, ParticleLink. 

# 9. MORE MATH

- This stuff covers most of the math in `utils` most importantly stuff like `Matrices` and `Quaternion`

# 10 - RIGID BODIES

- RigidBody class