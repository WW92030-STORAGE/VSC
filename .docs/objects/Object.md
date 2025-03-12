# objects/Mesh

`class Object`

## Description

Base class of scene objects. Objects can be transformed.

## Fields

- `transform: Transform` = World transform of the object.

## Constructors

- `Object()`

## Functions

- `void Trans(Transform t)` = Transforms the object akin to doing `transform = t * transform`. This will be overridden in subclasses as needed.
- `void ForceTrans(Transform t)` = Transforms the object data WITHOUT actually modifying the `transform`. This is useful if you want to move things like vertices without moving the object's coordinates.
- `std::string to_string()` = Gives a string representation.