/*
 * LightTest.cpp
 *
 *  Created on: Sep 18, 2021
 *      Author: nic
 */

#include "gtest/gtest.h"

#include "../src/Light.hpp"
#include "../src/Color.hpp"
#include "../src/Tuple.hpp"

TEST(LightTest, SetPositionAndIntensity)
{
	Light l(Point(1, 2, 3), Color(1, 0.5, 0.25));

	EXPECT_EQ(l.position, Point(1, 2, 3));
	EXPECT_EQ(l.intensity, Color(1, 0.5, 0.25));
}


