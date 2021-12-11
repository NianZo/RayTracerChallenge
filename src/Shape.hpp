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

    Shape() noexcept : transform(IdentityMatrix()) {};
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
    // I'd prefer to not define these and have a compile time error if these are used, but *shrug*
    [[nodiscard]] virtual Tuple objectNormal([[maybe_unused]] const Tuple& p) const noexcept
    {
        return Tuple(0.0F, 0.0F, 0.0F, 0.0F);
    }
    [[nodiscard]] virtual std::vector<Intersection> objectIntersect([[maybe_unused]] const Ray& r) const noexcept
    {
        std::vector<Intersection> i;
        return i;
    }
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

struct IntersectionDetails
{
    const Shape& object;
    const Tuple point;
    const Tuple overPoint;
    const Tuple underPoint;
    const Tuple eyeVector;
    const Tuple normalVector;
    const Tuple reflectionVector;
    const float t;
    const float reflectance;
    const bool inside;
    const float n1;
    const float n2;
};

Sphere GlassSphere() noexcept;

#endif /* SRC_SHAPE_HPP_ */
