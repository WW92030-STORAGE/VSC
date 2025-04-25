# ACT I - THE BASICS  
  
# 1 - NOTATION/SPECS  
  
- @ is used to denote dot products and matrix multiplication IN COMMENTS AND THIS README.  
- |R> ("ket R") is the vector where all entries are 0 except for the Rth. R is zero indexed, and the vector dimension is context driven.  
  
- When solving a system (such as a linear system, or checking geometry intersections), exactly one solution will be given if there are solutions. If multiple solutions exist then one is given. If no solutions exist then a special NIL state containing NANs is given.

# 2 - VECTORS

Vectors.h - Implementations of basic methods for Vector2 and Vector3 and Vector4.  
- Vector definitions, constructors, copying  
- Vector operations: elementwise, scalars, dot and cross products  
- Normalization and projection of vectors. Rotations for 2d vectors  
  
# 3 - MATRICES  
  
Matrices.h - 2x2 and 3x3 matrices  
- Addition, Mulitpliaction (vectors and mats), Determinants  
- Inverses, Eigenvalues, Diagonalization, Systems (Eigens and Diag for 2x2 only)  
- The individual vectors in a matrix here are COLUMNS. So the x axis for instance has one element in each row.  
  
The main takeaways are:  
- Matrix multiplication is a sequence of dot products.  
- Determinants for small matrices can be computed directly using cofactors.  
- The matrix of eigenvector columns, combined with the diagonal matrix of corresponding eigenvalues, diagonalizes a matrix.  
  
# 4A - TRANSFORMATIONS   
  
NOTES TO BEGIN  
- A vector is basically a linear combination of its basis components. So if you have a vector (x, y, z) normally its basis is the standard "computational" basis |0>, |1>, |2> in which it becomes:  
- (x, y, z) = x(1, 0, 0) + y(0, 1, 0) + z(0, 0, 1) + x|0> + y|1> + z|2>.  
- But if you switch the basis vectors to say independent basis X, Y, Z then you instead get the vector xX + yY + zZ if you instead "read" the (x, y, z) in that basis.  
- This effect of switching from the computational basis into the {X, Y, Z} basis is done by simply multiply matrix B = [X, Y, Z] on the left of the v = (x, y, z) vector. The result Bv is the vector xX + yY + zZ as components read in the computational basis.  
  
Orthonormal matries, those whose transposes are their inverses and whose columns are orthogonal unit vectors, are transforms that preserve distanecs and angles.   

Matrices.h + Vectors.h:  
- Rotations for 2d and 3d matrices  
- 4d matrices, generalizations of Gaussian Elimination (det, inv, solve)  
  
Transform.h  
- The infamous Transform structure, which encompasses location and rotation in one box.  
- Vectors now are 4 dimensional: If the last component (w) is 0 then the vector is purely directional and is not affected by the affine translation (last matrix column). Otherwise if the w component is 1 then the vector is locational and is affected by translations.   
- Transformations into matrices and back, and a normal vector transform system.  
  
# 4B - QUATERNIONS  
  
Quaternion.h  
- Quaternions, a transform-like system to process rotations. Quaternions (rotations in 3d) are an extension of complex numbers (rotations in 2d) with even more weirder component interaction rules.  
- Quaternion multiplication - the bizarre method of compositing two rotations.  
- Quaternion interpolation - more than just a simple LERP.   

# 5A - POINTS, LINES, PLANES

Primitives.h  
- Lines are described by point and slope (direction)  
- Planes are described by point and normal vector, or a standard form using normal and distance to origin.  
- Intersection is done by solving systems of equations based on the parametric forms  
- To transform a plane is to transform the normal vector and the point. But we can actually do it with one transform if using the standard form vector.  

# 5B - VIEWS AND PERSPECTIVE  
  
Camera.h  
- A frustum class to denote the pyramid and frustum of vision. The projection plane is always defined so that the side planes intersect it 1 unit distance laterally from its center. The base of the frustum is a square.  
- A more general Camera class to denote a camera.  
- Rasterization is done by interpolating rays linearly across the projection plane onto surfaces.  
- For now we don't modify the near plane of the clipping system.  
- Be aware that the z direction is reversed after projection, so far away objects have a higher z position in clip space but a lower z position in camera space.  
   
