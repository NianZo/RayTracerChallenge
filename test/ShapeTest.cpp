/*
 * SphereTest.cpp
 *
 *  Created on: Apr 25, 2021
 *      Author: nic
 */

#include "Shape.hpp"
#include "ObjParser.hpp"
#include "gtest/gtest.h"
#include "Tuple.hpp"
#include "Ray.hpp"
#include "Transformation.hpp"
#include "Material.hpp"
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
	Sphere s;
	Shape& o = s;

	EXPECT_EQ(o.transform, IdentityMatrix());
}

TEST(ShapeTest, AssignTransformation)
{
	Sphere s;
	Shape& o = s;
	o.transform = (translation(2, 3, 4));

	EXPECT_EQ(o.transform, translation(2, 3, 4));
}

TEST(ShapeTest, DefaultMaterial)
{
	Sphere s;
	Shape& o = s;

	EXPECT_EQ(o.material, Material());
}

TEST(ShapeTest, AssignMaterial)
{
	Sphere s;
	Shape& o = s;
	Material m;
	m.ambient = 1.0f;
	o.material = m;

	EXPECT_EQ(o.material, m);
}

TEST(ShapeTest, CopyConstruction)
{
	Sphere s;
	s.transform = translation(4, 0, 0);

	Sphere s2 = s;

	EXPECT_EQ(s2.transform, translation(4, 0, 0));
}

