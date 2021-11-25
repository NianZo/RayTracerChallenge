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

// Must have full constructor definition since infinity has an incomplete type
Cylinder::Cylinder()
{
	transform = IdentityMatrix();
	material = Material();
	minimum = -std::numeric_limits<float>::infinity();
	maximum = std::numeric_limits<float>::infinity();
	closed = false;
}

Tuple Cylinder::objectNormal(const Tuple& p) const
{
	const float dist = p.x * p.x + p.z * p.z;
	Tuple normal;

	if (dist < 1.0f && p.y >= maximum - TUPLE_EPSILON)
	{
		normal = Vector(0, 1, 0);
	} else if (dist < 1.0f && p.y <= minimum + TUPLE_EPSILON)
	{
		normal = Vector(0, -1, 0);
	} else
	{
		normal = Vector(p.x, 0, p.z);
	}

	return normal;
}

std::vector<Intersection> Cylinder::objectIntersect(const Ray& r) const
{
	std::vector<Intersection> i;

	// Calculate discriminant
	const float a = r.direction.x * r.direction.x + r.direction.z * r.direction.z;
	const float b = 2 * r.origin.x * r.direction.x + 2 * r.origin.z * r.direction.z;
	const float c = r.origin.x * r.origin.x + r.origin.z * r.origin.z - 1;
	const float discriminant = b * b - 4 * a * c;

	if (std::abs(a) > TUPLE_EPSILON && discriminant >= 0)
	{
		const float t0 = (-b - sqrtf(discriminant)) / (2 * a);
		const float t1 = (-b + sqrtf(discriminant)) / (2 * a);

		const float y0 = r.origin.y + t0 * r.direction.y;
		if (y0 > minimum && y0 < maximum)
		{
			i.emplace_back(Intersection(t0, this));
		}

		const float y1 = r.origin.y + t1 * r.direction.y;
		if (y1 > minimum && y1 < maximum)
		{
			i.emplace_back(Intersection(t1, this));
		}
	}

	if (closed)
	{
		const float tMin = (minimum - r.origin.y) / r.direction.y;
		if (std::pow(r.origin.x + tMin * r.direction.x, 2.0f) + std::pow(r.origin.z + tMin * r.direction.z, 2.0f) <= 1.0f)
		{
			i.emplace_back(Intersection(tMin, this));
		}

		const float tMax = (maximum - r.origin.y) / r.direction.y;
		if (std::pow(r.origin.x + tMax * r.direction.x, 2.0f) + std::pow(r.origin.z + tMax * r.direction.z, 2.0f) <= 1.0f)
		{
			i.emplace_back(Intersection(tMax, this));
		}
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
