/*
 * SphereTest.cpp
 *
 *  Created on: Apr 25, 2021
 *      Author: nic
 */

#include "Shape.hpp"
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

	std::vector<std::reference_wrapper<const Shape>> shapeRefs = g.objects();
	EXPECT_FALSE(shapeRefs.empty());
	EXPECT_EQ(shapeRefs[0].get().transform, g2.transform);
	EXPECT_EQ(shapeRefs[1].get().transform, sp.transform);
	EXPECT_EQ(shapeRefs[2].get().transform, p.transform);
	EXPECT_EQ(shapeRefs[3].get().transform, cu.transform);
	EXPECT_EQ(shapeRefs[4].get().transform, cy.transform);
	EXPECT_EQ(shapeRefs[5].get().transform, co.transform);
	EXPECT_EQ(shapeRefs[0].get().parent, &g);
	EXPECT_EQ(shapeRefs[1].get().parent, &g);
	EXPECT_EQ(shapeRefs[2].get().parent, &g);
	EXPECT_EQ(shapeRefs[3].get().parent, &g);
	EXPECT_EQ(shapeRefs[4].get().parent, &g);
	EXPECT_EQ(shapeRefs[5].get().parent, &g);
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