TEST(ShapeTest, CopyAssignment)
{
	Sphere s;
	s.transform = translation(4, 0, 0);

	Sphere s2;
	s2.transform = translation(6, 5, 3);
	s2 = s;

	EXPECT_EQ(s2.transform, translation(4, 0, 0));
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

TEST(SphereTest, GlassSphereFactory)
{
	Sphere s = GlassSphere();

	EXPECT_EQ(s.transform, IdentityMatrix());
	EXPECT_FLOAT_EQ(s.material.transparency, 1.0f);
	EXPECT_FLOAT_EQ(s.material.refractiveIndex, 1.5f);
}

TEST(CubeTest, IntersectWithRay)
{
	Cube c;

	Ray r1(Point(5, 0.5, 0), Vector(-1, 0, 0));
	auto intersections1 = c.intersect(r1);
	EXPECT_EQ(intersections1.size(), 2);
	EXPECT_FLOAT_EQ(intersections1[0].t, 4);
	EXPECT_FLOAT_EQ(intersections1[1].t, 6);

	Ray r2(Point(-5, 0.5, 0), Vector(1, 0, 0));
	auto intersections2 = c.intersect(r2);
	EXPECT_EQ(intersections2.size(), 2);
	EXPECT_FLOAT_EQ(intersections2[0].t, 4);
	EXPECT_FLOAT_EQ(intersections2[1].t, 6);

	Ray r3(Point(0.5, 5, 0), Vector(0, -1, 0));
	auto intersections3 = c.intersect(r3);
	EXPECT_EQ(intersections3.size(), 2);
	EXPECT_FLOAT_EQ(intersections3[0].t, 4);
	EXPECT_FLOAT_EQ(intersections3[1].t, 6);

	Ray r4(Point(0.5, -5, 0), Vector(0, 1, 0));
	auto intersections4 = c.intersect(r4);
	EXPECT_EQ(intersections4.size(), 2);
	EXPECT_FLOAT_EQ(intersections4[0].t, 4);
	EXPECT_FLOAT_EQ(intersections4[1].t, 6);

	Ray r5(Point(0.5, 0, 5), Vector(0, 0, -1));
	auto intersections5 = c.intersect(r5);
	EXPECT_EQ(intersections5.size(), 2);
	EXPECT_FLOAT_EQ(intersections5[0].t, 4);
	EXPECT_FLOAT_EQ(intersections5[1].t, 6);

	Ray r6(Point(0.5, 0, -5), Vector(0, 0, 1));
	auto intersections6 = c.intersect(r6);
	EXPECT_EQ(intersections6.size(), 2);
	EXPECT_FLOAT_EQ(intersections6[0].t, 4);
	EXPECT_FLOAT_EQ(intersections6[1].t, 6);

	Ray r7(Point(0, 0.5, 0), Vector(0, 0, 1));
	auto intersections7 = c.intersect(r7);
	EXPECT_EQ(intersections7.size(), 2);
	EXPECT_FLOAT_EQ(intersections7[0].t, -1);
	EXPECT_FLOAT_EQ(intersections7[1].t, 1);
}

TEST(CubeTest, RayMissesCube)
{
	Cube c;

	Ray r1(Point(-2, 0, 0), Vector(0.2673, 0.5345, 0.8018));
	auto intersections1 = c.intersect(r1);
	EXPECT_EQ(intersections1.size(), 0);

	Ray r2(Point(0, -2, 0), Vector(0.8018, 0.2673, 0.5345));
	auto intersections2 = c.intersect(r2);
	EXPECT_EQ(intersections2.size(), 0);

	Ray r3(Point(0, 0, -2), Vector(0.5345, 0.8018, 0.2673));
	auto intersections3 = c.intersect(r3);
	EXPECT_EQ(intersections3.size(), 0);

	Ray r4(Point(2, 0, 2), Vector(0, 0, -1));
	auto intersections4 = c.intersect(r4);
	EXPECT_EQ(intersections4.size(), 0);

	Ray r5(Point(0, 2, 2), Vector(0, -1, 0));
	auto intersections5 = c.intersect(r5);
	EXPECT_EQ(intersections5.size(), 0);

	Ray r6(Point(2, 2, 0), Vector(-1, 0, 0));
	auto intersections6 = c.intersect(r6);
	EXPECT_EQ(intersections6.size(), 0);
}

TEST(CubeTest, CubeNormal)
{
	Cube c;

	Tuple p1 = Point(1, 0.5, -0.8);
	Tuple normal1 = c.normal(p1);
	EXPECT_EQ(normal1, Vector(1, 0, 0));

	Tuple p2 = Point(-1, -0.2, 0.9);
	Tuple normal2 = c.normal(p2);
	EXPECT_EQ(normal2, Vector(-1, 0, 0));

	Tuple p3 = Point(-0.4, 1, -0.1);
	Tuple normal3 = c.normal(p3);
	EXPECT_EQ(normal3, Vector(0, 1, 0));

	Tuple p4 = Point(0.3, -1, -0.7);
	Tuple normal4 = c.normal(p4);
	EXPECT_EQ(normal4, Vector(0, -1, 0));

	Tuple p5 = Point(-0.6, 0.3, 1);
	Tuple normal5 = c.normal(p5);
	EXPECT_EQ(normal5, Vector(0, 0, 1));

	Tuple p6 = Point(0.4, 0.4, -1);
	Tuple normal6 = c.normal(p6);
	EXPECT_EQ(normal6, Vector(0, 0, -1));

	Tuple p7 = Point(1, 1, 1);
	Tuple normal7 = c.normal(p7);
	EXPECT_EQ(normal7, Vector(1, 0, 0));

	Tuple p8 = Point(-1, -1, -1);
	Tuple normal8 = c.normal(p8);
	EXPECT_EQ(normal8, Vector(-1, 0, 0));
}

TEST(CylinderTest, RayMissesCylinder)
{
	Cylinder c;

	Ray r1(Point(1, 0, 0), Vector(0, 1, 0));
	auto intersections1 = c.intersect(r1);
	EXPECT_EQ(intersections1.size(), 0);

	Ray r2(Point(0, 0, 0), Vector(0, 1, 0));
	auto intersections2 = c.intersect(r2);
	EXPECT_EQ(intersections2.size(), 0);

	Ray r3(Point(0, 0, -5), Vector(1, 1, 1).normalize());
	auto intersections3 = c.intersect(r3);
	EXPECT_EQ(intersections3.size(), 0);
}

TEST(CylinderTest, RayHitsCylinder)
{
	Cylinder c;

	Ray r1(Point(1, 0, -5), Vector(0, 0, 1));
	auto intersections1 = c.intersect(r1);
	EXPECT_EQ(intersections1.size(), 2);
	EXPECT_FLOAT_EQ(intersections1[0].t, 5);
	EXPECT_FLOAT_EQ(intersections1[1].t, 5);

	Ray r2(Point(0, 0, -5), Vector(0, 0, 1));
	auto intersections2 = c.intersect(r2);
	EXPECT_EQ(intersections2.size(), 2);
	EXPECT_FLOAT_EQ(intersections2[0].t, 4);
	EXPECT_FLOAT_EQ(intersections2[1].t, 6);

	Ray r3(Point(0.5, 0, -5), Vector(0.1, 1, 1).normalize());
	auto intersections3 = c.intersect(r3);
	EXPECT_EQ(intersections3.size(), 2);
	EXPECT_FLOAT_EQ(intersections3[0].t, 6.808006);
	EXPECT_FLOAT_EQ(intersections3[1].t, 7.08870);
}

TEST(CylinderTest, NormalOfCylinder)
{
	Cylinder c;

	Tuple n1 = c.normal(Point(1, 0, 0));
	EXPECT_EQ(n1, Vector(1, 0, 0));

	Tuple n2 = c.normal(Point(0, 5, -1));
	EXPECT_EQ(n2, Vector(0, 0, -1));

	Tuple n3 = c.normal(Point(0, -2, 1));
	EXPECT_EQ(n3, Vector(0, 0, 1));

	Tuple n4 = c.normal(Point(-1, 1, 0));
	EXPECT_EQ(n4, Vector(-1, 0, 0));
}

TEST(CylinderTest, DefaultCylinderExtent)
{
	Cylinder c;

	EXPECT_FLOAT_EQ(c.minimum, -std::numeric_limits<float>::infinity());
	EXPECT_FLOAT_EQ(c.maximum, std::numeric_limits<float>::infinity());
}

TEST(CylinderTest, IntersectingConstrainedCylinder)
{
	Cylinder c;
	c.minimum = 1;
	c.maximum = 2;

	Ray r1(Point(0, 1.5, 0), Vector(0.1, 1, 0));
	auto intersections1 = c.intersect(r1);
	EXPECT_EQ(intersections1.size(), 0);

	Ray r2(Point(0, 3, -5), Vector(0, 0, 1));
	auto intersections2 = c.intersect(r2);
	EXPECT_EQ(intersections2.size(), 0);

	Ray r3(Point(0, 0, -5), Vector(0, 0, 1));
	auto intersections3 = c.intersect(r3);
	EXPECT_EQ(intersections3.size(), 0);

	Ray r4(Point(0, 2, -5), Vector(0, 0, 1));
	auto intersections4 = c.intersect(r4);
	EXPECT_EQ(intersections4.size(), 0);

	Ray r5(Point(0, 1, -5), Vector(0, 0, 1));
	auto intersections5 = c.intersect(r5);
	EXPECT_EQ(intersections5.size(), 0);

	Ray r6(Point(0, 1.5, -2), Vector(0, 0, 1));
	auto intersections6 = c.intersect(r6);
	EXPECT_EQ(intersections6.size(), 2);
}

TEST(CylinderTest, DefaultCylinderNotClosed)
{
	Cylinder c;

	EXPECT_FALSE(c.closed);
}

TEST(CylinderTest, IntersectingCapsOfClosedCylinder)
{
	Cylinder c;
	c.minimum = 1;
	c.maximum = 2;
	c.closed = true;

	Ray r1(Point(0, 3, 0), Vector(0, -1, 0));
	auto intersections1 = c.intersect(r1);
	EXPECT_EQ(intersections1.size(), 2);

	Ray r2(Point(0, 3, -2), Vector(0, -1, 2));
	auto intersections2 = c.intersect(r2);
	EXPECT_EQ(intersections2.size(), 2);

	Ray r3(Point(0, 4, -2), Vector(0, -1, 1));
	auto intersections3 = c.intersect(r3);
	EXPECT_EQ(intersections3.size(), 2);

	Ray r4(Point(0, 0, -2), Vector(0, 1, 2));
	auto intersections4 = c.intersect(r4);
	EXPECT_EQ(intersections4.size(), 2);

	Ray r5(Point(0, -1, -2), Vector(0, 1, 1));
	auto intersections5 = c.intersect(r5);
	EXPECT_EQ(intersections5.size(), 2);
}

TEST(CylinderTest, NormalVectorOnCylinderEndCaps)
{
	Cylinder c;
	c.minimum = 1;
	c.maximum = 2;
	c.closed = true;

	Tuple n1 = c.normal(Point(0, 1, 0));
	EXPECT_EQ(n1, Vector(0, -1, 0));

	Tuple n2 = c.normal(Point(0.5, 1, 0));
	EXPECT_EQ(n2, Vector(0, -1, 0));

	Tuple n3 = c.normal(Point(0, 1, 0.5));
	EXPECT_EQ(n3, Vector(0, -1, 0));

	Tuple n4 = c.normal(Point(0, 2, 0));
	EXPECT_EQ(n4, Vector(0, 1, 0));

	Tuple n5 = c.normal(Point(0.5, 2, 0));
	EXPECT_EQ(n5, Vector(0, 1, 0));

	Tuple n6 = c.normal(Point(0, 2, 0.5));
	EXPECT_EQ(n6, Vector(0, 1, 0));
}

TEST(ConeTest, ConeExtentDefault)
{
	Cone c;

	EXPECT_FLOAT_EQ(c.minimum, -std::numeric_limits<float>::infinity());
	EXPECT_FLOAT_EQ(c.maximum, std::numeric_limits<float>::infinity());
}

TEST(ConeTest, DefaultConeNotClosed)
{
	Cone c;

	EXPECT_FALSE(c.closed);
}

TEST(ConeTest, IntersectingConeWithRay)
{
	Cone c;

	Ray r1(Point(0, 0, -5), Vector(0, 0, 1));
	auto intersections1 = c.intersect(r1);
	EXPECT_EQ(intersections1.size(), 2);
	EXPECT_FLOAT_EQ(intersections1[0].t, 5);
	EXPECT_FLOAT_EQ(intersections1[1].t, 5);

	Ray r2(Point(0, 0, -5), Vector(1, 1, 1).normalize());
	auto intersections2 = c.intersect(r2);
	EXPECT_EQ(intersections2.size(), 2);
	EXPECT_FLOAT_EQ(intersections2[0].t, 8.6602545);
	EXPECT_FLOAT_EQ(intersections2[1].t, 8.6602545);

	Ray r3(Point(1, 1, -5), Vector(-0.5, -1, 1).normalize());
	auto intersections3 = c.intersect(r3);
	EXPECT_EQ(intersections3.size(), 2);
	EXPECT_FLOAT_EQ(intersections3[0].t, 4.5500546);
	EXPECT_FLOAT_EQ(intersections3[1].t, 49.44994);
}

TEST(ConeTest, IntersectingConeWithRayParallelToOneHalf)
{
	Cone c;

	Ray r(Point(0, 0, -1), Vector(0, 1, 1).normalize());
	auto intersections = c.intersect(r);
	EXPECT_EQ(intersections.size(), 1);
	EXPECT_FLOAT_EQ(intersections[0].t, 0.35355338);
}

TEST(ConeTest, IntersectingCapsOfClosedCone)
{
	Cone c;
	c.minimum = -0.5f;
	c.maximum = 0.5f;
	c.closed = true;

	Ray r1(Point(0, 0, -5), Vector(0, 1, 0));
	auto intersections1 = c.intersect(r1);
	EXPECT_EQ(intersections1.size(), 0);

	Ray r2(Point(0, 0, -0.25), Vector(0, 1, 1).normalize());
	auto intersections2 = c.intersect(r2);
	EXPECT_EQ(intersections2.size(), 2);

	Ray r3(Point(0, 0, -0.25), Vector(0, 1, 0));
	auto intersections3 = c.intersect(r3);
	EXPECT_EQ(intersections3.size(), 4);
}

TEST(ConeTest, NormalsOfCone)
{
	Cone c;

	Tuple n1 = c.normal(Point(0, 0, 0));
	EXPECT_EQ(n1, Vector(0, 0, 0).normalize());

	Tuple n2 = c.normal(Point(1, 1, 1));
	EXPECT_EQ(n2, Vector(1, -sqrt(2), 1).normalize());

	Tuple n3 = c.normal(Point(-1, -1, 0));
	EXPECT_EQ(n3, Vector(-1, 1, 0).normalize());

	Cone c2;
	c2.maximum = 2.0f;
	c2.minimum = -2.0f;
	c2.closed = true;

	Tuple n4 = c2.normal(Point(0, 2, 1.9));
	EXPECT_EQ(n4, Vector(0, 1, 0));

	Tuple n5 = c2.normal(Point(1.9, -2, 0));
	EXPECT_EQ(n5, Vector(0, -1, 0));
}

TEST(GroupTest, CreateNewGroup)
{
	Group g;

	EXPECT_EQ(g.transform, IdentityMatrix());
	EXPECT_TRUE(g.objects().empty());
}

TEST(GroupTest, AddChildToGroup)
{
	Group g;

	Group g2;
	g2.transform = translation(1, 0, 0);
	g.addChild(g2);
	Sphere sp;
	sp.transform = translation(2, 0, 0);
	g.addChild(sp);
	Plane p;
	p.transform = translation(3, 0, 0);
	g.addChild(p);
	Cube cu;
	cu.transform = translation(4, 0, 0);
	g.addChild(cu);
	Cylinder cy;
	cy.transform = translation(5, 0, 0);
	g.addChild(cy);
	Cone co;
	co.transform = translation(6, 0, 0);
	g.addChild(co);
	Triangle t(Point(1, 0, 0), Point(0, 1, 0), Point(0, 0, 1));
	t.transform = translation(7, 0, 0);
	g.addChild(t);
	SmoothTriangle st(Point(1, 0, 0), Point(0, 1, 0), Point(0, 0, 1), Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1));
	st.transform = translation(8, 0, 0);
	g.addChild(st);

	std::vector<std::reference_wrapper<const Shape>> shapeRefs = g.objects();
	EXPECT_FALSE(shapeRefs.empty());
	EXPECT_EQ(shapeRefs[0].get().transform, g2.transform);
	EXPECT_EQ(shapeRefs[1].get().transform, sp.transform);
	EXPECT_EQ(shapeRefs[2].get().transform, p.transform);
	EXPECT_EQ(shapeRefs[3].get().transform, cu.transform);
	EXPECT_EQ(shapeRefs[4].get().transform, cy.transform);
	EXPECT_EQ(shapeRefs[5].get().transform, co.transform);
	EXPECT_EQ(shapeRefs[6].get().transform, t.transform);
	EXPECT_EQ(shapeRefs[7].get().transform, st.transform);
	EXPECT_EQ(shapeRefs[0].get().parent, &g);
	EXPECT_EQ(shapeRefs[1].get().parent, &g);
	EXPECT_EQ(shapeRefs[2].get().parent, &g);
	EXPECT_EQ(shapeRefs[3].get().parent, &g);
	EXPECT_EQ(shapeRefs[4].get().parent, &g);
	EXPECT_EQ(shapeRefs[5].get().parent, &g);
	EXPECT_EQ(shapeRefs[6].get().parent, &g);
	EXPECT_EQ(shapeRefs[7].get().parent, &g);
}

