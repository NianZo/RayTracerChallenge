/*
 * MatrixTest.cpp
 *
 *  Created on: Apr 1, 2021
 *      Author: nic
 */

#include "Matrix.hpp"
#include "Tuple.hpp"
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

TEST(MatrixTest, MatrixEquality)
{
	Matrix<4> m4({{
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 8, 7, 6},
		{5, 4, 3, 2}
	}});

	EXPECT_TRUE(m4 == Matrix<4>({{
		{1.000001, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 8, 7, 6},
		{5, 4, 3, 2}
	}}));

	EXPECT_FALSE(m4 == Matrix<4>({{
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 8, 7, 6},
		{5, 4, 3, 1}
	}}));

	Matrix<3> m3({{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
	}});

	EXPECT_TRUE(m3 == Matrix<3>({{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
	}}));

	EXPECT_FALSE(m3 == Matrix<3>({{
			{1, 2, 3},
			{4, 6, 6},
			{7, 8, 9}
	}}));

	Matrix<2> m2({{
		{1, 2},
		{-1, -2}
	}});

	EXPECT_TRUE(m2 == Matrix<2>({{
		{1, 2},
		{-1, -2}
	}}));

	EXPECT_FALSE(m2 == Matrix<2>({{
		{2, 2},
		{-1, -3}
	}}));
}

TEST(MatrixTest, MatrixInequality)
{
	Matrix<4> m4({{
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 8, 7, 6},
		{5, 4, 3, 2}
	}});

	EXPECT_TRUE(m4 != Matrix<4>({{
		{1, 2, 3, 4},
		{5, 6, 8, 8},
		{9, 8, 7, 6},
		{5, 4, 3, 2}
	}}));

	EXPECT_FALSE(m4 != Matrix<4>({{
		{1.000001, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 8, 7, 6},
		{5, 4, 3, 2}
	}}));

	Matrix<3> m3({{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
	}});

	EXPECT_FALSE(m3 != Matrix<3>({{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
	}}));

	EXPECT_TRUE(m3 != Matrix<3>({{
			{1, 2, 3},
			{4, 6, 6},
			{7, 8, 9}
	}}));

	Matrix<2> m2({{
		{1, 2},
		{-1, -2}
	}});

	EXPECT_FALSE(m2 != Matrix<2>({{
		{1, 2},
		{-1, -2}
	}}));

	EXPECT_TRUE(m2 != Matrix<2>({{
		{2, 2},
		{-1, -3}
	}}));
}

TEST(MatrixTest, MatrixMultiplication)
{
	Matrix<4> m1({{
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 8, 7, 6},
		{5, 4, 3, 2}
	}});

	Matrix<4> m2({{
		{-2, 1, 2, 3},
		{3, 2, 1, -1},
		{4, 3, 6, 5},
		{1, 2, 7, 8}
	}});

	EXPECT_EQ(m1 * m2, Matrix<4>({{
		{20, 22, 50, 48},
		{44, 54, 114, 108},
		{40, 58, 110, 102},
		{16, 26, 46, 42}
	}}));
}

TEST(MatrixTest, MatrixMultiplicationWithTuple)
{
	Matrix<4> m({{
		{1, 2, 3, 4},
		{2, 4, 4, 2},
		{8, 6, 4, 1},
		{0, 0, 0, 1}
	}});

	Tuple t(1, 2, 3, 1);

	EXPECT_EQ(m * t, Tuple(18, 24, 33, 1));
}

TEST(MatrixTest, IdentityMatrix)
{
	Matrix<4> i = IdentityMatrix();
	Matrix<4> m({{
		{0, 1, 2, 4},
		{1, 2, 4, 8},
		{2, 4, 8, 16},
		{4, 8, 16, 32}
	}});

	EXPECT_EQ(i, Matrix<4>({{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}}));

	EXPECT_EQ(m * i, m);
}

TEST(MatrixTest, MatrixTranspose)
{
	Matrix<4> A({{
		{0, 9, 3, 0},
		{9, 8, 0, 8},
		{1, 8, 5, 3},
		{0, 0, 5, 8}
	}});

	EXPECT_EQ(A.transpose(), Matrix<4>({{
		{0, 9, 1, 0},
		{9, 8, 8, 0},
		{3, 0, 5, 5},
		{0, 8, 3, 8}
	}}));

	Matrix<3> B({{
		{0, 9, 3},
		{9, 8, 0},
		{1, 8, 5}
	}});

	EXPECT_EQ(B.transpose(), Matrix<3>({{
		{0, 9, 1},
		{9, 8, 8},
		{3, 0, 5}
	}}));

	Matrix<2> C({{
		{1, 2},
		{3, 4}
	}});

	EXPECT_EQ(C.transpose(), Matrix<2>({{
		{1, 3},
		{2, 4}
	}}));

	Matrix<4> I = IdentityMatrix();

	EXPECT_EQ(I.transpose(), I);
}

