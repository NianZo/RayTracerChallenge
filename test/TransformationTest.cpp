/*
 * TransformationTest.cpp
 *
 *  Created on: Apr 10, 2021
 *      Author: nic
 */

#include "Tuple.hpp"
#include "Transformation.hpp"
#include "gtest/gtest.h"
#include <cmath>
#include <numbers>

TEST(TransformationTest, Translation)
{
	Tuple p = Point(-3, 4, 5);
	EXPECT_EQ(translation(5, -3, 2) * p, Point(2, 1, 7));
	EXPECT_EQ(translation(5, -3, 2).inverse() * p, Point(-8, 7, 3));

	Tuple v = Vector(-3, 4, 5);
	EXPECT_EQ(translation(5, -3, 2) * v, v);
}

TEST(TransformationTest, Scaling)
{
	Tuple p = Point(-4, 6, 8);
	EXPECT_EQ(scaling(2, 3, 4) * p, Point(-8, 18, 32));

	Tuple v = Vector(-4, 6, 8);
	EXPECT_EQ(scaling(2, 3, 4) * v, Vector(-8, 18, 32));
	EXPECT_EQ(scaling(2, 3, 4).inverse() * v, Vector(-2, 2, 2));

	Tuple p2 = Point(2, 3, 4);
	EXPECT_EQ(scaling(-1, 1, 1) * p2, Point(-2, 3, 4));
}

TEST(TransformationTest, Rotation)
{
	// Rotate around x-axis
	Tuple p = Point(0, 1, 0);
	EXPECT_EQ(rotationX(std::numbers::pi / 4) * p, Point(0, sqrt(2) / 2, sqrt(2) / 2));
	EXPECT_EQ(rotationX(std::numbers::pi / 2) * p, Point(0, 0, 1));

	// Inverse of x-rotation rotates in opposite direction
	EXPECT_EQ(rotationX(std::numbers::pi / 4).inverse() * p, Point(0, sqrt(2) / 2, -sqrt(2) / 2));

	// Rotate around y-axis
	Tuple p2 = Point(0, 0, 1);
	EXPECT_EQ(rotationY(std::numbers::pi / 4) * p2, Point(sqrt(2) / 2, 0, sqrt(2) / 2));
	EXPECT_EQ(rotationY(std::numbers::pi / 2) * p2, Point(1, 0, 0));

	// Rotate around z-axis
	Tuple p3 = Point(0, 1, 0);
	EXPECT_EQ(rotationZ(std::numbers::pi / 4) * p3, Point(-sqrt(2) / 2, sqrt(2) / 2, 0));
	EXPECT_EQ(rotationZ(std::numbers::pi / 2) * p3, Point(-1, 0, 0));
}

TEST(TransformationTest, Shear)
{
	Tuple p = Point(2, 3, 4);

	EXPECT_EQ(shearing(1, 0, 0, 0, 0, 0) * p, Point(5, 3, 4));
	EXPECT_EQ(shearing(0, 1, 0, 0, 0, 0) * p, Point(6, 3, 4));
	EXPECT_EQ(shearing(0, 0, 1, 0, 0, 0) * p, Point(2, 5, 4));
	EXPECT_EQ(shearing(0, 0, 0, 1, 0, 0) * p, Point(2, 7, 4));
	EXPECT_EQ(shearing(0, 0, 0, 0, 1, 0) * p, Point(2, 3, 6));
	EXPECT_EQ(shearing(0, 0, 0, 0, 0, 1) * p, Point(2, 3, 7));
}

TEST(TransformationTest, ChainedTransforms)
{
	Tuple p = Point(1, 0, 1);
	Matrix<4> A = rotationX(std::numbers::pi / 2);
	Matrix<4> B = scaling(5, 5, 5);
	Matrix<4> C = translation(10, 5, 7);

	Tuple p2 = A * p;
	EXPECT_EQ(p2, Point(1, -1, 0));

	Tuple p3 = B * p2;
	EXPECT_EQ(p3, Point(5, -5, 0));

	Tuple p4 = C * p3;
	EXPECT_EQ(p4, Point(15, 0, 7));

	Matrix<4> T = C * B * A;
	EXPECT_EQ(T * p, Point(15, 0, 7));
}

TEST(TransformationTest, DefaultViewTransform)
{
	Tuple from = Point(0, 0, 0);
	Tuple to = Point(0, 0, -1);
	Tuple up = Vector(0, 1, 0);
	Matrix<4> viewTransform = ViewTransform(from, to, up);

	EXPECT_EQ(viewTransform, IdentityMatrix());

	Matrix<4> defaultViewTransform = ViewTransform();

	EXPECT_EQ(defaultViewTransform, IdentityMatrix());
}

TEST(TransformationTest, ViewTransformLookingAtPositiveZ)
{
	Tuple from = Point(0, 0, 0);
	Tuple to = Point(0, 0, 1);
	Tuple up = Vector(0, 1, 0);
	Matrix<4> viewTransform = ViewTransform(from, to, up);

	EXPECT_EQ(viewTransform, scaling(-1, 1, -1));
}

TEST(TransformationTest, ViewTransformMovesWorld)
{
	Tuple from = Point(0, 0, 8);
	Tuple to = Point(0, 0, 0);
	Tuple up = Vector(0, 1, 0);
	Matrix<4> viewTransform = ViewTransform(from, to, up);

	EXPECT_EQ(viewTransform, translation(0, 0, -8));
}

TEST(TransformationTest, ArbitraryViewTransform)
{
	Tuple from = Point(1, 3, 2);
	Tuple to = Point(4, -2, 8);
	Tuple up = Vector(1, 1, 0);
	Matrix<4> viewTransform = ViewTransform(from, to, up);

	Matrix<4> testMatrix({{
		{-0.50709, 0.50709, 0.67612, -2.36643},
		{0.76772, 0.60609, 0.12122, -2.82843},
		{-0.35857, 0.59761, -0.71714, 0.0},
		{0.0, 0.0, 0.0, 1.0}}});
	EXPECT_EQ(viewTransform, testMatrix);
}