TEST(GroupTest, IntersectRayWithEmptyGroup)
{
	Group g;
	Ray r(Point(0, 0, 0), Vector(0, 0, 1));

	auto intersections = g.intersect(r);
	EXPECT_TRUE(intersections.empty());
}

TEST(GroupTest, IntersectRayWithNonEmptyGroup)
{
	Group g;
	Sphere s1;
	Sphere s2;
	s2.transform = translation(0, 0, -3);
	Sphere s3;
	s3.transform = translation(5, 0, 0);
	g.addChild(s1);
	g.addChild(s2);
	g.addChild(s3);
	Ray r(Point(0, 0, -5), Vector(0, 0, 1));

	auto intersections = g.intersect(r);
	EXPECT_EQ(intersections.size(), 4);
	EXPECT_EQ(intersections[0].object->transform, s1.transform);
	EXPECT_EQ(intersections[1].object->transform, s1.transform);
	EXPECT_EQ(intersections[2].object->transform, s2.transform);
	EXPECT_EQ(intersections[3].object->transform, s2.transform);
}

TEST(GroupTest, IntersectRayWithTransformedGroup)
{
	Group g;
	g.transform = scaling(2, 2, 2);
	Sphere s;
	s.transform = translation(5, 0, 0);
	g.addChild(s);
	Ray r(Point(10, 0, -10), Vector(0, 0, 1));

	auto intersections = g.intersect(r);
	EXPECT_EQ(intersections.size(), 2);
}

