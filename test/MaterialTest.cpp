/*
 * MaterialTest.cpp
 *
 *  Created on: Sep 18, 2021
 *      Author: nic
 */

#include "gtest/gtest.h"
#include "Color.hpp"
#include "Material.hpp"
#include "Tuple.hpp"
#include "Light.hpp"
#include <cmath>

TEST(MaterialTest, DefaultMaterial)
{
	Material m;

	EXPECT_EQ(m.color, Color(1, 1, 1));
	EXPECT_FLOAT_EQ(m.ambient, 0.1);
	EXPECT_FLOAT_EQ(m.diffuse, 0.9);
	EXPECT_FLOAT_EQ(m.specular, 0.9);
	EXPECT_FLOAT_EQ(m.shininess, 200.0);
}

TEST(MaterialTest, CustomConstructor)
{
	Material m(Color(1, 0.5, 0.25), 0.5, 0.4, 0.7, 150.0, 0.0f, 0.0f, 1.0f);

	EXPECT_EQ(m.color, Color(1, 0.5, 0.25));
	EXPECT_FLOAT_EQ(m.ambient, 0.5);
	EXPECT_FLOAT_EQ(m.diffuse, 0.4);
	EXPECT_FLOAT_EQ(m.specular, 0.7);
	EXPECT_FLOAT_EQ(m.shininess, 150.0);
}

TEST(MaterialTest, LightingWithEyeBetweenLightAndSurface)
{
	Material m;
	Tuple position = Point(0, 0, 0);
	Tuple eyeV = Vector(0, 0, -1);
	Tuple normalV = Vector(0, 0, -1);
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	Color result = m.light(light, position, eyeV, normalV, false);

	EXPECT_EQ(result, Color(1.9, 1.9, 1.9));
}

TEST(MaterialTest, LightingWithEyeAtAngle)
{
	Material m;
	Tuple position = Point(0, 0, 0);
	Tuple eyeV = Vector(0, sqrt(2) / 2, sqrt(2) / 2);
	Tuple normalV = Vector(0, 0, -1);
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	Color result = m.light(light, position, eyeV, normalV, false);

	EXPECT_EQ(result, Color(1.0, 1.0, 1.0));
}

TEST(MaterialTest, LightingWithLightAtAngle)
{
	Material m;
	Tuple position = Point(0, 0, 0);
	Tuple eyeV = Vector(0, 0, -1);
	Tuple normalV = Vector(0, 0, -1);
	Light light(Point(0, 10, -10), Color(1, 1, 1));
	Color result = m.light(light, position, eyeV, normalV, false);

	EXPECT_EQ(result, Color(0.7364, 0.7364, 0.7364));
}

TEST(MaterialTest, LightingWithLightAndEyeAtOpposingAngle)
{
	Material m;
	Tuple position = Point(0, 0, 0);
	Tuple eyeV = Vector(0, -sqrt(2) / 2, -sqrt(2) / 2);
	Tuple normalV = Vector(0, 0, -1);
	Light light(Point(0, 10, -10), Color(1, 1, 1));
	Color result = m.light(light, position, eyeV, normalV, false);

	EXPECT_EQ(result, Color(1.6364, 1.6364, 1.6364));
}

TEST(MaterialTest, LightingWithLightBehindSurface)
{
	Material m;
	Tuple position = Point(0, 0, 0);
	Tuple eyeV = Vector(0, 0, -1);
	Tuple normalV = Vector(0, 0, -1);
	Light light(Point(0, 0, 10), Color(1, 1, 1));
	Color result = m.light(light, position, eyeV, normalV, false);

	EXPECT_EQ(result, Color(0.1, 0.1, 0.1));
}

TEST(MaterialTest, LightingWithSurfaceInShadow)
{
	Material m;
	Tuple position = Point(0, 0, 0);
	Tuple eyeV = Vector(0, 0, -1);
	Tuple normalV = Vector(0, 0, -1);
	Light light(Point(0, 0, -10), Color(1, 1, 1));
	bool inShadow = true;
	Color result = m.light(light, position, eyeV, normalV, inShadow);

	EXPECT_EQ(result, Color(0.1, 0.1, 0.1));
}

TEST(MaterialTest, DefaultMaterialReflectivity)
{
	Material m;

	EXPECT_FLOAT_EQ(m.reflectivity, 0.0f);
}

TEST(MaterialTest, DefaultMaterialTransparencyAndRefractiveIndex)
{
	Material m;

	EXPECT_FLOAT_EQ(m.transparency, 0.0f);
	EXPECT_FLOAT_EQ(m.refractiveIndex, 1.0f);
}














