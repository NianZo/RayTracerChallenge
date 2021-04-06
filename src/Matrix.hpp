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
			for (auto& e : row)
			{
				e = 0;
			}
		}
	}
	Matrix(std::array<std::array<float, N>, N> initialData) : data(initialData) {};
	//std::array<float, N>& operator[](int);
	std::array<float, N>& operator[](const unsigned int index) // TODO why does this HAVE to be defined here?
	{
		if (index >= N)
		{
			throw std::runtime_error("Array index out of bounds");
			exit(0);
		}
		return data[index];
	}

	const std::array<float, N>& operator[](const unsigned int index) const
	{
		if (index >= N)
		{
			throw std::runtime_error("Array index out of bounds");
			exit(0);
		}
		return data[index];
	}

	bool operator==(const Matrix<N>& other) const
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

	bool operator!=(const Matrix<N>& other) const // The inequality operator tests passed without this... what?!
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

	Matrix<N> operator*(const Matrix<N>& other) const
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

	Tuple operator*(const Tuple& other) const
	{
		return Tuple(
				data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z + data[0][3] * other.w,
				data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z + data[1][3] * other.w,
				data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z + data[2][3] * other.w,
				data[3][0] * other.x + data[3][1] * other.y + data[3][2] * other.z + data[3][3] * other.w
		);
	}

	Matrix<N> transpose() const
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

	float determinant() const;



	Matrix<N-1> submatrix(int row, int col) const
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

	float minor(int row, int col) const;
	float cofactor(int row, int col) const;
	bool invertible() const;
	Matrix<N> inverse() const;

private:
	std::array<std::array<float, N>, N> data;
};

Matrix<4> IdentityMatrix();

#endif /* SRC_MATRIX_HPP_ */