TEST(GroupTest, NormalOfChildInGroup)
{
	Group g1;
	g1.transform = rotationY(std::numbers::pi / 2);
	Group g2;
	g2.transform = scaling(1, 2, 3);
	Sphere s;
	s.transform = translation(5, 0, 0);

	// Note: manually accessing these children should NOT be done; need it just for testing
	Group& g2Ref = g1.addChild(g2);
	Sphere& sRef = g2Ref.addChild(s);
	//Sphere& sRef = g2.addChild(s);
	//g1.groups[0].transform = scaling(1, 1, 1);

	Tuple n = sRef.normal(Point(1.7321, 1.1547, -5.5774));
	EXPECT_EQ(n, Vector(0.2857, 0.4286, -0.8571));
}

TEST(GroupTest, GroupCopyConstructor)
{
	Group g;

	Sphere sgc;
	sgc.transform = translation(1, 0, 0);
	Group gc;
	gc.transform = translation(2, 0, 0);
	gc.addChild(sgc);
	g.addChild(gc);

	Sphere s;
	s.transform = translation(3, 0, 0);
	g.addChild(s);

	Plane p;
	p.transform = translation(4, 0, 0);
	g.addChild(p);

	Cube cu;
	cu.transform = translation(5, 0, 0);
	g.addChild(cu);

	Cylinder cy;
	cy.transform = translation(6, 0, 0);
	g.addChild(cy);

	Cone co;
	co.transform = translation(7, 0, 0);
	g.addChild(co);

	Triangle t(Point(1, 0, 0), Point(0, 1, 0), Point(0, 0, 1));
	t.transform = translation(8, 0, 0);
	g.addChild(t);

	SmoothTriangle st(Point(1, 0, 0), Point(0, 1, 0), Point(0, 0, 1), Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1));
	st.transform = translation(9, 0, 0);
	g.addChild(st);

	// Exercise copy constructor
	Group g2(g);

	EXPECT_EQ(g.objects()[2].get().transform, g2.objects()[2].get().transform);
}

