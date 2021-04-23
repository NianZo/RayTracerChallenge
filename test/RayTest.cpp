/*
 * RayTest.cpp
 *
 *  Created on: Apr 22, 2021
 *      Author: nic
 */

#include "gtest/gtest.h"
#include "../src/Tuple.hpp"
#include "../src/Ray.hpp"


TEST(RayTest, RayCreation)
{
	Ray r(Point(1, 2, 3), Vector(4, 5, 6));
	EXPECT_EQ(r.origin, Point(1, 2, 3));
	EXPECT_EQ(r.direction, Vector(4, 5, 6));
}

TEST(RayTest, RayCast)
{
	Ray r(Point(2, 3, 4), Vector(1, 0, 0));
	EXPECT_EQ(r.cast(0), Point(2, 3, 4));
	EXPECT_EQ(r.cast(1), Point(3, 3, 4));
	EXPECT_EQ(r.cast(-1), Point(1, 3, 4));
	EXPECT_EQ(r.cast(2.5), Point(4.5, 3, 4));
}
