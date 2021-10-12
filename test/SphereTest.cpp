/*
 * SphereTest.cpp
 *
 *  Created on: Apr 25, 2021
 *      Author: nic
 */

#include "gtest/gtest.h"
#include "../src/Tuple.hpp"
#include "../src/Ray.hpp"
#include "../src/Sphere.hpp"
#include "../src/Transformation.hpp"
#include "../src/Material.hpp"
#include <cmath>
#include <numbers>


TEST(SphereTest, RaySphereIntersectionNormal)
{
	Ray r(Point(0, 0, -5), Vector(0, 0, 1));
	Sphere s;
	auto xs = s.intersect(r);

	EXPECT_EQ(xs.size(), 2);
	EXPECT_FLOAT_EQ(xs[0].t, 4.0f);
	EXPECT_FLOAT_EQ(xs[1].t, 6.0f);
	EXPECT_EQ(*(xs[0].object), s);
	EXPECT_EQ(*(xs[1].object), s);
}

TEST(SphereTest, RaySphereIntersectionTangent)
{
	Ray r(Point(0, 1, -5), Vector(0, 0, 1));
	Sphere s;
	auto xs = s.intersect(r);

	EXPECT_EQ(xs.size(), 2);
	EXPECT_FLOAT_EQ(xs[0].t, 5.0f);
	EXPECT_FLOAT_EQ(xs[1].t, 5.0f);
}

TEST(SphereTest, RaySphereIntersectionMiss)
{
	Ray r(Point(0, 2, -5), Vector(0, 0, 1));
	Sphere s;
	auto xs = s.intersect(r);
	EXPECT_EQ(xs.size(), 0);
}

TEST(SphereTest, RaySphereIntersectionInsideSphere)
{
	Ray r(Point(0, 0, 0), Vector(0, 0, 1));
	Sphere s;
	auto xs = s.intersect(r);

	EXPECT_EQ(xs.size(), 2);
	EXPECT_FLOAT_EQ(xs[0].t, -1.0f);
	EXPECT_FLOAT_EQ(xs[1].t, 1.0f);
}

TEST(SphereTest, RaySphereIntersectionSphereBehindRay)
{
	Ray r(Point(0, 0, 5), Vector(0, 0, 1));
	Sphere s;
	auto xs = s.intersect(r);

	EXPECT_EQ(xs.size(), 2);
	EXPECT_FLOAT_EQ(xs[0].t, -6.0f);
	EXPECT_FLOAT_EQ(xs[1].t, -4.0f);
}

TEST(SphereTest, SphereHitCalculation)
{
	Sphere s;
	const std::vector<Intersection> xs = {Intersection(1, &s), Intersection(2, &s)};
	auto i  = Ray::hit(xs);
	EXPECT_EQ(*i, xs[0]);

	const std::vector<Intersection> xs2 = {Intersection(-1, &s), Intersection(1, &s)};
	auto i2 = Ray::hit(xs2);
	EXPECT_EQ(*i2, xs2[1]);

	const std::vector<Intersection> xs3 = {Intersection(-2, &s), Intersection(-1, &s)};
	auto i3 = Ray::hit(xs3);
	EXPECT_FALSE(i3);

	const std::vector<Intersection> xs4 = {
			Intersection(5, &s),
			Intersection(7, &s),
			Intersection(-3, &s),
			Intersection(2, &s)
	};
	auto i4 = Ray::hit(xs4);
	EXPECT_EQ(*i4, xs4[3]);
}
/*
TEST(SphereTest, DefaultTransform)
{
	Sphere s;

	EXPECT_EQ(s.transform, IdentityMatrix());
}

TEST(SphereTest, ChangeTransform)
{
	Sphere s;
	Matrix<4> t = translation(2, 3, 4);
	s.transform = t;

	EXPECT_EQ(s.transform, t);
}
*/
TEST(SphereTest, ScaledSphereRayIntersection)
{
	Ray r(Point(0, 0, -5), Vector(0, 0, 1));
	Sphere s;
	s.transform = scaling(2, 2, 2);
	auto xs = s.intersect(r);

	EXPECT_EQ(xs.size(), 2);
	EXPECT_FLOAT_EQ(xs[0].t, 3);
	EXPECT_FLOAT_EQ(xs[1].t, 7);
}

TEST(SphereTest, TranslatedSphereRayIntersection)
{
	Ray r(Point(0, 0, -5), Vector(0, 0, 1));
	Sphere s;
	s.transform = translation(5, 0, 0);
	auto xs = s.intersect(r);

	EXPECT_EQ(xs.size(), 0);
}