TEST(GroupTest, GroupCopyAssignment)
{
	Group g;

	Sphere sgc;
	sgc.transform = translation(1, 0, 0);
	Group gc;
	gc.transform = translation(2, 0, 0);
	gc.addChild(sgc);
	g.addChild(gc);

	Sphere s;
	s.transform = translation(3, 0, 0);
	g.addChild(s);

	Plane p;
	p.transform = translation(4, 0, 0);
	g.addChild(p);

	Cube cu;
	cu.transform = translation(5, 0, 0);
	g.addChild(cu);

	Cylinder cy;
	cy.transform = translation(6, 0, 0);
	g.addChild(cy);

	Cone co;
	co.transform = translation(7, 0, 0);
	g.addChild(co);

	Triangle t(Point(1, 0, 0), Point(0, 1, 0), Point(0, 0, 1));
	t.transform = translation(8, 0, 0);
	g.addChild(t);

	SmoothTriangle st(Point(1, 0, 0), Point(0, 1, 0), Point(0, 0, 1), Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1));
	st.transform = translation(9, 0, 0);
	g.addChild(st);

	// Exercise copy assignment
	Group g2;
	g2.transform = translation(10, 0, 0);
	g2 = g;

	EXPECT_EQ(g.objects()[2].get().transform, g2.objects()[2].get().transform);
}

TEST(GroupTest, GroupCopySelfAssignment)
{
	Group g;
	Group& gRef = g;
	gRef.transform = translation(1, 0, 0);

	g = gRef;

	EXPECT_EQ(g.transform, translation(1, 0, 0));
}

TEST(GroupTest, MoveConstruction)
{
	Group g;
	g.transform = translation(1, 0, 0);

	Group g2 = std::move(g);
	EXPECT_EQ(g2.transform, translation(1, 0, 0));
}

TEST(GroupTest, GroupNormalInvalid)
{
	Group g;

	EXPECT_EQ(g.normal(Point(1, 1, 1)), Vector(0, 0, 0));
}

TEST(TriangleTest, ConstructTriangle)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Triangle t(p1, p2, p3);

	EXPECT_EQ(t.vertices[0], p1);
	EXPECT_EQ(t.vertices[1], p2);
	EXPECT_EQ(t.vertices[2], p3);
}

TEST(TriangleTest, NormalVector)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Triangle t(p1, p2, p3);

	Tuple expectedNormal = Vector(0, 0, -1);

	EXPECT_EQ(t.normal(Point(0, 0.5, 0)), expectedNormal);
	EXPECT_EQ(t.normal(Point(-0.5, 0.75, 0)), expectedNormal);
	EXPECT_EQ(t.normal(Point(0.5, 0.25, 0)), expectedNormal);
}

TEST(TriangleTest, NoIntersectionWithParallelRay)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Triangle t(p1, p2, p3);
	Ray r(Point(0, -1, -2), Vector(0, 1, 0));

	auto intersections = t.intersect(r);

	EXPECT_TRUE(intersections.empty());
}

TEST(TriangleTest, RayMissesV0V2Edge)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Triangle t(p1, p2, p3);
	Ray r(Point(1, 1, -2), Vector(0, 0, 1));

	auto intersections = t.intersect(r);

	EXPECT_TRUE(intersections.empty());
}

TEST(TriangleTest, RayMissesV0V1Edge)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Triangle t(p1, p2, p3);
	Ray r(Point(-1, 1, -2), Vector(0, 0, 1));

	auto intersections = t.intersect(r);

	EXPECT_TRUE(intersections.empty());
}

TEST(TriangleTest, RayMissesV1V2Edge)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Triangle t(p1, p2, p3);
	Ray r(Point(0, -1, -2), Vector(0, 0, 1));

	auto intersections = t.intersect(r);

	EXPECT_TRUE(intersections.empty());
}

TEST(TriangleTest, RayIntersectionSucceeds)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Triangle t(p1, p2, p3);
	Ray r(Point(0, 0.5, -2), Vector(0, 0, 1));

	auto intersections = t.intersect(r);

	EXPECT_FALSE(intersections.empty());
	EXPECT_FLOAT_EQ(intersections[0].t, 2.0);
}

TEST(SmoothTriangleTest, NoIntersectionWithParallelRay)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	SmoothTriangle t(p1, p2, p3, Vector(0, 0, 0), Vector(0, 0, 0), Vector(0, 0, 0));
	Ray r(Point(0, -1, -2), Vector(0, 1, 0));

	auto intersections = t.intersect(r);

	EXPECT_TRUE(intersections.empty());
}

