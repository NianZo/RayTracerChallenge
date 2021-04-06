/*
 * Matrix.cpp
 *
 *  Created on: Apr 2, 2021
 *      Author: nic
 */

#include "Matrix.hpp"

#include <iostream>

//template<int N>
//std::array<float, 4>& Matrix<N>::operator[](int index)
//{
//	if (index >= N)
//	{
//		throw std::runtime_error("Array index out of bounds");
//		exit(0);
//	}
//	return testData[index];
//}

//template<int N>
//float& Matrix<N>::operator[](int index)
//{
//	if (index >= N)
//	{
//		exit(0);
//	}
//	return testData[index];
//}
//template<int N>
//bool Matrix<N>::operator==(const Matrix<N>& other) const
//{
//	return data == other.data;
//}

Matrix<4> IdentityMatrix()
{
	return Matrix<4>({{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}});
}

template<int N>
float Matrix<N>::determinant() const
{
	float d = 0;
	for (int i = 0; i < N; i++)
	{
		d += data[0][i] * this->cofactor(0, i);
	}
	return d;
}

template <>
float Matrix<2>::determinant() const
{
	return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}

//template<>
//float Matrix<3>::determinant() const
//{
//	float d = 0;
//	for (int i = 0; i < 3; i++)
//	{
//		d += this->cofactor(0, i);
//	}
//	return d;
//}
//
//template<>
//float Matrix<4>::determinant() const
//{
//	float d = 0;
//	for (int i = 0; i < 4; i++)
//	{
//		d += this->cofactor(0, i);
//	}
//	return d;
//}

template<>
float Matrix<3>::minor(int row, int col) const
{
	return this->submatrix(row, col).determinant();
}

template<>
float Matrix<4>::minor(int row, int col) const
{
	return this->submatrix(row, col).determinant();
}

template<>
float Matrix<3>::cofactor(int row, int col) const
{
	return (row + col) % 2 > 0 ? -1 * this->minor(row, col) : this->minor(row, col);
}

template<>
float Matrix<4>::cofactor(int row, int col) const
{
	return (row + col) % 2 > 0 ? -1 * this->minor(row, col) : this->minor(row, col);
}

template<int N>
bool Matrix<N>::invertible() const
{
	return this->determinant() != 0;
}
//template<int N>
//Matrix<N-1> Matrix<N>::submatrix(int row, int col) const
//{
//	Matrix<N-1> s;
//	int rowOffset = 0;
//	for (int i = 0; i < N; i++)
//	{
//		int colOffset = 0;
//		if (i == row)
//		{
//			rowOffset++;
//			continue;
//		}
//			for (int j = 0; j < N; j++)
//			{
//				if (j == col)
//				{
//					colOffset++;
//					continue;
//				}
//				s[i][j] = data[i + rowOffset][j + colOffset];
//			}
//
//
//	}
//}
template<int N>
Matrix<N> Matrix<N>::inverse() const
{
	Matrix<N> I;

	float d = determinant();
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			I[j][i] = cofactor(i, j) / d;
		}
	}

	return I;
}
