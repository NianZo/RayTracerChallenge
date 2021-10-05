/*
 * World.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: nic
 */

#include "World.hpp"
#include "Transformation.hpp"
#include <algorithm>

World World::BaseWorld()
{
	Sphere s1;
	s1.material.color = Color(0.8f, 1.0f, 0.6f);
	s1.material.diffuse = 0.7f;
	s1.material.specular = 0.2f;

	Sphere s2;
	s2.transform = scaling(0.5, 0.5, 0.5);

	World w;
	w.objects.push_back(s1);
	w.objects.push_back(s2);
	w.light = Light(Point(-10, 10, -10), Color(1, 1, 1));

	return w;
}

std::vector<Intersection> World::intersect(Ray r) const
{
	std::vector<Intersection> intersections;
	for (const auto& object : objects)
	{
		auto objectIntersections = r.intersect(object);
		for (const auto& intersection : objectIntersections)
		{
			intersections.push_back(intersection);
		}
	}
	std::sort(intersections.begin(), intersections.end());
	return intersections;
}

Color World::shadeHit(IntersectionDetails id) const
{
	const bool shadowed = isShadowed(id.overPoint);
	return id.object.material.light(light, id.point, id.eyeVector, id.normalVector, shadowed);
}

Color World::colorAt(Ray r) const
{
	std::vector<Intersection> intersections;
	for (const Sphere& object : objects)
	{
		const std::vector<Intersection> i = r.intersect(object);
		for (const Intersection& event : i)
		{
			intersections.push_back(event);
		}
	}
	auto hit = Ray::hit(intersections);
	if (hit)
	{
		return shadeHit(r.precomputeDetails(*hit));
	} else
	{
		return Color(0, 0, 0);
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