TEST(SmoothTriangleTest, RayMissesV0V2Edge)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	SmoothTriangle t(p1, p2, p3, Vector(0, 0, 0), Vector(0, 0, 0), Vector(0, 0, 0));
	Ray r(Point(1, 1, -2), Vector(0, 0, 1));

	auto intersections = t.intersect(r);

	EXPECT_TRUE(intersections.empty());
}

TEST(SmoothTriangleTest, RayMissesV0V1Edge)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	SmoothTriangle t(p1, p2, p3, Vector(0, 0, 0), Vector(0, 0, 0), Vector(0, 0, 0));
	Ray r(Point(-1, 1, -2), Vector(0, 0, 1));

	auto intersections = t.intersect(r);

	EXPECT_TRUE(intersections.empty());
}

TEST(SmoothTriangleTest, RayMissesV1V2Edge)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	SmoothTriangle t(p1, p2, p3, Vector(0, 0, 0), Vector(0, 0, 0), Vector(0, 0, 0));
	Ray r(Point(0, -1, -2), Vector(0, 0, 1));

	auto intersections = t.intersect(r);

	EXPECT_TRUE(intersections.empty());
}

TEST(SmoothTriangleTest, Construction)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Tuple n1 = Vector(0, 1, 0);
	Tuple n2 = Vector(-1, 0, 0);
	Tuple n3 = Vector(1, 0, 0);
	SmoothTriangle st(p1, p2, p3, n1, n2, n3);

	EXPECT_EQ(st.vertices[0], p1);
	EXPECT_EQ(st.vertices[1], p2);
	EXPECT_EQ(st.vertices[2], p3);
	EXPECT_EQ(st.normals[0], n1);
	EXPECT_EQ(st.normals[1], n2);
	EXPECT_EQ(st.normals[2], n3);
}

TEST(SmoothTriangleTest, IntersectionStoresUV)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Tuple n1 = Vector(0, 1, 0);
	Tuple n2 = Vector(-1, 0, 0);
	Tuple n3 = Vector(1, 0, 0);
	SmoothTriangle st(p1, p2, p3, n1, n2, n3);

	Ray r(Point(-0.2, 0.3, -2), Vector(0, 0, 1));
	auto intersections = st.intersect(r);

	EXPECT_FALSE(intersections.empty());
	EXPECT_EQ(intersections.size(), 1);
	EXPECT_FLOAT_EQ(intersections[0].u, 0.45);
	EXPECT_FLOAT_EQ(intersections[0].v, 0.25);
}

TEST(SmoothTriangleTest, NormalInterpolation)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Tuple n1 = Vector(0, 1, 0);
	Tuple n2 = Vector(-1, 0, 0);
	Tuple n3 = Vector(1, 0, 0);
	SmoothTriangle st(p1, p2, p3, n1, n2, n3);
	Ray r(Point(-0.2, 0.3, -2), Vector(0, 0, 1));
	auto intersections = st.intersect(r);

	EXPECT_EQ(intersections.size(), 1);
	EXPECT_EQ(st.normal(Point(0, 0, 0), intersections[0]), Vector(-0.5547, 0.83205, 0));
}

TEST(SmoothTriangleTest, NormalFromPrecomputeDetails)
{
	Tuple p1 = Point(0, 1, 0);
	Tuple p2 = Point(-1, 0, 0);
	Tuple p3 = Point(1, 0, 0);
	Tuple n1 = Vector(0, 1, 0);
	Tuple n2 = Vector(-1, 0, 0);
	Tuple n3 = Vector(1, 0, 0);
	SmoothTriangle st(p1, p2, p3, n1, n2, n3);
	Ray r(Point(-0.2, 0.3, -2), Vector(0, 0, 1));
	auto intersections = st.intersect(r);
	auto id = r.precomputeDetails(*r.hit(intersections), intersections);

	EXPECT_EQ(id.normalVector, Vector(-0.5547, 0.83205, 0));
}

TEST(ObjParserTest, IgnoreLineCountForEmptyString)
{
	std::string empty = "";
	ObjParser parser(empty);

	EXPECT_EQ(parser.ignoredLines, 0);
}

TEST(ObjParserTest, IgnoreUnrecognizedLines)
{
	std::string gibberish = "There was a young lady named Bright\nwho traveled much faster than light.\nShe set out one day\nin a relative way,\nand came back the previous night.\nvirgo";
	ObjParser parser(gibberish);

	EXPECT_EQ(parser.ignoredLines, 6);
}

TEST(ObjParserTest, ParseVertexData)
{
	std::string vertexData = "v -1 1 0\nv -1.0000 0.5000 0.0000\nv 1 0 0\nv 1 1 0";
	ObjParser parser(vertexData);

	EXPECT_EQ(parser.vertices.size(), 4);
	EXPECT_EQ(parser.vertices[0], Point(-1, 1, 0));
	EXPECT_EQ(parser.vertices[1], Point(-1, 0.5, 0));
	EXPECT_EQ(parser.vertices[2], Point(1, 0, 0));
	EXPECT_EQ(parser.vertices[3], Point(1, 1, 0));
}

TEST(ObjParserTest, ParseTriangleData)
{
	std::string data =
			"v -1 1 0\n"
			"v -1 0 0\n"
			"v 1 0 0\n"
			"v 1 1 0\n"
			"f 1 2 3\n"
			"f 1 3 4\n";
	ObjParser parser(data);

	auto objects = parser.defaultGroup.objects();

	EXPECT_EQ(objects.size(), 2);
	EXPECT_EQ(parser.vertices.size(), 4);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[0].get()).vertices[0], parser.vertices[0]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[0].get()).vertices[1], parser.vertices[1]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[0].get()).vertices[2], parser.vertices[2]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[1].get()).vertices[0], parser.vertices[0]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[1].get()).vertices[1], parser.vertices[2]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[1].get()).vertices[2], parser.vertices[3]);
}