- ... You're better off just reading through the Camera.h file and its comments. The story is a bit convoluted. (The same applies to most things from now on.)  
  
# ACT II - DESCENT INTO HELL  
  
This part will delve into some stuff that might be beyond the scope of the Skeleton Book. We begin to manifest the fundamental building blocks of rendering to the codebase--triangles, meshes, and the like. We also setup the renderer so we can see the outputs.  

The code here is instead using https://github.com/OneLoneCoder/Javidx9/tree/master/ConsoleGameEngine/BiggerProjects/Engine3D as a basis. This existing codebase we will refer to as Engine3D. If you would like he also has tutorials on YouTube starting with this one: https://www.youtube.com/watch?v=ih20l3pJoeU. Be aware it goes into some of the content from Act I but also Act III.  
   
Object.h  
- Objects are basically the entities in a scene. Primitives, triangles, models, and cameras are objects and inherit from the Object class.  
- The only use of this centralization is so you can have arrays of objects.   
  
Triangle.h  
- Triangles form the basis of most if not all 3d modelling systems. A triangle is defined by three non-collinear points and represents a portion of a plane.  
  
Mesh.h  
- Meshes compose of vertices that form triangle faces.   
- Entities are loaded from .obj files. In the future we will explore the nature of texturing and materials. 
  
Scene.h  
- The most basic form of a scene contains a camera and some objects. The camera is positioned at the origin and facing in the negative Z direction, so that camera space is lined up with Euclidean space. To move the camera, instead we can simply inverse transform the scene.  
- The output is drawn onto the buffer, which represents the rasterization of pixels. In Act III we will explore more efficient rasterization methods.  
  
DirectionalLight.h  
- A simple directional light system to provide clarity to mesh faces. The color of the pixel is hardcoded with lighting accounted for, this will not be the case in the future.  
  
# INTERMISSION - FRAGMENTATION  
  
Scene.h
- Fragment class which represents fragments. For now when the buffer is outputted the color is retrieved. The original version of Scene.h is located in the archive.  
- Fragments contain the clip space coordinates but also the original z coordinate of a point (here represented as the w coordinate after the frustum matrix), surface normal, and the color.  
  
Triangle.h  
- Added a new TriangleF class which now stores fragments. It is a generalization of Triangle3. When a Triangle3 is attempted to be drawn it is now projected into TriangleF structures which represent location in NDC  
  
# 6. RAY TRACING
  
The ray tracer portion of this project wil
  
Primitives.h  
- Method to calculate when a Plane intersects a ray (line) with origin and unit vector direction (NaN for null intersection).  
  
Triangle.h  
- Method to calculate when a Triangle3 intersects a ray with origin and unit vector direction (NaN for null intersection).  
  
Shapes.h  
- Added some hardcoded meshes for basic shapes.  
  
# 7 - LIGHT AND SHADOW
  
PointLight.h  
- Point light class with an attenuation. Also modified the base Light class to enclose the intensity as a Vector3 (three channels).  
  
Scene.h  
- Added an illuminate method which does raycasting to compute the illumination of a point given the primary ray, position, base color, etc.  
  
Material.h  
- Collating of material parameters such as base color and specular.  
  
# ACT III - RECONVENE  
  
ImageTexture.h  
- Image texture format to store images as 2-dimensional color matrices.  
  
Mesh.h  
- Meshes now store texture data in uvs.
- objs can now be imported with the texture coordinates  
  
Scene.h  
- Illumination and color picking methods changed to detect image textures and apply the texture mapping.  
  
Ray.h  
- Rays and Intersection Points  
  
RayTracer.h  
- A subclass of Scene.h that supports the rasterization of Scene but now also the rendering of ray-traced scenes. (You can do both RTX and rasterization, the buffer is the same.)  
  
# 8 - VISIBILITY  
  
Scene.h  
- Backface culling -- the act of not drawing triangles that face away from the camera.  
  
aabb.h  
- Axis-Aligned Bounding Boxes and ray intersections.  
  
BVH.h  
- Bounding Volume Hierarch system with recursive ray intersections with AABBs and Triangle3s.  
  
RayTracer.h  
- Configuration ability to toggle BVH usage when rendering. Results are significantly faster with a BVH.  
  
# 11 - CURVES  
  
BaseCurve.h  
- Base class for curves.  
Bezier.h  
- Bezier curves and splines.  