TEST(MatrixTest, MatrixDeterminant)
{
	Matrix<2> m2({{
		{1, 5},
		{-3, 2}
	}});

	EXPECT_EQ(m2.determinant(), 17);

	Matrix<3> m3({{
		{1, 2, 6},
		{-5, 8, -4},
		{2, 6, 4}
	}});

	EXPECT_EQ(m3.cofactor(0, 0), 56);
	EXPECT_EQ(m3.cofactor(0, 1), 12);
	EXPECT_EQ(m3.cofactor(0, 2), -46);
	EXPECT_EQ(m3.determinant(), -196);

	Matrix<4> m4({{
		{-2, -8, 3, 5},
		{-3, 1, 7, 3},
		{1, 2, -9, 6},
		{-6, 7, 7, -9}
	}});

	EXPECT_EQ(m4.cofactor(0, 0), 690);
	EXPECT_EQ(m4.cofactor(0, 1), 447);
	EXPECT_EQ(m4.cofactor(0, 2), 210);
	EXPECT_EQ(m4.cofactor(0, 3), 51);
	EXPECT_EQ(m4.determinant(), -4071);
}

TEST(MatrixTest, Submatrix)
{
	Matrix<3> A({{
		{1, 5, 0},
		{-3, 2, 7},
		{0, 6, -3}
	}});

	EXPECT_EQ(A.submatrix(0, 2), Matrix<2>({{
		{-3, 2},
		{0, 6}
	}}));

	Matrix<4> B({{
		{-6, 1, 1, 6},
		{-8, 5, 8, 6},
		{-1, 0, 8, 2},
		{-7, 1, -1, 1}
	}});

	EXPECT_EQ(B.submatrix(2, 1), Matrix<3>({{
		{-6, 1, 6},
		{-8, 8, 6},
		{-7, -1, 1}
	}}));
}

TEST(MatrixTest, MatrixMinor)
{
	Matrix<3> A({{
		{3, 5, 0},
		{2, -1, -7},
		{6, -1, 5}
	}});

	EXPECT_EQ(A.submatrix(1, 0).determinant(), 25);
	EXPECT_EQ(A.minor(1, 0), 25);
}

TEST(MatrixTest, MatrixCofactor)
{
	Matrix<3> A({{
		{3, 5, 0},
		{2, -1, -7},
		{6, -1, 5}
	}});

	EXPECT_EQ(A.minor(0, 0), -12);
	EXPECT_EQ(A.cofactor(0, 0), -12);
	EXPECT_EQ(A.minor(1, 0), 25);
	EXPECT_EQ(A.cofactor(1, 0), -25);
}

TEST(MatrixTest, MatrixInvertibility)
{
	Matrix<4> A({{
		{6, 4, 4, 4},
		{5, 5, 7, 6},
		{4, -9, 3, -7},
		{9, 1, 7, -6}
	}});

	EXPECT_EQ(A.determinant(), -2120);
	EXPECT_TRUE(A.invertible());

	Matrix<4> B({{
		{-4, 2, -2, -3},
		{9, 6, 2, 6},
		{0, -5, 1, -5},
		{0, 0, 0, 0}
	}});

	EXPECT_EQ(B.determinant(), 0);
	EXPECT_FALSE(B.invertible());
}

TEST(MatrixTest, MatrixInversion)
{
	Matrix<4> A({{
		{-5, 2, 6, -8},
		{1, -5, 1, 8},
		{7, 7, -6, -7},
		{1, -3, 7, 4}
	}});

	Matrix<4> B = A.inverse();

	EXPECT_EQ(A.determinant(), 532);
	EXPECT_EQ(A.cofactor(2, 3), -160);
	EXPECT_FLOAT_EQ(B[3][2], -160./532);
	EXPECT_EQ(A.cofactor(3, 2), 105);
	EXPECT_FLOAT_EQ(B[2][3], 105./532);
	EXPECT_EQ(B, Matrix<4>({{
		{0.21805, 0.45113, 0.24060, -0.04511},
		{-0.80827, -1.45677, -0.44361, 0.52068},
		{-0.07895, -0.22368, -0.05263, 0.19737},
		{-0.52256, -0.81391, -0.30075, 0.30639}
	}}));

	Matrix<4> C({{
		{8, -5, 9, 2},
		{7, 5, 6, 1},
		{-6, 0, 9, 6},
		{-3, 0, -9, -4}
	}});

	EXPECT_EQ(C.inverse(), Matrix<4>({{
		{-0.15385, -0.15385, -0.28205, -0.53846},
		{-0.07692, 0.12308, 0.02564, 0.03077},
		{0.35897, 0.35897, 0.43590, 0.92308},
		{-0.69231, -0.69231, -0.76923, -1.92308}
	}}));

	Matrix<4> D({{
		{9, 3, 0, 9},
		{-5, -2, -6, -3},
		{-4, 9, 6, 4},
		{-7, 6, 6, 2}
	}});

	EXPECT_EQ(D.inverse(), Matrix<4>({{
		{-0.04074, -0.07778, 0.14444, -0.22222},
		{-0.07778, 0.03333, 0.36667, -0.33333},
		{-0.02901, -0.14630, -0.10926, 0.12963},
		{0.17778, 0.06667, -0.26667, 0.33333}
	}}));

	Matrix<4> E({{
		{3, -9, 7, 3},
		{3, -8, 2, -9},
		{-4, 4, 4, 1},
		{-6, 5, -1, 1}
	}});

	Matrix<4> F({{
		{8, 2, 2, 2},
		{3, -1, 7, 0},
		{7, 0, 5, 4},
		{6, -2, 0, 5}
	}});

	Matrix<4> G = E * F;

	EXPECT_EQ(G * F.inverse(), E);
}

