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
class Intersection;

class Shape
{
  public:
    Matrix<4> transform;
    Material material;

    Shape() noexcept : transform(IdentityMatrix()){};
    virtual ~Shape() noexcept = default;
    Shape(const Shape&) noexcept = default;
    Shape(Shape&&) noexcept = default;
    Shape& operator=(const Shape&) noexcept = default;
    Shape& operator=(Shape&&) noexcept = default;

    bool operator==(const Shape& other) const noexcept { return transform == other.transform && material == other.material; }

    [[nodiscard]] Tuple normal(const Tuple& p) const noexcept;
    [[nodiscard]] std::vector<Intersection> intersect(const Ray& r) const noexcept;
    [[nodiscard]] Color shade(const Light& light, const Tuple& position, const Tuple& eyeVector, bool inShadow) const noexcept;

  private:
    [[nodiscard]] virtual Tuple objectNormal([[maybe_unused]] const Tuple& p) const noexcept = 0;
    [[nodiscard]] virtual std::vector<Intersection> objectIntersect([[maybe_unused]] const Ray& r) const noexcept = 0;
};

class Sphere : public Shape
{
  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Plane : public Shape
{
  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Cube : public Shape
{
  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Cylinder : public Shape
{
  public:
    float minimum;
    float maximum;
    bool closed;

    Cylinder() noexcept;

  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Cone : public Shape
{
  public:
    float minimum;
    float maximum;
    bool closed;

    Cone() noexcept;

  private:
    [[nodiscard]] Tuple objectNormal(const Tuple& p) const noexcept override;
    [[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Group : public Shape
{
public:
	std::vector<std::reference_wrapper<const Shape>> objects() const noexcept;
	void addChild(const Group& c) noexcept;
	void addChild(const Sphere& c) noexcept;
	void addChild(const Plane& c) noexcept;
	void addChild(const Cube& c) noexcept;
	void addChild(const Cylinder& c) noexcept;
	void addChild(const Cone& c) noexcept;

private:
	std::vector<Group> groups;
	std::vector<Sphere> spheres;
	std::vector<Plane> planes;
	std::vector<Cube> cubes;
	std::vector<Cylinder> cylinders;
	std::vector<Cone> cones;

	[[nodiscard]] Tuple objectNormal(const Tuple& p) const noexcept override;
	[[nodiscard]] std::vector<Intersection> objectIntersect(const Ray& r) const noexcept override;
};

class Intersection
{
  public:
    // The actual fields must not be const or we can't sort a vector of hits based on intersection parameter 't'
    // However, object is fine because the pointer isn't const, the object it points to is const.
    float t;
    const Shape* object;

    Intersection(float tIn, const Shape* objectIn) noexcept : t(tIn), object(objectIn){};
    bool operator==(const Intersection& other) const noexcept { return t == other.t && object == other.object; }
    bool operator<(const Intersection& other) const noexcept { return t < other.t; }
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
