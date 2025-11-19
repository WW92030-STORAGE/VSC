# utils/geometry/Fragment/Fragment

`class Fragment`

## Description

Fragment class that contains color, depth, etc. information about pixels. (Useful for shaders).

## Notes

- This is not what is stored in a scene buffer.

## Fields

- `NDC: Vector4` = Normalized Device Coordinates (`w` encodes information about original `z` camera space coordinate).
- `normal: Vector3` = Normal vector of the point the fragment extracts (world space).
- `uv: Vector2` = uv of the point the fragment extracts from.
- `color: uint32_t` = Illuminated color of the fragment (might be base color? I don't really know).
- `albedo: uint32_t` = Base color of the fragment (i.e. before shading).
- `screenUV: Vector2` = Screen UV of the point. Be aware that this coordinate system has origin bottom left (so flipped from what you normally expect to be UV).
- `wspos: Vector3` = World space position of the fragment.

## Constructors

- `Fragment()`
- `Fragment(Vector4 V, Vector3 N, uint32_t C)`
- `Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U) `
- `Fragment(Vector4 V, Vector3 N, uint32_t C, Vector2 U, uint32_t A)`

More constructors for things with more data. However you shouldn't need to use them normally. If you do end up needing to use them you can manually set fields.

## Functions

- `bool operator==(Fragment& o)`
- `bool operator!=(Fragment& o)`
- `std::string to_string()`

---

# utils/geometry/Fragment/ReducedFrag

`class ReducedFrag`

## Description

Reduced Fragment: perfect for a buffer (depth and color only).

## Notes

- This is what is stored in a scene buffer.

## Fields

- `depth: float` = Depth in camera space
- `color: uint32_t` = Rendered Color

## Constructors

- `ReducedFrag()`
- `ReducedFrag(float F, uint32_t C)`

## Functions

- `bool operator==(ReducedFrag& o)`
- `bool operator!=(ReducedFrag& o)`
- `std::string to_string()`
