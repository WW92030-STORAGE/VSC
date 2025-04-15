# material/ImageTexture

`class ImageTexture : public BaseMaterial`

## Description

Image texture material.

## Notes

- ImageTexture not supported on the raytracer for morphed meshes (Normal meshes OK).
- Python converters included.

## Fields

- `image: std::vector<std::vector<uint32_t>>` = stores the pixel data of the image. Look in `/python` to find programs to convert images to data.
- `W, H: int` = dimensions

## Constructors

- `ImageTexture(int w, int h)`
- `ImageTexture(std::vector<std::vector<uint32_t>> tex)`

## Functions

- `std::pair<int, int> getTexel(Vector2 v)` = converts a uv coordinate ([0, 0] ... [1, 1]) to a pixel coordinate using nearest neighbor.
- `void setTexel(Vector2 v, uint32_t c)` = Sets the color of a pixel given the uv coordinates.
- `void setTexel(int x, int y, uint32_t c)` = Sets the color of a pixel given the pixel coordinates.

- getColor now returns the relevant pixel color instead of the base color.