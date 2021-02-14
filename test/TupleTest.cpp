/*
 * TupleTest.cpp
 *
 *  Created on: Jan 9, 2021
 *      Author: nic
 */

#include "../src/Tuple.hpp"

#include "gtest/gtest.h"

#include <cmath>

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

TEST(TupleTest, CreatePointFromTuple)
{
	Tuple tuple(4.3f, -4.2f, 3.1f, 1.0f);

	EXPECT_FLOAT_EQ(4.3f, tuple.x);
	EXPECT_FLOAT_EQ(-4.2f, tuple.y);
	EXPECT_FLOAT_EQ(3.1f, tuple.z);
	EXPECT_FLOAT_EQ(1.0f, tuple.w);
	EXPECT_TRUE(tuple.IsPoint());
	EXPECT_FALSE(tuple.IsVector());
}

TEST(TupleTest, CreateVectorFromTuple)
{
	Tuple tuple(4.3f, -4.2f, 3.1f, 0.0f);

	EXPECT_FLOAT_EQ(4.3f, tuple.x);
	EXPECT_FLOAT_EQ(-4.2f, tuple.y);
	EXPECT_FLOAT_EQ(3.1f, tuple.z);
	EXPECT_FLOAT_EQ(0.0f, tuple.w);
	EXPECT_FALSE(tuple.IsPoint());
	EXPECT_TRUE(tuple.IsVector());
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

TEST(TupleTest, Equality)
{
	Tuple a = Tuple(8.7, 3.0, 3.7, 8.4);
	Tuple b = Tuple(8.7, 3.0, 3.7, 8.4);

	EXPECT_TRUE(a == b);
	EXPECT_FALSE(a == Tuple(8.8, 3.0, 3.7, 8.4));
	EXPECT_FALSE(a == Tuple(8.7, 2.9, 3.7, 8.4));
	EXPECT_FALSE(a == Tuple(8.7, 3.0, 3.8, 8.4));
	EXPECT_FALSE(a == Tuple(8.7, 3.0, 3.7, 8.5));
}

TEST(TupleTest, Inequality)
{
	Tuple a = Tuple(8.7, 3.0, 3.7, 8.4);
	Tuple b = Tuple(8.7, 3.0, 3.7, 8.4);

	EXPECT_FALSE(a != b);
	EXPECT_TRUE(a != Tuple(8.8, 3.0, 3.7, 8.4));
	EXPECT_TRUE(a != Tuple(8.7, 2.9, 3.7, 8.4));
	EXPECT_TRUE(a != Tuple(8.7, 3.0, 3.8, 8.4));
	EXPECT_TRUE(a != Tuple(8.7, 3.0, 3.7, 8.5));
}

TEST(TupleTest, Addition)
{
	Tuple a = Tuple(-5.3, 32.7, -0.0, 8.4);
	Tuple b = Tuple(9.5, -10.3, 52.3, 6.9);

	EXPECT_EQ(a + b, Tuple(4.2, 22.4, 52.3, 15.3));
}

TEST(TupleTest, Subtraction)
{
	Tuple a = Tuple(-5.3, 32.7, -0.0, 8.4);
	Tuple b = Tuple(9.5, -10.3, 52.3, 6.9);

	EXPECT_EQ(a - b, Tuple(-14.8, 43.0, -52.3, 1.5));
}

TEST(TupleTest, Negation)
{
	Tuple a = Tuple(9.5, -10.3, 52.3, 6.9);

	EXPECT_EQ(-a, Tuple(-9.5, 10.3, -52.3, -6.9));
}

TEST(TupleTest, ScalarMultiplication)
{
	Tuple a = Tuple(1.0, -2.0, 3.0, -4.0);

	EXPECT_EQ(a * 3.5, Tuple(3.5, -7.0, 10.5, -14.0));
}

TEST(TupleTest, ScalarDivision)
{
	Tuple a = Tuple(1.0, -2.0, 3.0, -4.0);

	EXPECT_EQ(a / 2, Tuple(0.5, -1.0, 1.5, -2.0));
}

TEST(TupleTest, Magnitude)
{
	Tuple a = Vector(1.0, 0.0, 0.0);
	EXPECT_FLOAT_EQ(a.magnitude(), 1.0);

	Tuple b = Vector(0.0, 1.0, 0.0);
	EXPECT_FLOAT_EQ(b.magnitude(), 1.0);

	Tuple c = Vector(0.0, 0.0, 1.0);
	EXPECT_FLOAT_EQ(c.magnitude(), 1.0);

	Tuple d = Vector(1.0, 2.0, 3.0);
	EXPECT_FLOAT_EQ(d.magnitude(), std::sqrt(14.0));

	Tuple e = Vector(-1.0, -2.0, -3.0);
	EXPECT_FLOAT_EQ(e.magnitude(), std::sqrt(14.0));
}

TEST(TupleTest, Normalize)
{
	Tuple a = Vector(4.0, 0.0, 0.0);
	EXPECT_EQ(a.normalize(), Vector(1.0, 0.0, 0.0));
	EXPECT_EQ(a, Vector(4.0, 0.0, 0.0));

	Tuple b = Vector(1.0, 2.0, 3.0);
	EXPECT_EQ(b.normalize(), Vector(1/std::sqrt(14.0), 2/std::sqrt(14.0), 3/std::sqrt(14.0)));
	EXPECT_EQ(b, Vector(1.0, 2.0, 3.0));

	Tuple c = b.normalize();
	EXPECT_FLOAT_EQ(c.magnitude(), 1.0);
}

TEST(TupleTest, DotProduct)
{
	Tuple a = Vector(1.0, 2.0, 3.0);
	Tuple b = Vector(2.0, 3.0, 4.0);

	EXPECT_FLOAT_EQ(a.dot(b), 20);
	EXPECT_FLOAT_EQ(b.dot(a), 20);
}

TEST(TupleTest, CrossProduct)
{
	Tuple a = Vector(1.0, 2.0, 3.0);
	Tuple b = Vector(2.0, 3.0, 4.0);

	EXPECT_EQ(a.cross(b), Vector(-1.0, 2.0, -1.0));
	EXPECT_EQ(b.cross(a), Vector(1.0, -2.0, 1.0));
}



