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

#include <memory>
#include <numbers>
#include <string>
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
    [[nodiscard]] virtual std::vector<std::reference_wrapper<const Shape>> allSubObjects() const noexcept {return {std::ref(*this)};};
    [[nodiscard]] virtual std::unique_ptr<Shape> clone() const noexcept = 0;

  private:
    [[nodiscard]] virtual Tuple objectNormal([[maybe_unused]] const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept = 0;
    [[nodiscard]] virtual std::vector<Intersection> objectIntersect([[maybe_unused]] const Ray& r) const noexcept = 0;
    [[nodiscard]] Matrix<4> getFullTransform() const noexcept;
};

class Sphere : public Shape
{
public:
	[[nodiscard]] std::unique_ptr<Shape> clone() const noexcept override
	{
		return std::make_unique<Sphere>(*this);
	}
  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Plane : public Shape
{
public:
	[[nodiscard]] std::unique_ptr<Shape> clone() const noexcept override
	{
		return std::make_unique<Plane>(*this);
	}
  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Cube : public Shape
{
public:
	[[nodiscard]] std::unique_ptr<Shape> clone() const noexcept override
	{
		return std::make_unique<Cube>(*this);
	}
  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Cylinder : public Shape
{
public:
	[[nodiscard]] std::unique_ptr<Shape> clone() const noexcept override
	{
		return std::make_unique<Cylinder>(*this);
	}

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
	[[nodiscard]] std::unique_ptr<Shape> clone() const noexcept override
	{
		return std::make_unique<Cone>(*this);
	}

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
	[[nodiscard]] std::unique_ptr<Shape> clone() const noexcept override
	{
		return std::make_unique<Triangle>(*this);
	}

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
	[[nodiscard]] std::unique_ptr<Shape> clone() const noexcept override
	{
		return std::make_unique<SmoothTriangle>(*this);
	}

  private:
    std::array<Tuple, 2> edges;

    [[nodiscard]] Tuple objectNormal(const Tuple& p, [[maybe_unused]] const Intersection& i) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class CSG : public Shape
{
  public:
    int operation;
    std::unique_ptr<Shape> left;
    std::unique_ptr<Shape> right;

    CSG(int operationIn, std::unique_ptr<Shape> leftIn, std::unique_ptr<Shape> rightIn)
    noexcept : operation(operationIn), left(std::move(leftIn)), right(std::move(rightIn))
    {
        left->parent = this;
        right->parent = this;
    };
    CSG(const CSG& other) noexcept : Shape(other), operation(other.operation), left(other.left->clone()), right(other.right->clone()) {};
    ~CSG() noexcept override = default;
    CSG& operator=(const CSG& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        operation = other.operation;
        left = other.left->clone();
        right = other.right->clone();
        return *this;
    }

    static bool intersectionAllowed(int operation, bool lhit, bool inl, bool inr);
    [[nodiscard]] std::vector<Intersection> filterIntersections(const std::vector<Intersection>& intersections) const noexcept;
    [[nodiscard]] std::vector<std::reference_wrapper<const Shape>> allSubObjects() const noexcept override;
	[[nodiscard]] std::unique_ptr<Shape> clone() const noexcept override
	{
		std::unique_ptr<CSG> newShape = std::make_unique<CSG>(*this);
		newShape->left = left->clone();
		newShape->right = right->clone();
		return newShape;
	}

	enum Operation {Union, Intersect, Difference};

  private:
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
										 //csgs(other.csgs)
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
        for (auto& csg : csgs)
        {
        	csg.parent = this;
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
        //csgs = other.csgs;

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
    [[nodiscard]] std::vector<std::reference_wrapper<const Shape>> allSubObjects() const noexcept override;
	[[nodiscard]] std::unique_ptr<Shape> clone() const noexcept override
	{
		return std::make_unique<Group>(*this);
	}
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
    CSG& addChild(const CSG& csg) noexcept;

  private:
    std::vector<Group> groups;
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<Cube> cubes;
    std::vector<Cylinder> cylinders;
    std::vector<Cone> cones;
    std::vector<Triangle> triangles;
    std::vector<SmoothTriangle> smoothTriangles;
    std::vector<CSG> csgs;

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
