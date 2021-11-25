/*
 * CanvasTest.cpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#include "Canvas.hpp"
#include "gtest/gtest.h"
#include <cmath>
#include <string_view>
#include <string>
#include <iostream>

TEST(CanvasTest, CanvasCreation)
{
	Canvas c = Canvas(10, 20);

	EXPECT_EQ(c.width, 10);
	EXPECT_EQ(c.height, 20);

	int rows = 0;
	for (const auto& row : c.pixels)
	{
		int columns = 0;
		for (const auto& pixel : row)
		{
			EXPECT_EQ(pixel, Color(0.0, 0.0, 0.0));
			columns++;
		}
		EXPECT_EQ(columns, 10);
		rows++;
	}
	EXPECT_EQ(rows, 20);
}

TEST(CanvasTest, WriteToCanvas)
{
	Canvas c = Canvas(10, 20);
	Color red = Color(1.0, 0.0, 0.0);

	c.pixels[3][2] = red;
	EXPECT_EQ(c.pixels[3][2], red);
}

TEST(CanvasTest, PPMHeader)
{
	Canvas c = Canvas(5, 3);
	std::string ppmString = c.GetPPMString();
	//std::string_view sv("p3");
	EXPECT_TRUE(ppmString.starts_with("P3\n5 3\n255\n"));
}

TEST(CanvasTest, PixelData)
{
	Canvas c = Canvas(5, 3);
	Color c1 = Color(1.5, 0, 0);
	Color c2 = Color(0, 0.5, 0);
	Color c3 = Color(-0.5, 0, 1);

	c.pixels[0][0] = c1;
	c.pixels[1][2] = c2;
	c.pixels[2][4] = c3;

	std::string ppmString = c.GetPPMString();
	EXPECT_TRUE(ppmString.starts_with("P3\n5 3\n255\n255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n"));
}

TEST(CanvasTest, MaximumLineLength)
{
	Canvas c = Canvas(10, 2);

	for (auto& row : c.pixels)
	{
		for (auto& pixel : row)
		{
			pixel = Color(1, 0.8, 0.6);
		}
	}

	std::string ppmString = c.GetPPMString();
	std::string compString =
			"P3\n"
			"10 2\n"
			"255\n"
			"255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
			"153 255 204 153 255 204 153 255 204 153 255 204 153\n"
			"255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
			"153 255 204 153 255 204 153 255 204 153 255 204 153\n";
	EXPECT_TRUE(ppmString.starts_with(compString));
}

TEST(CanvasTest, EndsWithNewline)
{
	Canvas c = Canvas(5, 3);
	std::string ppmString = c.GetPPMString();
	EXPECT_TRUE(ppmString.ends_with("\n"));
}







