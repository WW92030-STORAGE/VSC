# raytracer/BVH/TriangleData

`struct TriangleData`

## Description

Data containing information on a specific Triangle3.

## Notes

- Either this, or an AABB, is contained in a BVHNode.

## Fields

- `tri: int` = The index of the triangle in the `mesh`.
- `mesh: Mesh*` = The mesh which this TriangleData represents a triangle in. The represented triangle corresponds to  `mesh->triindices[tri]`
- `mat: BaseMaterial*` = The material of this mesh. Useful for logistics.
- `interpnorms: bool` = Do we interpolate norms across the triangle?

## Constructors

- `TriangleData()` = `{0, nullptr, nullptr, false}`
- `TriangleData(int t, Mesh* m, BaseMaterial* mm, bool i)`

## Functions

- `Triangle3 get()` = Retrieve the Triangle3 represented by this TriangleData using the `mesh` and `tri` values.
- `operator<(TriangleData& o)` = compare by centroid.
- `std::string to_string(bool nofluff = false)` = String representation.

---

# raytracer/BVH/BVHNode

`class BVHNode`

## Description

Node of a BVH tree. Can contain a TriangleData or an AABB, as well as up to two child nodes.

## Notes

- Either a TriangleData or an AABB is contained in a BVHNode.

## Fields

- `box: AABB` = The AABB represented, assuming this node represents an AABB.
- `obj: TriangleData` = The Triangle3 represented, assuming this node represents a Triangle3.
- `L, R: BVHNode*` = child nodes.
- `splitAxis: int` = The axis to split along (0 for x, 1 for y, 2 for z).
- `splitThreshold: float` = The threshold to split upon. (splitAxis and splitThreshold are unused.)
- `isObj: bool` = If this node represents an object (Triangle3/TriangleData) instead of an AABB.

## Constructors

- `BVHNode()` = Does nothing
- `BVHNode(int o, BaseMaterial* mat, Mesh* mesh, bool interp)` = Represents a `TriangleData{o, mesh, mat, interp}`.
- `BVHNode(AABB aabb)` = Represents an `AABB(aabb)`.

## Functions

- `int size()` = Gets the number of TriangleData contained in this node and its subtree.
- `IntersectionPoint simpleIntersection(Ray r)` = Intersection point of the ray with either the AABB or the Triangle3.
- `IntersectionPoint IntersectRay(Ray r)` = First intersection point of the ray with all Triangle3 objects in its subtree.

---

# raytracer/BVH

`class BVH`

## Description

Bounding Volume Hierarchy tree system. Actually just a wrapper class for a BVHNode representing the root of the tree.

## Notes

- Either a TriangleData or an AABB is contained in a BVHNode.

## Fields

- `root: BVHNode*` = The root node of the tree.

## Constructors

- `BVH()`
- `BVH(BVHNode* n)` = `root = n`

There is no copy constructor.

## Functions

- `void inorder(std::vector<BVHNode*>& v, BVHNode* x)` = Helper function to store the inorder traversal of `x` subtree in `v`. 
- `std::vector<BVHNode*> traverse()` = Get the inorder traversal of all nodes.
- `IntersectionPoint IntersectRay(Ray r)` = Gets the first intersection point of a ray with any stored Triangle3 in its nodes.

# Static Instances

- `void awaken(BVHNode* node, std::vector<TriangleData> v)` = Create a subtree rooted at `node` representing all the `TriangleData` i.e. `Triangle3` in `v`.
- `BVH* create(std::vector<Mesh*> meshes, std::vector<BaseMaterial*> materials, std::vector<bool> NormInterps)` = Create a BVH with `TriangleData` representing the `Triangle3` objects in the meshes of `meshes`, using the `materials` and `NormInterps` vectors for additional configuration. These vectors are readily available in a `RayTracer` scene instance.