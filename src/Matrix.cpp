/*
 * Matrix.cpp
 *
 *  Created on: Apr 2, 2021
 *      Author: nic
 */

#include "Matrix.hpp"

#include <iostream>
/*
template<int N>
std::array<float, N>& Matrix<N>::operator[](const unsigned int index)
{
	return data[index];
}

template<int N>
const std::array<float, N>& Matrix<N>::operator[](const unsigned int index) const
{
	return data[index];
}

template<int N>
bool Matrix<N>::operator==(const Matrix<N>& other) const
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (std::abs(data[i][j] - other.data[i][j]) > MATRIX_EPSILON)
			{
				return false;
			}
		}
	}
	return true;
}

template<int N>
bool Matrix<N>::operator!=(const Matrix<N>& other) const // The inequality operator tests passed without this... what?!
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (std::abs(data[i][j] - other.data[i][j]) > MATRIX_EPSILON)
			{
				return true;
			}
		}
	}
	return false;
}

template<int N>
Matrix<N> Matrix<N>::operator*(const Matrix<N>& other) const
{
	Matrix<N> product;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			float partialProduct = 0;
			for (int k = 0; k < N; k++)
			{
				partialProduct += data[i][k] * other.data[k][j];
			}
			product[i][j] = partialProduct;
		}
	}
	return product;
}

template<>
Tuple Matrix<4>::operator*(const Tuple& other) const // This is only valid for Matrix<4> until Tuple is overhauled
{
	return Tuple(
			data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z + data[0][3] * other.w,
			data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z + data[1][3] * other.w,
			data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z + data[2][3] * other.w,
			data[3][0] * other.x + data[3][1] * other.y + data[3][2] * other.z + data[3][3] * other.w
	);
}

template<int N>
Matrix<N> Matrix<N>::transpose() const
{
	Matrix<N> t;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			t.data[i][j] = data[j][i];
		}
	}

	return t;
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

template<int N>
Matrix<N-1> Matrix<N>::submatrix(int row, int col) const
{
	Matrix<N-1> s;
	int rowOffset = 0;
	for (int i = 0; i < N; i++)
	{
		int colOffset = 0;
		if (i == row)
		{
			rowOffset++;
		} else
		{
			for (int j = 0; j < N; j++)
			{
				if (j == col)
				{
					colOffset++;
				} else
				{
					s[i - rowOffset][j - colOffset] = data[i][j];
				}

			}
		}

	}
	return s;
}

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

*/

template<>
Tuple Matrix<4>::operator*(const Tuple& other) const // This is only valid for Matrix<4> until Tuple is overhauled
{
	return Tuple(
			data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z + data[0][3] * other.w,
			data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z + data[1][3] * other.w,
			data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z + data[2][3] * other.w,
			data[3][0] * other.x + data[3][1] * other.y + data[3][2] * other.z + data[3][3] * other.w
	);
}

template <>
float Matrix<2>::determinant() const
{
	return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}

template<>
float Matrix<3>::minor(uint32_t row, uint32_t col) const
{
	return this->submatrix(row, col).determinant();
}

template<>
float Matrix<4>::minor(uint32_t row, uint32_t col) const
{
	return this->submatrix(row, col).determinant();
}

template<>
float Matrix<3>::cofactor(uint32_t row, uint32_t col) const
{
	return (row + col) % 2 > 0 ? -1 * this->minor(row, col) : this->minor(row, col);
}

template<>
float Matrix<4>::cofactor(uint32_t row, uint32_t col) const
{
	return (row + col) % 2 > 0 ? -1 * this->minor(row, col) : this->minor(row, col);
}

Matrix<4> IdentityMatrix()
{
	return Matrix<4>({{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}});
}
