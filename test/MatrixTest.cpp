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
	Matrix<4> matrix({{
			{1, 2, 3, 4},
			{5.5, 6.5, 7.5, 8.5},
			{9, 10, 11, 12},
			{13.5, 14.5, 15.5, 16.5}}});

	EXPECT_FLOAT_EQ(matrix[0][0], 1.0f);
	EXPECT_FLOAT_EQ(matrix[0][3], 4.0f);
	EXPECT_FLOAT_EQ(matrix[1][0], 5.5f);
	EXPECT_FLOAT_EQ(matrix[1][2], 7.5f);
	EXPECT_FLOAT_EQ(matrix[2][2], 11.0f);
	EXPECT_FLOAT_EQ(matrix[3][0], 13.5f);
	EXPECT_FLOAT_EQ(matrix[3][2], 15.5f);
}

TEST(MatrixTest, Matrix3Creation)
{
	Matrix<3> m({{
			{-3.0f, 5.0f, 0.0f},
			{1.0f, -2.0f, -7.0f},
			{0.0f, 1.0f, 1.0f}}});

	EXPECT_FLOAT_EQ(m[0][0], -3.0f);
	EXPECT_FLOAT_EQ(m[1][1], -2.0f);
	EXPECT_FLOAT_EQ(m[2][2], 1.0f);
	EXPECT_FLOAT_EQ(m[2][1], 1.0f);
}

TEST(MatrixTest, Matrix2Creation)
{
	Matrix<2> m({{
			{-0.0f, 0.0f},
			{0.0f, -0.0f}}});
	m[0][0] = -3.0f;
	m[0][1] = 5.0f;
	m[1][0] = 1.0f;
	m[1][1] = -2.0f;

	EXPECT_FLOAT_EQ(m[0][0], -3.0f);
	EXPECT_FLOAT_EQ(m[0][1], 5.0f);
	EXPECT_FLOAT_EQ(m[1][0], 1.0f);
	EXPECT_FLOAT_EQ(m[1][1], -2.0f);
}

TEST(MatrixTest, ConstMatrix2Creation)
{
	const Matrix<2> m({{
			{-3.0f, 5.0f},
			{1.0f, -2.0f}}});

	EXPECT_FLOAT_EQ(m[0][0], -3.0f);
	EXPECT_FLOAT_EQ(m[0][1], 5.0f);
	EXPECT_FLOAT_EQ(m[1][0], 1.0f);
	EXPECT_FLOAT_EQ(m[1][1], -2.0f);
}
