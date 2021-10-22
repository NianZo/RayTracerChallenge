/*
 * PatternTest.cpp
 *
 *  Created on: Oct 17, 2021
 *      Author: nic
 */

#include "gtest/gtest.h"
#include "../src/Material.hpp"
#include "../src/Sphere.hpp"
#include "../src/Transformation.hpp"

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











