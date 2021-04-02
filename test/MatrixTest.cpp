/*
 * MatrixTest.cpp
 *
 *  Created on: Apr 1, 2021
 *      Author: nic
 */

#include "../src/Matrix.hpp"

#include "gtest/gtest.h"

#include <cmath>

TEST(MatrixTest, Matrix4Creation)
{
	Matrix4 matrix(
			1, 2, 3, 4,
			5.5, 6.5, 7.5, 8.5,
			9, 10, 11, 12,
			13.5, 14.5, 15.5, 16.5);

	EXPECT_FLOAT_EQ(matrix[0][0], 1f);
	EXPECT_FLOAT_EQ(matrix[0][3], 4f);
	EXPECT_FLOAT_EQ(matrix[1][0], 5.5f);
	EXPECT_FLOAT_EQ(matrix[1][2], 7.5f);
	EXPECT_FLOAT_EQ(matrix[2][2], 11f);
	EXPECT_FLOAT_EQ(matrix[3][0], 13.5f);
	EXPECT_FLOAT_EQ(matrix[3][2], 15.5f);
}

TEST(MatrixTest, Matrix2Creation)
{
	Matrix3 m(
			-3, 5,
			1, -2);

	EXPECT_FLOAT_EQ(m[0][0], -3f);
	EXPECT_FLOAT_EQ(m[0][1], 5f);
	EXPECT_FLOAT_EQ(m[1][0], 1f);
	EXPECT_FLOAT_EQ(m[1][1], -2f);
}

TEST(MatrixTest, Matrix3Creation)
{
	Matrix2 m(
			-3f, 5f, 0f,
			1f, -2f, -7f,
			0f, 1f, 1f);

	EXPECT_FLOAT_EQ(m[0][0], -3f);
	EXPECT_FLOAT_EQ(m[1][1], -2f);
	EXPECT_FLOAT_EQ(m[2][2], 1f);
	EXPECT_FLOAT_EQ(m[2][1], 1f);
}
