# lighting/Light

`class Light : public Object`

## Description

Basic light object.

## Notes

Directional lights not supported yet.

## Fields

- `intensity: Vector3` = the emitted color
- `attenuation: float` = Distance linear attenuation
- `TYPE: enum LightType {NIL, POINT, DIRECTIONAL}` = Light type

## Constructors

- `Light()` = Blank constructor
- `Light(Vector3 i)` = sets the color `intensity = i`
