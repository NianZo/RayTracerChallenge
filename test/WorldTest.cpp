/*
 * WorldTest.cpp
 *
 *  Created on: Sep 19, 2021
 *      Author: nic
 */

#include "gtest/gtest.h"

#include "../src/World.hpp"
#include "../src/Light.hpp"
#include "../src/Transformation.hpp"
#include "../src/Sphere.hpp"
#include "../src/Ray.hpp"

TEST(WorldTest, DefaultWorld)
{
	World w;

	EXPECT_TRUE(w.objects.empty());
	// EXPECT_TRUE(w.light);
}

TEST(WorldTest, BaseWorld)
{
	World w = World::BaseWorld();
	Light l(Point(-10, 10, -10), Color(1, 1, 1));
	Sphere s1;
	s1.material.color = Color(0.8, 1.0, 0.6);
	s1.material.diffuse = 0.7;
	s1.material.specular = 0.2;

	Sphere s2;
	s2.transform = scaling(0.5, 0.5, 0.5);

	// TODO sphere order shouldn't matter
	//EXPECT_EQ(w.lights.size(), 1);
	EXPECT_EQ(w.light, l);
	EXPECT_EQ(w.objects.size(), 2);
	EXPECT_EQ(w.objects[0], s1);
	EXPECT_EQ(w.objects[1], s2);
}

TEST(WorldTest, RayIntersectWorld)
{
	World w = World::BaseWorld();
	Ray r(Point(0, 0, -5), Vector(0, 0, 1));
	auto xs = w.intersect(r);

	EXPECT_EQ(xs.size(), 4);
	EXPECT_FLOAT_EQ(xs[0].t, 4);
	EXPECT_FLOAT_EQ(xs[1].t, 4.5);
	EXPECT_FLOAT_EQ(xs[2].t, 5.5);
	EXPECT_FLOAT_EQ(xs[3].t, 6);
}

// TODO re-write these, this should not be the model for using them...
TEST(WorldTest, ShadingIntersection)
{
	World w = World::BaseWorld();
	Ray r(Point(0, 0, -5), Vector(0, 0, 1));
	Sphere s = w.objects[0];
	Intersection i = r.intersect(s)[0];
	IntersectionDetails id = r.precomputeDetails(i);
	Color c = w.shadeHit(id);

	EXPECT_EQ(c, Color(0.38066, 0.47583, 0.2855));
}

TEST(WorldTest, ShadingIntersectionFromInside)
{
	World w = World::BaseWorld();
	w.light = Light(Point(0, 0.25, 0), Color(1, 1, 1));
	Ray r(Point(0, 0, 0), Vector(0, 0, 1));
	Sphere s = w.objects[1];
	Intersection i = r.intersect(s)[1];
	IntersectionDetails id = r.precomputeDetails(i);
	Color c = w.shadeHit(id);

	EXPECT_EQ(c, Color(0.90498, 0.90498, 0.90498));
}

TEST(WorldTest, ShadeRayIntersectionOnMiss)
{
	World w = World::BaseWorld();
	Ray r(Point(0, 0, -5), Vector(0, 1, 0));
	Color c = w.colorAt(r);

	EXPECT_EQ(c, Color(0, 0, 0));
}

TEST(WorldTest, ShadeRayIntersectionOnHit)
{
	World w = World::BaseWorld();
	Ray r(Point(0, 0, -5), Vector(0, 0, 1));
	Color c = w.colorAt(r);

	EXPECT_EQ(c, Color(0.38066, 0.47583, 0.2855));
}

TEST(WorldTest, ShadeRayIntersectionBetweenSpheres)
{
	World w = World::BaseWorld();
	Ray r(Point(0, 0, 0.75), Vector(0, 0, -1));
	Sphere& outerSphere = w.objects[0];
	outerSphere.material.ambient = 1;
	Sphere& innerSphere = w.objects[1];
	innerSphere.material.ambient = 1;
	Color c = w.colorAt(r);

	EXPECT_EQ(c, innerSphere.material.color);
}





