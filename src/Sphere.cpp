/*
 * Sphere.cpp
 *
 *  Created on: Sep 17, 2021
 *      Author: nic
 */

#include "Sphere.hpp"
#include "Ray.hpp"

#include <cmath>

Tuple Shape::normal(const Tuple& p) const
{
    return objectNormal(transform.inverse() * p);
}

std::vector<Intersection> Shape::intersect(const Ray& r) const
{
    return objectIntersect(r.transform(transform.inverse()));
}

Color Shape::shade(const Light& light, const Tuple& position, const Tuple& eyeVector, const bool inShadow) const
{
    const Light objectLight = {transform.inverse() * light.position, light.intensity};
    const Tuple objectPosition = transform.inverse() * position;
    return material.light(objectLight, objectPosition, eyeVector, normal(position), inShadow);
}

// Implicitly assumes that p is on the sphere surface and is a valid point (w = 1)
Tuple Sphere::objectNormal(const Tuple& p) const
{
    //const Tuple objectSpaceP = transform.inverse() * p;
    const Tuple objectSpaceNormal = (p - Point(0, 0, 0));
    Tuple worldSpaceNormal = transform.inverse().transpose() * objectSpaceNormal; // This is technically a hack and messes with w; set it to 0 at the end;
    worldSpaceNormal.w = 0;
    return worldSpaceNormal.normalize();
}

std::vector<Intersection> Sphere::objectIntersect(const Ray& r) const
{
    //const Ray ray2 = r.transform(this->transform.inverse());
    const Tuple sphereToRay = r.origin - Point(0, 0, 0);

    const float a = r.direction.dot(r.direction);
    const float b = 2 * r.direction.dot(sphereToRay);
    const float c = sphereToRay.dot(sphereToRay) - 1;

    const float discriminant = b * b - 4 * a * c;

    std::vector<Intersection> intersections;
    if (discriminant >= 0)
    {
        intersections.push_back(Intersection((-b - sqrtf(discriminant)) / (2 * a), this));
        intersections.push_back(Intersection((-b + sqrtf(discriminant)) / (2 * a), this));
    }

    return intersections;
}

Tuple Plane::objectNormal([[maybe_unused]] const Tuple& p) const
{
    return Vector(0, 1, 0);
}

std::vector<Intersection> Plane::objectIntersect([[maybe_unused]] const Ray& r) const
{
    std::vector<Intersection> i;
    if (std::abs(r.direction.y) > TUPLE_EPSILON)
    {
        const float t = -r.origin.y / r.direction.y;
        i.emplace_back(Intersection(t, this));
    }
    return i;
}

Tuple Cube::objectNormal(const Tuple& p) const
{
    const float maxCoord = std::max(std::max(std::abs(p.x), std::abs(p.y)), std::abs(p.z));

    Tuple normal;
    if (maxCoord == std::abs(p.x))
    {
        normal = Vector(p.x, 0, 0);
    } else if (maxCoord == std::abs(p.y))
    {
        normal = Vector(0, p.y, 0);
    } else
    {
        normal = Vector(0, 0, p.z);
    }
    return normal;
}

std::vector<Intersection> Cube::objectIntersect(const Ray& r) const
{
    float xTMin = (-1.0f - r.origin.x) / r.direction.x;
    float xTMax = (1.0f - r.origin.x) / r.direction.x;
    if (xTMin > xTMax)
    {
        std::swap(xTMin, xTMax);
    }

    float yTMin = (-1.0f - r.origin.y) / r.direction.y;
    float yTMax = (1.0f - r.origin.y) / r.direction.y;
    if (yTMin > yTMax)
    {
        std::swap(yTMin, yTMax);
    }

    float zTMin = (-1.0f - r.origin.z) / r.direction.z;
    float zTMax = (1.0f - r.origin.z) / r.direction.z;
    if (zTMin > zTMax)
    {
        std::swap(zTMin, zTMax);
    }

    const float tMin = std::max(std::max(xTMin, yTMin), zTMin);
    const float tMax = std::min(std::min(xTMax, yTMax), zTMax);

    std::vector<Intersection> i;
    if (tMax > tMin)
    {
        i.emplace_back(Intersection(tMin, this));
        i.emplace_back(Intersection(tMax, this));
    }
    return i;
}

Sphere GlassSphere()
{
    Sphere s;
    s.material.transparency = 1.0f;
    s.material.refractiveIndex = 1.5f;
    return s;
}
