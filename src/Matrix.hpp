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

#endif /* SRC_MATRIX_HPP_ */
