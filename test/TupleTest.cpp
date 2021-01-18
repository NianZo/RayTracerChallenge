/*
 * TupleTest.cpp
 *
 *  Created on: Jan 9, 2021
 *      Author: nic
 */

#include "../src/Tuple.hpp"

#include "gtest/gtest.h"


TEST(TupleTest, TupleCreation)
{
	Tuple tuple(4.3f, -4.2f, 3.1f, 0.5f);

	EXPECT_FLOAT_EQ(4.3f, tuple.x);
	EXPECT_FLOAT_EQ(-4.2f, tuple.y);
	EXPECT_FLOAT_EQ(3.1f, tuple.z);
	EXPECT_FLOAT_EQ(0.5f, tuple.w);
	EXPECT_FALSE(tuple.IsPoint());
	EXPECT_FALSE(tuple.IsVector());
}

TEST(TupleTest, PointCreation)
{
	Tuple tuple  = Point(4.3f, -4.2f, 3.1f);

	EXPECT_FLOAT_EQ(4.3f, tuple.x);
	EXPECT_FLOAT_EQ(-4.2f, tuple.y);
	EXPECT_FLOAT_EQ(3.1f, tuple.z);
	EXPECT_FLOAT_EQ(1.0f, tuple.w);
	EXPECT_TRUE(tuple.IsPoint());
	EXPECT_FALSE(tuple.IsVector());
}

TEST(TupleTest, VectorCreation)
{
	Tuple tuple  = Vector(4.3f, -4.2f, 3.1f);

	EXPECT_FLOAT_EQ(4.3f, tuple.x);
	EXPECT_FLOAT_EQ(-4.2f, tuple.y);
	EXPECT_FLOAT_EQ(3.1f, tuple.z);
	EXPECT_FLOAT_EQ(0.0f, tuple.w);
	EXPECT_FALSE(tuple.IsPoint());
	EXPECT_TRUE(tuple.IsVector());
}
