/*
 * PatternTest.cpp
 *
 *  Created on: Oct 17, 2021
 *      Author: nic
 */

#include "gtest/gtest.h"

TEST(PatternTest, CreatingStripePattern)
{
	Pattern p = StripePattern(Color::White(), Color::Black());

	EXPECT_EQ(p.a, Color::White());
	EXPECT_EQ(p.b, Color::Black());
}

TEST(PatternTest, StripePatternConstantInY)
{
	Pattern p = StripePattern(Color::White(), Color::Black());

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White());
	EXPECT_EQ(p.colorAt(Point(0, 1, 0)), Color::White());
	EXPECT_EQ(p.colorAt(Point(0, 2, 0)), Color::White());
}

TEST(PatternTest, StripePatternConstantInZ)
{
	Pattern p = StripePattern(Color::White(), Color::Black());

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White());
	EXPECT_EQ(p.colorAt(Point(0, 0, 1)), Color::White());
	EXPECT_EQ(p.colorAt(Point(0, 0, 2)), Color::White());
}

TEST(PatternTest, StripeOscillatesInX)
{
	Pattern p = StripePattern(Color::White(), Color::Black());

	EXPECT_EQ(p.colorAt(Point(0, 0, 0)), Color::White());
	EXPECT_EQ(p.colorAt(Point(0.9, 0, 0)), Color::White());
	EXPECT_EQ(p.colorAt(Point(1.0, 0, 0)), Color::Black());
	EXPECT_EQ(p.colorAt(Point(1.9, 0, 0)), Color::Black());
	EXPECT_EQ(p.colorAt(Point(2.0, 0, 0)), Color::White());
}

Test(PatternTest, StripePatternShadedProperly)
{
	Material m;
	m.pattern = StripePattern(Color::White(), Color:Black());
	m.ambient = 1;
	m.diffuse = 0;
	m.specular = 0;
	Tuple eyeV = Vector(0, 0, -1);
	Tuple normalV = Vector(0, 0, -1);
	Light light(Point(0, 0, -10), Color(1, 1, 1));

	Color result = m.light(light, Point(0.9, 0, 0), eyeV, normalV, false);
	Color result2 = m.light(light, Point(1.1, 0, 0), eyeV, normalV, false);

	EXPECT_EQ(result, Color::White());
	EXPECT_EQ(result2, Color::Black());
}
