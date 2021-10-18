/*
 * Sphere.hpp
 *
 *  Created on: Apr 25, 2021
 *      Author: nic
 */

#ifndef SRC_SPHERE_HPP_
#define SRC_SPHERE_HPP_

#include "Material.hpp"
#include "Matrix.hpp"

#include <vector>

class Ray;
class Intersection;

class Shape
{
  public:
    Matrix<4> transform;
    Material material;

    Shape() : transform(IdentityMatrix()), material(Material()){};
    virtual ~Shape() = default;

    bool operator==(const Shape& other) const { return transform == other.transform && material == other.material; }

    Tuple normal(const Tuple& p) const;
    std::vector<Intersection> intersect(const Ray& r) const;

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
  public:
    //Sphere() {transform = IdentityMatrix();}
    //~Sphere() = default;

    //Tuple normal(const Tuple& p) const;

  private:
    Tuple objectNormal(const Tuple& p) const override;
    std::vector<Intersection> objectIntersect(const Ray& r) const override;
};

class Plane : public Shape
{
  public:
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
    const Tuple eyeVector;
    const Tuple normalVector;
    const float t;
    const bool inside;
};

#endif /* SRC_SPHERE_HPP_ */
