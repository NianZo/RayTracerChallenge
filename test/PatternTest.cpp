/*
 * PatternTest.cpp
 *
 *  Created on: Oct 17, 2021
 *      Author: nic
 */

#include "Shape.hpp"
#include "gtest/gtest.h"
#include "Material.hpp"
#include "Transformation.hpp"

TEST(PatternTest, CreatingStripePattern)
{
	Pattern p = Pattern::Stripe(Color::White, Color::Black);

	EXPECT_EQ(p.a, Color::White);
	EXPECT_EQ(p.b, Color::Black);
}

TEST(PatternTest, StripePatternConstantInY)
{
	Pattern p = Pattern::Stripe(Color::White, Color::Black);

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 1, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 2, 0)), Color::White);
}

TEST(PatternTest, StripePatternConstantInZ)
{
	Pattern p = Pattern::Stripe(Color::White, Color::Black);

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 0, 1)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 0, 2)), Color::White);
}

TEST(PatternTest, StripeOscillatesInX)
{
	Pattern p = Pattern::Stripe(Color::White, Color::Black);

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0.9, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(1.0, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(1.9, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(2.0, 0, 0)), Color::White);
	// TEST ESCAPES!!! Seeing no stripes in negative x in scene render
	EXPECT_EQ(p.colorAt(Point(-0.1, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(-1.0, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(-1.1, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(-2.0, 0, 0)), Color::White);
}

TEST(PatternTest, StripePatternShadedProperly)
{
	Material m;
	m.pattern = Pattern::Stripe(Color::White, Color::Black);
	m.ambient = 1;
	m.diffuse = 0;
	m.specular = 0;
	Tuple eyeV = Vector(0, 0, -1);
	Tuple normalV = Vector(0, 0, -1);
	Light light(Point(0, 0, -10), Color(1, 1, 1));

	Color result = m.light(light, Point(0.9, 0, 0), eyeV, normalV, false);
	Color result2 = m.light(light, Point(1.1, 0, 0), eyeV, normalV, false);

	EXPECT_EQ(result, Color::White);
	EXPECT_EQ(result2, Color::Black);
}

TEST(PatternTest, StripesWithObjectTransform)
{
	Plane s;
	s.transform = scaling(2, 2, 2);
	s.material.pattern = Pattern::Stripe(Color::White, Color::Black);
	s.material.ambient = 1;
	s.material.diffuse = 0;
	s.material.specular = 0;
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	Tuple eyeV = Vector(0, -1, 0);
	Color c = s.shade(light, Point(1.5, 0, 0), eyeV, false);

	EXPECT_EQ(c, Color::White);
}

TEST(PatternTest, StripesWithPatternTransformation)
{
	Plane s;
	s.material.pattern = Pattern::Stripe(Color::White, Color::Black);
	s.material.pattern->transform = scaling(2, 2, 2);
	s.material.ambient = 1;
	s.material.diffuse = 0;
	s.material.specular = 0;
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	Tuple eyeV = Vector(0, -1, 0);
	Color c = s.shade(light, Point(1.5, 0, 0), eyeV, false);

	EXPECT_EQ(c, Color::White);
}

TEST(PatternTest, StripesWithObjectAndPatternTransformation)
{
	Plane s;
	s.transform = scaling(2, 2, 2);
	s.material.pattern = Pattern::Stripe(Color::White, Color::Black);
	s.material.pattern->transform = translation(0.5, 0, 0);
	s.material.ambient = 1;
	s.material.diffuse = 0;
	s.material.specular = 0;
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	Tuple eyeV = Vector(0, -1, 0);
	Color c = s.shade(light, Point(2.5, 0, 0), eyeV, false);

	EXPECT_EQ(c, Color::White);
}

TEST(PatternTest, GradientLinearlyInterpolatesBetweenColors)
{
	Pattern p = Pattern::Gradient(Color::White, Color::Black);

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0.25, 0, 0)), Color(0.75, 0.75, 0.75));
	EXPECT_EQ(p.colorAt(Point(0.5, 0, 0)), Color(0.5, 0.5, 0.5));
	EXPECT_EQ(p.colorAt(Point(0.75, 0, 0)), Color(0.25, 0.25, 0.25));
}