TEST(ObjParserTest, ParsePolygonData)
{
	std::string data =
			"v -1 1 0\n"
			"v -1 0 0\n"
			"v 1 0 0\n"
			"v 1 1 0\n"
			"v 0 2 0\n"
			"f 1 2 3 4 5";
	ObjParser parser(data);

	auto objects = parser.defaultGroup.objects();

	EXPECT_EQ(objects.size(), 3);
	EXPECT_EQ(parser.vertices.size(), 5);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[0].get()).vertices[0], parser.vertices[0]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[0].get()).vertices[1], parser.vertices[1]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[0].get()).vertices[2], parser.vertices[2]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[1].get()).vertices[0], parser.vertices[0]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[1].get()).vertices[1], parser.vertices[2]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[1].get()).vertices[2], parser.vertices[3]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[2].get()).vertices[0], parser.vertices[0]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[2].get()).vertices[1], parser.vertices[3]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(objects[2].get()).vertices[2], parser.vertices[4]);
}

TEST(ObjParserTest, TrianglesInNamedGroups)
{
	std::string data =
			"v -1 1 0\n"
			"v -1 0 0\n"
			"v 1 0 0\n"
			"v 1 1 0\n"
			"g FirstGroup\n"
			"f 1 2 3\n"
			"g SecondGroup\n"
			"f 1 3 4\n";
	ObjParser parser(data);

	auto firstGroupObjects = parser.namedGroups["FirstGroup"].objects();
	auto secondGroupObjects = parser.namedGroups["SecondGroup"].objects();

	EXPECT_EQ(firstGroupObjects.size(), 1);
	EXPECT_EQ(secondGroupObjects.size(), 1);
	EXPECT_EQ(parser.vertices.size(), 4);
	EXPECT_EQ(dynamic_cast<const Triangle&>(firstGroupObjects[0].get()).vertices[0], parser.vertices[0]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(firstGroupObjects[0].get()).vertices[1], parser.vertices[1]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(firstGroupObjects[0].get()).vertices[2], parser.vertices[2]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(secondGroupObjects[0].get()).vertices[0], parser.vertices[0]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(secondGroupObjects[0].get()).vertices[1], parser.vertices[2]);
	EXPECT_EQ(dynamic_cast<const Triangle&>(secondGroupObjects[0].get()).vertices[2], parser.vertices[3]);
}

TEST(ObjParserTest, AllGroupsInDefaultGroup)
{
	std::string data =
			"v -1 1 0\n"
			"v -1 0 0\n"
			"v 1 0 0\n"
			"v 1 1 0\n"
			"g FirstGroup\n"
			"f 1 2 3\n"
			"g SecondGroup\n"
			"f 1 3 4\n";
	ObjParser parser(data);

	Group fullGroup = parser.getGroup();
	auto groupChildren = fullGroup.objects();

	EXPECT_EQ(dynamic_cast<const Triangle&>(dynamic_cast<const Group&>(groupChildren[1].get()).objects()[0].get()).vertices, dynamic_cast<const Triangle&>(parser.namedGroups["FirstGroup"].objects()[0].get()).vertices);
	EXPECT_EQ(dynamic_cast<const Triangle&>(dynamic_cast<const Group&>(groupChildren[0].get()).objects()[0].get()).vertices, dynamic_cast<const Triangle&>(parser.namedGroups["SecondGroup"].objects()[0].get()).vertices);
}

TEST(ObjParserTest, VertexNormalParsing)
{
	std::string data =
			"vn 0 0 1\n"
			"vn 0.707 0 -0.707\n"
			"vn 1 2 3";
	ObjParser parser(data);

	EXPECT_EQ(parser.normals.size(), 3);
	EXPECT_EQ(parser.normals[0], Vector(0, 0, 1));
	EXPECT_EQ(parser.normals[1], Vector(0.707, 0, -0.707));
	EXPECT_EQ(parser.normals[2], Vector(1, 2, 3));
}

TEST(ObjParserTest, FaceWithNormals)
{
	std::string data =
			"v 0 1 0\n"
			"v -1 0 0\n"
			"v 1 0 0\n"
			"vn -1 0 0\n"
			"vn 1 0 0\n"
			"vn 0 1 0\n"
			"f 1//3 2//1 3//2\n"
			"f 1/0/3 2/102/1 3/14/2";
	ObjParser parser(data);

	const SmoothTriangle& t1 = dynamic_cast<const SmoothTriangle&>(parser.defaultGroup.objects()[0].get());
	const SmoothTriangle& t2 = dynamic_cast<const SmoothTriangle&>(parser.defaultGroup.objects()[1].get());

	EXPECT_EQ(t1.vertices[0], parser.vertices[0]);
	EXPECT_EQ(t1.vertices[1], parser.vertices[1]);
	EXPECT_EQ(t1.vertices[2], parser.vertices[2]);
	EXPECT_EQ(t1.normals[0], parser.normals[2]);
	EXPECT_EQ(t1.normals[1], parser.normals[0]);
	EXPECT_EQ(t1.normals[2], parser.normals[1]);

	EXPECT_EQ(t2.vertices[0], parser.vertices[0]);
	EXPECT_EQ(t2.vertices[1], parser.vertices[1]);
	EXPECT_EQ(t2.vertices[2], parser.vertices[2]);
	EXPECT_EQ(t2.normals[0], parser.normals[2]);
	EXPECT_EQ(t2.normals[1], parser.normals[0]);
	EXPECT_EQ(t2.normals[2], parser.normals[1]);
}

