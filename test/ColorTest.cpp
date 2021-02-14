/*
 * ColorTest.cpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#include "../src/Color.hpp"

#include "gtest/gtest.h"

#include <cmath>


TEST(ColorTest, CreateColor)
{
	Color c = Color(-0.5, 0.4, 1.7);

	EXPECT_FLOAT_EQ(c.r, -0.5);
	EXPECT_FLOAT_EQ(c.g, 0.4);
	EXPECT_FLOAT_EQ(c.b, 1.7);
}

TEST(ColorTest, ColorEquality)
{
	Color c1 = Color(-0.5, 0.4, 1.7);
	Color c2 = Color(-0.5, 0.4, 1.7);

	EXPECT_TRUE(c1 == c2);
	EXPECT_FALSE(c1 == Color(0.5, 0.4, 1.7));
	EXPECT_FALSE(c1 == Color(-0.5, 0.5, 1.7));
	EXPECT_FALSE(c1 == Color(-0.5, 0.4, 1.6));
}

TEST(ColorTest, ColorInequality)
{
	Color c1 = Color(-0.5, 0.4, 1.7);

	EXPECT_TRUE(c1 != Color(0.5, 0.4, 1.7));
	EXPECT_TRUE(c1 != Color(-0.5, 0.5, 1.7));
	EXPECT_TRUE(c1 != Color(-0.5, 0.4, 1.6));
	EXPECT_FALSE(c1 != Color(-0.5, 0.4, 1.7));
}

TEST(ColorTest, AddColors)
{
	Color c1 = Color(0.9, 0.6, 0.75);
	Color c2 = Color(0.7, 0.1, 0.25);

	EXPECT_EQ(c1 + c2, Color(1.6, 0.7, 1.0));
}

TEST(ColorTest, SubtractColors)
{
	Color c1 = Color(0.9, 0.6, 0.75);
	Color c2 = Color(0.7, 0.1, 0.25);

	EXPECT_EQ(c1 - c2, Color(0.2, 0.5, 0.5));
}

TEST(ColorTest, ScalarMultiplication)
{
	Color c = Color(0.2, 0.3, 0.4);

	EXPECT_EQ(c * 2, Color(0.4, 0.6, 0.8));
}

TEST(ColorTest, MultiplyColors)
{
	Color c1 = Color(1, 0.2, 0.4);
	Color c2 = Color(0.9, 1, 0.1);

	EXPECT_EQ(c1 * c2, Color(0.9, 0.2, 0.04));
}


