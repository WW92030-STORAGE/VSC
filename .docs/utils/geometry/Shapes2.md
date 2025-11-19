# utils/geometry/Shapes

## Description

Functions to generate more complex shape meshes and composite forms made of multiple meshes. All meshes start centered at the origin, you must `Trans(form)` them.

## Functions

- `std::vector<Mesh> MengerSponge(float R, int L, bool cubesix = false)` = Generates a Menger sponge of side length `R` and with `L` levels of subdivision. If `cubesix` is set then each cube face will have a different texture (i.e. a `cube6`) otherwise the textures are same on all faces. WARNING - Each level of subdivision multiplies the number of cubes by 20.
