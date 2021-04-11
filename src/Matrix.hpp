/*
 * Matrix.hpp
 *
 *  Created on: Apr 2, 2021
 *      Author: nic
 */
#ifndef SRC_MATRIX_HPP_
#define SRC_MATRIX_HPP_

#include "Tuple.hpp"

#include <array>
#include <iostream>

#define MATRIX_EPSILON 0.00001f

template<int N>
class Matrix
{
public:
	Matrix()
	{
		for (auto& row : data)
		{
			row = {};
		}
	}
	Matrix(std::array<std::array<float, N>, N> initialData) : data(initialData) {};

	std::array<float, N>& operator[](const unsigned int index);
	const std::array<float, N>& operator[](const unsigned int index) const;
	bool operator==(const Matrix<N>& other) const;
	bool operator!=(const Matrix<N>& other) const; // The inequality operator tests passed without this... what?!
	Matrix<N> operator*(const Matrix<N>& other) const;
	Tuple operator*(const Tuple& other) const;

	Matrix<N> transpose() const;
	float determinant() const;
	Matrix<N-1> submatrix(int row, int col) const;
	float minor(int row, int col) const;
	float cofactor(int row, int col) const;
	bool invertible() const;
	Matrix<N> inverse() const;

private:
	std::array<std::array<float, N>, N> data;
};

Matrix<4> IdentityMatrix();


// Due to the nature of templates, non-specialized template members must
// be defined in the same header as the template declaration
// Template specializations seem to still need to be in a
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

template <>
float Matrix<2>::determinant() const;

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

//template float Matrix<4>::cofactor(int row, int col) const;

/*Matrix<4> IdentityMatrix()
{
	return Matrix<4>({{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}});
}*/

#endif /* SRC_MATRIX_HPP_ */
