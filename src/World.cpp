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
	s1.material.color = Color(0.8, 1.0, 0.6);
	s1.material.diffuse = 0.7;
	s1.material.specular = 0.2;

	Sphere s2;
	s2.transform = scaling(0.5, 0.5, 0.5);

	World w;
	w.objects.push_back(s1);
	w.objects.push_back(s2);
	w.light = Light(Point(-10, 10, -10), Color(1, 1, 1));

	return w;
}

const std::vector<Intersection> World::intersect(Ray r) const
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

const Color World::shadeHit(IntersectionDetails id) const
{
	return id.object.material.light(light, id.point, id.eyeVector, id.normalVector);
}

const Color World::colorAt(Ray r) const
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
