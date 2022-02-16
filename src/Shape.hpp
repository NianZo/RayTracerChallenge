/*
 * Sphere.hpp
 *
 *  Created on: Apr 25, 2021
 *      Author: nic
 */

#ifndef SRC_SHAPE_HPP_
#define SRC_SHAPE_HPP_

#include "Material.hpp"
#include "Matrix.hpp"

#include <numbers>
#include <vector>

class Ray;
class Shape;

class Intersection
{
  public:
    // The actual fields must not be const or we can't sort a vector of hits based on intersection parameter 't'
    // However, object is fine because the pointer isn't const, the object it points to is const.
    float t;
    const Shape* object;
    float u;
    float v;

    Intersection(float tIn, const Shape* objectIn) noexcept : t(tIn), object(objectIn), u(0.0F), v(0.0F){};
    Intersection(float tIn, const Shape* objectIn, float uIn, float vIn) noexcept : t(tIn), object(objectIn), u(uIn), v(vIn){};
    bool operator==(const Intersection& other) const noexcept { return t == other.t && object == other.object; }
    bool operator<(const Intersection& other) const noexcept { return t < other.t; }
};

class Shape
{
  public:
    Matrix<4> transform;
    Material material;
    Shape* parent = nullptr;

    Shape() noexcept : transform(IdentityMatrix()){};
    virtual ~Shape() noexcept = default;
    Shape(const Shape&) noexcept = default;
    Shape(Shape&&) noexcept = default;
    Shape& operator=(const Shape&) noexcept = default;
    Shape& operator=(Shape&&) noexcept = default;

    bool operator==(const Shape& other) const noexcept { return transform == other.transform && material == other.material && parent == other.parent; }

    [[nodiscard]] Tuple normal(const Tuple& p, const Intersection& i = Intersection(0.0F, nullptr)) const noexcept;
    [[nodiscard]] std::vector<Intersection> intersect(const Ray& r) const noexcept;
    [[nodiscard]] Color shade(const Light& light, const Tuple& position, const Tuple& eyeVector, bool inShadow) const noexcept;

  private:
    [[nodiscard]] virtual Tuple objectNormal([[maybe_unused]] const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept = 0;
    [[nodiscard]] virtual std::vector<Intersection> objectIntersect([[maybe_unused]] const Ray& r) const noexcept = 0;
    [[nodiscard]] Matrix<4> getFullTransform() const noexcept;
};

class Sphere : public Shape
{
  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Plane : public Shape
{
  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Cube : public Shape
{
  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Cylinder : public Shape
{
  public:
    float minimum;
    float maximum;
    bool closed{false};

    Cylinder() noexcept;

  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Cone : public Shape
{
  public:
    float minimum;
    float maximum;
    bool closed{false};

    Cone() noexcept;

  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Triangle : public Shape
{
  public:
    std::array<Tuple, 3> vertices;

    Triangle(const Tuple& v1, const Tuple& v2, const Tuple& v3) noexcept;

  private:
    std::array<Tuple, 2> edges;
    Tuple normalVector;

    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class SmoothTriangle : public Shape
{
  public:
    std::array<Tuple, 3> vertices;
    std::array<Tuple, 3> normals;

    SmoothTriangle(const Tuple& v1, const Tuple& v2, const Tuple& v3, const Tuple& n1, const Tuple& n2, const Tuple& n3) noexcept;

  private:
    std::array<Tuple, 2> edges;

    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Group : public Shape
{
  public:
    Group() = default;
    Group(const Group& other) noexcept : Shape(other),
                                         groups(other.groups),
                                         spheres(other.spheres),
                                         planes(other.planes),
                                         cubes(other.cubes),
                                         cylinders(other.cylinders),
                                         cones(other.cones),
                                         triangles(other.triangles),
                                         smoothTriangles(other.smoothTriangles)
    {
        for (auto& group : groups)
        {
            group.parent = this;
        }
        for (auto& sphere : spheres)
        {
            sphere.parent = this;
        }
        for (auto& plane : planes)
        {
            plane.parent = this;
        }
        for (auto& cube : cubes)
        {
            cube.parent = this;
        }
        for (auto& cylinder : cylinders)
        {
            cylinder.parent = this;
        }
        for (auto& cone : cones)
        {
            cone.parent = this;
        }
        for (auto& triangle : triangles)
        {
            triangle.parent = this;
        }
        for (auto& smoothTriangle : smoothTriangles)
        {
            smoothTriangle.parent = this;
        }
    };
    Group(Group&&) noexcept = default;
    Group& operator=(const Group& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        transform = other.transform;
        material = other.material;
        parent = other.parent;
        groups = other.groups;
        spheres = other.spheres;
        planes = other.planes;
        cubes = other.cubes;
        cylinders = other.cylinders;
        cones = other.cones;
        triangles = other.triangles;
        smoothTriangles = other.smoothTriangles;

        for (auto& group : groups)
        {
            group.parent = this;
        }
        for (auto& sphere : spheres)
        {
            sphere.parent = this;
        }
        for (auto& plane : planes)
        {
            plane.parent = this;
        }
        for (auto& cube : cubes)
        {
            cube.parent = this;
        }
        for (auto& cylinder : cylinders)
        {
            cylinder.parent = this;
        }
        for (auto& cone : cones)
        {
            cone.parent = this;
        }
        for (auto& triangle : triangles)
        {
            triangle.parent = this;
        }
        for (auto& smoothTriangle : smoothTriangles)
        {
            smoothTriangle.parent = this;
        }
        return *this;
    };
    Group& operator=(Group&&) noexcept = default;
    ~Group() noexcept override = default;
    [[nodiscard]] std::vector<std::reference_wrapper<const Shape>> objects() const noexcept;
    // TODO(nic) can I make this a template? Each pushes elements to a different vector
    // TODO(nic) it is dangerous for these to return a reference to the object added...
    Group& addChild(const Group& c) noexcept;
    Sphere& addChild(const Sphere& c) noexcept;
    Plane& addChild(const Plane& c) noexcept;
    Cube& addChild(const Cube& c) noexcept;
    Cylinder& addChild(const Cylinder& c) noexcept;
    Cone& addChild(const Cone& c) noexcept;
    Triangle& addChild(const Triangle& t) noexcept;
    SmoothTriangle& addChild(const SmoothTriangle& st) noexcept;

  private:
    std::vector<Group> groups;
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<Cube> cubes;
    std::vector<Cylinder> cylinders;
    std::vector<Cone> cones;
    std::vector<Triangle> triangles;
    std::vector<SmoothTriangle> smoothTriangles;

    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

struct __attribute__((aligned(128))) IntersectionDetails
{
    const Tuple point;
    const Tuple overPoint;
    const Tuple underPoint;
    const Tuple eyeVector;
    const Tuple normalVector;
    const Tuple reflectionVector;
    const Shape& object;
    const float t;
    const float reflectance;
    const float n1;
    const float n2;
    const bool inside;
};

Sphere GlassSphere() noexcept;

#endif /* SRC_SHAPE_HPP_ */
