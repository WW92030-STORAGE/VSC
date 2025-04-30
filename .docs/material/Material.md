# material/Material/BaseMaterial

`class BaseMaterial`

## Description

Base material class. Supports parameters such as color, specular, and reflectivity.

## Notes

- ImageTexture not supported on the raytracer.

## Fields

- `baseColor: Vector3` = Base color
- `specular: float` = Specular exponent
- `reflective: float` = Reflectivity (from 0 to 1)
- `reflective: bool` = Does the material refract light?
- `refraction: float` = Index of refraction

## Constructors

- `BaseMaterial()` = Color (0, 0, 0) and specular 0
- `BaseMaterial(Vector3 v, float spec, float refl, float refr)`
- `BaseMaterial(uint32_t v, float spec, float refl, float refr)` = converts from RGB hex to Vector3 color

## Functions

- `uint32_t getColor(Vector2 v)` = gets the color at uv coordinates `v`
- `uint32_t getColor(int x, int y)` = gets the color at pixel `(x, y)`
- `bool operator==(BaseMaterial& other)` = returns if the two materials have identical stats.
- `std::string to_string()` = returns a string representation

# Static Instances

- `BASEMAT_WHITE(Vector3(1, 1, 1), 0, 0, 0)`
- `BASEMAT_RED(Vector3(1, 0, 0), 0);`
- `BASEMAT_YELLOW(Vector3(1, 1, 0), 0);`
- `BASEMAT_GREEN(Vector3(0, 1, 0), 0);`
- `BASEMAT_CYAN(Vector3(0, 1, 1), 0);`
- `BASEMAT_BLUE(Vector3(0, 0, 1), 0);`
- `BASEMAT_MAGENTA(Vector3(1, 0, 1), 0);`
- `BASEMAT_BLACK(Vector3(0, 0, 0), 0);`