/*
 * RayTest.cpp
 *
 *  Created on: Apr 22, 2021
 *      Author: nic
 */

#include "gtest/gtest.h"
#include "../src/Tuple.hpp"
#include "../src/Transformation.hpp"
#include "../src/Ray.hpp"
#include <cmath>


TEST(RayTest, RayCreation)
{
	Ray r(Point(1, 2, 3), Vector(4, 5, 6));
	EXPECT_EQ(r.origin, Point(1, 2, 3));
	EXPECT_EQ(r.direction, Vector(4, 5, 6));
}

TEST(RayTest, RayCast)
{
	Ray r(Point(2, 3, 4), Vector(1, 0, 0));
	EXPECT_EQ(r.cast(0), Point(2, 3, 4));
	EXPECT_EQ(r.cast(1), Point(3, 3, 4));
	EXPECT_EQ(r.cast(-1), Point(1, 3, 4));
	EXPECT_EQ(r.cast(2.5), Point(4.5, 3, 4));
}

TEST(RayTest, RayTranslation)
{
	Ray r(Point(1, 2, 3), Vector(0, 1, 0));
	Matrix<4> m = translation(3, 4, 5);
	Ray r2 = r.transform(m);

	EXPECT_EQ(r2.origin, Point(4, 6, 8));
	EXPECT_EQ(r2.direction, Vector(0, 1, 0));
}

TEST(RayTest, RayScaling)
{
	Ray r(Point(1, 2, 3), Vector(0, 1, 0));
	Matrix<4> m = scaling(2, 3, 4);
	Ray r2 = r.transform(m);

	EXPECT_EQ(r2.origin, Point(2, 6, 12));
	EXPECT_EQ(r2.direction, Vector(0, 3, 0));
}

TEST(RayTest, PrecomputeIntersectionParametersOutside)
{
	Ray r(Point(0, 0, -5), Vector(0, 0, 1));
	Sphere s;
	auto intersection = s.intersect(r);
	IntersectionDetails id = r.precomputeDetails(intersection[0]);

	EXPECT_FLOAT_EQ(id.t, intersection[0].t);
	EXPECT_EQ(id.object, s);
	EXPECT_EQ(id.point, Point(0, 0, -1));
	EXPECT_EQ(id.eyeVector, Vector(0, 0, -1));
	EXPECT_EQ(id.normalVector, Vector(0, 0, -1));
	EXPECT_FALSE(id.inside);
}

TEST(RayTest, PrecomputeIntersectionParametersInside)
{
	Ray r(Point(0, 0, 0), Vector(0, 0, 1));
	Sphere s;
	auto intersections = s.intersect(r);
	IntersectionDetails id = r.precomputeDetails(intersections[1]);

	EXPECT_FLOAT_EQ(id.t, intersections[1].t);
	EXPECT_EQ(id.object, s);
	EXPECT_EQ(id.point, Point(0, 0, 1));
	EXPECT_EQ(id.eyeVector, Vector(0, 0, -1));
	EXPECT_EQ(id.normalVector, Vector(0, 0, -1));
	EXPECT_TRUE(id.inside);
}

TEST(RayTest, PrecomputeIntersectionCalculatesOverPoint)
{
	Ray r(Point(0, 0, -5), Vector(0, 0, 1));
	Sphere s;
	s.transform = translation(0, 0, 1);
	auto i = s.intersect(r);
	auto id = r.precomputeDetails(*r.hit(i));

	EXPECT_LT(id.overPoint.z, -TUPLE_EPSILON / 2);
	EXPECT_LT(id.overPoint.z, id.point.z);
}

TEST(RayTest, PrecomputeIntersectionCalculatesReflectionVector)
{
	Plane p = Plane();
	Ray r = Ray(Point(0, 1, -1), Vector(0, -sqrt(2) / 2.0, sqrt(2) / 2.0));
	auto intersections = p.intersect(r);
	IntersectionDetails id = r.precomputeDetails(*r.hit(intersections));

	EXPECT_EQ(id.reflectionVector, Vector(0, sqrt(2) / 2, sqrt(2) / 2));
}