TEST(SphereTest, NormalsOnUnitSphere)
{
	Sphere s;

	Tuple n1 = s.normal(Point(1, 0, 0));
	Tuple n2 = s.normal(Point(0, 1, 0));
	Tuple n3 = s.normal(Point(0, 0, 1));
	Tuple n4 = s.normal(Point(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));

	EXPECT_EQ(n1, Vector(1, 0, 0));
	EXPECT_EQ(n2, Vector(0, 1, 0));
	EXPECT_EQ(n3, Vector(0, 0, 1));
	EXPECT_EQ(n4, Vector(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
}

TEST(SphereTest, NormalsAreNormalized)
{
	Sphere s;
	s.transform = scaling(2, 2, 2);

	Tuple n1 = s.normal(Point(2, 0, 0));
	Tuple n2 = s.normal(Point(0, 2, 0));
	Tuple n3 = s.normal(Point(0, 0, 2));
	Tuple n4 = s.normal(Point(2 * sqrt(3) / 3, 2 * sqrt(3) / 3, 2 * sqrt(3) / 3));

	EXPECT_EQ(n1, Vector(1, 0, 0));
	EXPECT_EQ(n2, Vector(0, 1, 0));
	EXPECT_EQ(n3, Vector(0, 0, 1));
	EXPECT_EQ(n4, Vector(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
}

TEST(SphereTest, NormalOfTranslatedSphere)
{
	Sphere s;
	s.transform = translation(0, 1, 0);
	Tuple n = s.normal(Point(0, 1.70711, -0.70711));

	EXPECT_EQ(n, Vector(0, 0.70711, -0.70711));
}

TEST(SphereTest, NormalOfTransformedSphere)
{
	Sphere s;
	s.transform = scaling(1, 0.5, 1) * rotationZ(std::numbers::pi / 5.);
	Tuple n = s.normal(Vector(0, sqrt(2) / 2, -sqrt(2) / 2));

	EXPECT_EQ(n, Vector(0, 0.97014, -0.24254));
}
/*
TEST(SphereTest, DefaultMaterial)
{
	Sphere s;

	EXPECT_EQ(s.material, Material());
}

TEST(SphereTest, AssignMaterial)
{
	Sphere s;
	Material m;
	m.ambient = 1.0f;
	s.material = m;

	EXPECT_EQ(s.material, m);
}
*/
TEST(ShapeTest, DefaultTransformation)
{
	Shape o;

	EXPECT_EQ(o.transform, IdentityMatrix());
}

TEST(ShapeTest, AssignTransformation)
{
	Shape o;
	o.transform = (translation(2, 3, 4));

	EXPECT_EQ(o.transform, translation(2, 3, 4));
}

TEST(ShapeTest, DefaultMaterial)
{
	Shape o;

	EXPECT_EQ(o.material, Material());
}

TEST(ShapeTest, AssignMaterial)
{
	Shape o;
	Material m;
	m.ambient = 1.0f;
	o.material = m;

	EXPECT_EQ(o.material, m);
}

TEST(PlaneTest, NormalConstantEverywhere)
{
	Plane p;

	EXPECT_EQ(p.normal(Point(0, 0, 0)), Vector(0, 1, 0));
	EXPECT_EQ(p.normal(Point(10, 0, -10)), Vector(0, 1, 0));
	EXPECT_EQ(p.normal(Point(-5, 0, 150)), Vector(0, 1, 0));
}

TEST(PlaneTest, IntersectWithParallelRay)
{
	Plane p;
	Ray r = Ray(Point(0, 10, 0), Vector(0, 0, 1));
	auto xs = p.intersect(r);

	EXPECT_EQ(xs.size(), 0);
}

TEST(PlaneTest, IntersectWithCoplanarRay)
{
	Plane p;
	Ray r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
	auto xs = p.intersect(r);

	EXPECT_EQ(xs.size(), 0);
}

TEST(PlaneTest, IntersectWithRayFromAbove)
{
	Plane p;
	Ray r = Ray(Point(0, 1, 0), Vector(0, -1, 0));
	auto xs = p.intersect(r);

	EXPECT_EQ(xs.size(), 1);
	EXPECT_EQ(xs[0].t, 1);
	EXPECT_EQ(*xs[0].object, p);
}

TEST(PlaneTest, IntersectWithRayFromBelow)
{
	Plane p;
	Ray r = Ray(Point(0, -1, 0), Vector(0, 1, 0));
	auto xs = p.intersect(r);

	EXPECT_EQ(xs.size(), 1);
	EXPECT_EQ(xs[0].t, 1);
	EXPECT_EQ(*xs[0].object, p);
}







