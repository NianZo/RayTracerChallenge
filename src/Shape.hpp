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

#include <vector>
#include <numbers>

class Ray;
class Intersection;

class Shape
{
  public:
    Matrix<4> transform;
    Material material;

    Shape() : transform(IdentityMatrix()), material(Material()){};
    virtual ~Shape() = default;
    Shape(const Shape&) = default;
    Shape(Shape&&) = default;
    Shape& operator=(const Shape&) = default;
    Shape& operator=(Shape&&) = default;

    bool operator==(const Shape& other) const { return transform == other.transform && material == other.material; }

    Tuple normal(const Tuple& p) const;
    std::vector<Intersection> intersect(const Ray& r) const;
    Color shade(const Light& light, const Tuple& position, const Tuple& eyeVector, const bool inShadow) const;

  private:
    // I'd prefer to not define these and have a compile time error if these are used, but *shrug*
    virtual Tuple objectNormal([[maybe_unused]] const Tuple& p) const
    {
        return Tuple(0.0f, 0.0f, 0.0f, 0.0f);
    }
    virtual std::vector<Intersection> objectIntersect([[maybe_unused]] const Ray& r) const
    {
        std::vector<Intersection> i;
        return i;
    }
};

class Sphere : public Shape
{
  private:
    Tuple objectNormal(const Tuple& p) const override;
    std::vector<Intersection> objectIntersect(const Ray& r) const override;
};

class Plane : public Shape
{
  private:
    Tuple objectNormal(const Tuple& p) const override;
    std::vector<Intersection> objectIntersect(const Ray& r) const override;
};

class Cube : public Shape
{
  private:
    Tuple objectNormal(const Tuple& p) const override;
    std::vector<Intersection> objectIntersect(const Ray& r) const override;
};

class Cylinder : public Shape
{
  public:
	float minimum;
	float maximum;
	bool closed;

	Cylinder();

  private:
	Tuple objectNormal(const Tuple& p) const override;
	std::vector<Intersection> objectIntersect(const Ray& r) const override;
};

class Cone : public Shape
{
public:
	float minimum;
	float maximum;
	bool closed;

	Cone();

private:
	Tuple objectNormal(const Tuple& p) const override;
	std::vector<Intersection> objectIntersect(const Ray& r) const override;
};

class Intersection
{
  public:
    // The actual fields must not be const or we can't sort a vector of hits based on intersection parameter 't'
    // However, object is fine because the pointer isn't const, the object it points to is const.
    float t;
    const Shape* object;

    Intersection(const float tIn, const Shape* objectIn) : t(tIn), object(objectIn){};
    bool operator==(const Intersection& other) const { return t == other.t && object == other.object; }
    bool operator<(const Intersection& other) const { return t < other.t; }
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

Sphere GlassSphere();

#endif /* SRC_SHAPE_HPP_ */
