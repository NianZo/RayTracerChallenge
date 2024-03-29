/*
 * World.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: nic
 */

#include "World.hpp"
#include "Transformation.hpp"
#include <algorithm>
#include <cmath>

World World::BaseWorld() noexcept
{
    Sphere s1;
    s1.material.color = Color(0.8F, 1.0F, 0.6F);
    s1.material.diffuse = 0.7F;
    s1.material.specular = 0.2F;

    Sphere s2;
    s2.transform = scaling(0.5F, 0.5F, 0.5F);

    World w;
    w.spheres.push_back(s1);
    w.spheres.push_back(s2);
    w.light = Light(Point(-10, 10, -10), Color(1, 1, 1));

    return w;
}

std::vector<std::reference_wrapper<const Shape>> World::objects() const noexcept
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
    for (const Cube& cube : cubes)
    {
        objects.emplace_back(std::ref(cube));
    }
    for (const Cylinder& cylinder : cylinders)
    {
        objects.emplace_back(std::ref(cylinder));
    }
    for (const Cone& cone : cones)
    {
        objects.emplace_back(std::ref(cone));
    }
    for (const Group& group : groups)
    {
        objects.emplace_back(std::ref(group));
    }

    return objects;
}

std::vector<Intersection> World::intersect(Ray r) const noexcept
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

Color World::shadeHit(const IntersectionDetails& id, int remainingCalls) const noexcept
{
    const bool shadowed = isShadowed(id.overPoint);
    const Color surface = id.object.material.light(light, id.point, id.eyeVector, id.normalVector, shadowed);
    const Color reflected = reflectedColor(id, remainingCalls);
    const Color refracted = refractedColor(id, remainingCalls);

    Color finalColor;
    if (id.object.material.reflectivity > 0.0F && id.object.material.transparency > 0.0F)
    {
        finalColor = surface + reflected * id.reflectance + refracted * (1 - id.reflectance);
    } else
    {
        finalColor = surface + reflected + refracted;
    }
    return finalColor;
}

Color World::reflectedColor(const IntersectionDetails& id, int remainingCalls) const noexcept
{
    // Early out if object is not reflective or max recursion depth reached
    if (id.object.material.reflectivity == 0.0F || remainingCalls < 1)
    {
        return Color::Black;
    }

    const Ray reflectionRay = Ray(id.overPoint, id.reflectionVector);
    const Color reflectedColor = colorAt(reflectionRay, remainingCalls - 1);

    return reflectedColor * id.object.material.reflectivity;
}

Color World::refractedColor(const IntersectionDetails& id, int remainingCalls) const noexcept
{
    // Early out if object is not reflective or max recursion depth reached
    if (id.object.material.transparency == 0 || remainingCalls < 1)
    {
        return Color::Black;
    }

    const float nRatio = id.n1 / id.n2;
    const float cosI = id.eyeVector.dot(id.normalVector);
    const float sin2T = nRatio * nRatio * (1 - cosI * cosI);
    if (sin2T >= 1.0F)
    {
        return Color::Black;
    }

    const float cosT = sqrtf(1.0F - sin2T);
    const Tuple refractionDirection = id.normalVector * (nRatio * cosI - cosT) - id.eyeVector * nRatio;
    const Ray refractionRay = Ray(id.underPoint, refractionDirection);
    return colorAt(refractionRay, remainingCalls - 1) * id.object.material.transparency;
}

Color World::colorAt(Ray r, int remainingCalls) const noexcept
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
    return hit ? shadeHit(r.precomputeDetails(*hit, intersections), remainingCalls) : Color(0, 0, 0);
}

bool World::isShadowed(const Tuple& point) const noexcept
{
    const Tuple shadowVector = (light.position - point).normalize();
    const float distanceToLight = (light.position - point).magnitude();
    const Ray shadowRay = Ray(point, shadowVector);
    const auto intersections = intersect(shadowRay);
    const auto hit = Ray::hit(intersections);

    const bool shadowed = hit && hit->t < distanceToLight;
    return shadowed;
}