TEST(ObjParserTest, VertexParsingError)
{
	std::string data = "v 0 0";
	try
	{
		ObjParser parser(data);
	} catch(std::runtime_error& e)
	{
		EXPECT_EQ(std::string(e.what()), "ObjParser: Unable to parse vertex");
	}
}

TEST(ObjParserTest, VertexNormalParsingError)
{
	std::string data = "vn 0 0";
	try
	{
		ObjParser parser(data);
	} catch(std::runtime_error& e)
	{
		EXPECT_EQ(std::string(e.what()), "ObjParser: Unable to parse vertex normal");
	}
}

TEST(ObjParserTest, FaceParsingError)
{
	std::string data = "f 0 0";
	try
	{
		ObjParser parser(data);
	} catch(std::runtime_error& e)
	{
		EXPECT_EQ(std::string(e.what()), "ObjParser: Unable to parse face");
	}
}

TEST(ObjParserTest, GroupParsingError)
{
	std::string data = "g";
	try
	{
		ObjParser parser(data);
	} catch(std::runtime_error& e)
	{
		EXPECT_EQ(std::string(e.what()), "ObjParser: Unable to parse group name");
	}
}

TEST(ConstructiveSolidGeometry, Creation)
{
	std::unique_ptr<Shape> s1 = std::make_unique<Sphere>();
	s1->transform = translation(1, 0, 0);
	std::unique_ptr<Shape> s2 = std::make_unique<Cube>();
	s2->transform = translation(2, 0, 0);

	CSG csg(CSG::Union, std::move(s1), std::move(s2));

	EXPECT_EQ(csg.operation, CSG::Union);
	EXPECT_EQ(csg.left->transform, translation(1, 0, 0));
	EXPECT_EQ(csg.right->transform, translation(2, 0, 0));
	EXPECT_EQ(csg.left->parent, &csg);
	EXPECT_EQ(csg.right->parent, &csg);
}

TEST(ConstructiveSolidGeometry, IntersectionAllowedUnion)
{
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Union, true, true, true));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Union, true, true, false));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Union, true, false, true));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Union, true, false, false));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Union, false, true, true));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Union, false, true, false));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Union, false, false, true));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Union, false, false, false));
}

TEST(ConstructiveSolidGeometry, IntersectionAllowedIntersection)
{
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Intersect, true, true, true));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Intersect, true, true, false));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Intersect, true, false, true));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Intersect, true, false, false));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Intersect, false, true, true));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Intersect, false, true, false));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Intersect, false, false, true));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Intersect, false, false, false));
}

TEST(ConstructiveSolidGeometry, IntersectionAllowedDifference)
{
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Difference, true, true, true));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Difference, true, true, false));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Difference, true, false, true));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Difference, true, false, false));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Difference, false, true, true));
	EXPECT_TRUE(CSG::intersectionAllowed(CSG::Difference, false, true, false));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Difference, false, false, true));
	EXPECT_FALSE(CSG::intersectionAllowed(CSG::Difference, false, false, false));
}

TEST(ConstructiveSolidGeometry, FilteringIntersectionListUnion)
{
	std::unique_ptr<Shape> s1 = std::make_unique<Sphere>();
	s1->transform = translation(1, 0, 0);
	std::unique_ptr<Shape> s2 = std::make_unique<Cube>();
	s2->transform = translation(2, 0, 0);

	CSG csgUnion(CSG::Union, std::move(s1), std::move(s2));
	std::vector<Intersection> intersections = {Intersection(1, csgUnion.left.get()), Intersection(2, csgUnion.right.get()), Intersection(3, csgUnion.left.get()), Intersection(4, csgUnion.right.get())};
	auto result = csgUnion.filterIntersections(intersections);

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], intersections[0]);
	EXPECT_EQ(result[1], intersections[3]);
}

TEST(ConstructiveSolidGeometry, FilteringIntersectionListIntersection)
{
	std::unique_ptr<Shape> s1 = std::make_unique<Sphere>();
	s1->transform = translation(1, 0, 0);
	std::unique_ptr<Shape> s2 = std::make_unique<Cube>();
	s2->transform = translation(2, 0, 0);

	CSG csg(CSG::Intersect, std::move(s1), std::move(s2));
	std::vector<Intersection> intersections = {Intersection(1, csg.left.get()), Intersection(2, csg.right.get()), Intersection(3, csg.left.get()), Intersection(4, csg.right.get())};
	auto result = csg.filterIntersections(intersections);

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], intersections[1]);
	EXPECT_EQ(result[1], intersections[2]);
}

TEST(ConstructiveSolidGeometry, FilteringIntersectionListDifference)
{
	std::unique_ptr<Shape> s1 = std::make_unique<Sphere>();
	s1->transform = translation(1, 0, 0);
	std::unique_ptr<Shape> s2 = std::make_unique<Cube>();
	s2->transform = translation(2, 0, 0);

	CSG csg(CSG::Difference, std::move(s1), std::move(s2));
	std::vector<Intersection> intersections = {Intersection(1, csg.left.get()), Intersection(2, csg.right.get()), Intersection(3, csg.left.get()), Intersection(4, csg.right.get())};
	auto result = csg.filterIntersections(intersections);

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], intersections[0]);
	EXPECT_EQ(result[1], intersections[1]);
}










