# scene/Shader

## Description

Shader system

## Notes

A fragment shader basically says "For this fragment (containing info on lighted color, albedo, world space coords, uv, screenUV, etc.) what color do we actually set it to?"

## Functions

All functions are in a special namespace `FragShader`

- `Fragment identity(Fragment f)` = identity function. A `nullptr` `FRAG_SHADER` pointer equivalents to the identity.
- `Fragment invert(Fragment f)` = invert colors

## Fields

- `typedef float (*INTERP)(float)` = FRAG_SHADER function type. It modifies fragments.