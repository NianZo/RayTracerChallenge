/*
 * CanvasTest.cpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#include "../src/Canvas.hpp"

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
	EXPECT_TRUE(ppmString.starts_with("Yolo"));
}