TEST(PatternTest, RingExtendsInXAndZ)
{
	Pattern p = Pattern::Ring(Color::White, Color::Black);

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(1, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, 0, 1)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0.708, 0, 0.708)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(2, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 0, 2)), Color::White);
	EXPECT_EQ(p.colorAt(Point(1.416, 0, 1.416)), Color::White);
	EXPECT_EQ(p.colorAt(Point(-1, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, 0, -1)), Color::Black);
}

TEST(PatternTest, CheckersRepeatInX)
{
	Pattern p = Pattern::Checker(Color::White, Color::Black);

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0.99, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(1, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(-0.99, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(-1, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(-1.01, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(1.99, 0, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(2, 0, 0)), Color::White);
}

TEST(PatternTest, CheckersRepeatInY)
{
	Pattern p = Pattern::Checker(Color::White, Color::Black);

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 0.99, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 1, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, -0.99, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, -1, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, -1.01, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 1.99, 0)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, 2, 0)), Color::White);
}

TEST(PatternTest, CheckersRepeatInZ)
{
	Pattern p = Pattern::Checker(Color::White, Color::Black);

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 0, 0.99)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 0, 1)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, 0, -0.99)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, 0, -1)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, 0, -1.01)), Color::White);
	EXPECT_EQ(p.colorAt(Point(0, 0, 1.99)), Color::Black);
	EXPECT_EQ(p.colorAt(Point(0, 0, 2)), Color::White);
}

TEST(PatternTest, TestPatternWithObjectTransform)
{
	Sphere s = Sphere();
	s.transform = scaling(2, 2, 2);
	s.material.pattern = Pattern::Test();
	s.material.ambient = 1;
	s.material.diffuse = 0;
	s.material.specular = 0;
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	Tuple eyeV = Vector(0, -1, 0);
	Color c = s.shade(light, Point(2, 3, 4), eyeV, false);

	EXPECT_EQ(c, Color(1.0, 1.5, 2.0));
}

TEST(PatternTest, TestPatternWithPatternTransform)
{
	Sphere s = Sphere();
	s.material.pattern = Pattern::Test();
	s.material.pattern->transform = scaling(2, 2, 2);
	s.material.ambient = 1;
	s.material.diffuse = 0;
	s.material.specular = 0;
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	Tuple eyeV = Vector(0, -1, 0);
	Color c = s.shade(light, Point(2, 3, 4), eyeV, false);

	EXPECT_EQ(c, Color(1.0, 1.5, 2.0));
}

TEST(PatternTest, TestPatternWithObjectAndPatternTransform)
{
	Sphere s = Sphere();
	s.transform = scaling(2, 2, 2);
	s.material.pattern = Pattern::Test();
	s.material.pattern->transform = translation(0.5, 1, 1.5);
	s.material.ambient = 1;
	s.material.diffuse = 0;
	s.material.specular = 0;
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	Tuple eyeV = Vector(0, -1, 0);
	Color c = s.shade(light, Point(2.5, 3, 3.5), eyeV, false);

	EXPECT_EQ(c, Color(0.75, 0.5, 0.25));
}

TEST(PatternTest, TestPatternWithGroupAndPatternTransform)
{
	Sphere s;
	s.transform = translation(1, 0, 0);
	s.material.pattern = Pattern::Test();
	s.material.pattern->transform = scaling(2, 2, 2);
	s.material.ambient = 1;
	s.material.diffuse = 0;
	s.material.specular = 0;
	Group g1;
	g1.transform = translation(0, 0, 3);
	Group g2;
	g2.transform = translation(0, 2, 0);

	// Note: manually accessing these children should NOT be done; need it just for testing
	Group& g2Ref = g1.addChild(g2);
	Sphere& sRef = g2Ref.addChild(s);
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	Tuple eyeV = Vector(0, -1, 0);
	Color c = sRef.shade(light, Point(2.5, 3, 3.5), eyeV, false);

	EXPECT_EQ(c, Color(0.75, 0.5, 0.25));
}










