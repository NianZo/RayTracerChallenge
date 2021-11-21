/*
 * World.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: nic
 */

#include "World.hpp"
#include "Transformation.hpp"
#include <algorithm>

// TODO this is the cause of the failed test, essentially returning references to local objects
World World::BaseWorld()
{
    Sphere s1;
    s1.material.color = Color(0.8f, 1.0f, 0.6f);
    s1.material.diffuse = 0.7f;
    s1.material.specular = 0.2f;

    Sphere s2;
    s2.transform = scaling(0.5, 0.5, 0.5);

    World w;
    w.spheres.push_back(s1);
    w.spheres.push_back(s2);
    w.light = Light(Point(-10, 10, -10), Color(1, 1, 1));

    return w;
}

std::vector<std::reference_wrapper<const Shape>> World::objects() const
{
    std::vector<std::reference_wrapper<const Shape>> objects;

    for (const Shape& sphere : spheres)
    {
        objects.emplace_back(std::ref(sphere));
    }
    for (const Shape& plane : planes)
    {
        objects.emplace_back(std::ref(plane));
    }

    return objects;
}

std::vector<Intersection> World::intersect(Ray r) const
{
    std::vector<Intersection> intersections;
    for (const auto object : objects())
    {
        auto objectIntersections = object.get().intersect(r);
        for (const auto& intersection : objectIntersections)
        {
            intersections.push_back(intersection);
        }
    }
    std::sort(intersections.begin(), intersections.end());
    return intersections;
}

Color World::shadeHit(const IntersectionDetails& id, int remainingCalls) const
{
    const bool shadowed = isShadowed(id.overPoint);
    const Color surface = id.object.material.light(light, id.point, id.eyeVector, id.normalVector, shadowed);
    const Color reflected = reflectedColor(id, remainingCalls);
    return surface + reflected;
}

Color World::reflectedColor(const IntersectionDetails& id, int remainingCalls) const
{
    // Early out if object is not reflective or max recursion depth reached
    if (id.object.material.reflectivity == 0.0f || remainingCalls < 1)
    {
        return Color::Black;
    }

    const Ray reflectionRay = Ray(id.overPoint, id.reflectionVector);
    const Color reflectedColor = colorAt(reflectionRay, remainingCalls - 1);

    return reflectedColor * id.object.material.reflectivity;
}

Color World::refractedColor(const IntersectionDetails& id, int remainingCalls) const
{
	// Early out if object is not reflective or max recursion depth reached
	if (id.object.material.transparency == 0 || remainingCalls < 1)
	{
		return Color::Black;
	}

	const float nRatio = id.n1 / id.n2;
	const float cosI = id.eyeVector.dot(id.normalVector);
	const float sin2T = nRatio * nRatio * (1 - cosI * cosI);
	if (sin2T >= 1.0f)
	{
		return Color::Black;
	}

	return Color::White;
}

Color World::colorAt(Ray r, int remainingCalls) const
{
    std::vector<Intersection> intersections;
    for (const auto& object : objects())
    {
        const std::vector<Intersection> i = object.get().intersect(r);
        for (const Intersection& event : i)
        {
            intersections.push_back(event);
        }
    }
    auto hit = Ray::hit(intersections);
    if (hit)
    {
        return shadeHit(r.precomputeDetails(*hit, intersections), remainingCalls);
    } else
    {
        return {0, 0, 0};
    }
}

bool World::isShadowed(const Tuple& point) const
{
    const Tuple shadowVector = (light.position - point).normalize();
    const float distanceToLight = (light.position - point).magnitude();
    const Ray shadowRay = Ray(point, shadowVector);
    const auto intersections = intersect(shadowRay);
    const auto hit = shadowRay.hit(intersections);

    const bool shadowed = hit && hit->t < distanceToLight;
    return shadowed;
}
