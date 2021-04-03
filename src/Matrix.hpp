/*
 * Matrix.hpp
 *
 *  Created on: Apr 2, 2021
 *      Author: nic
 */

#ifndef SRC_MATRIX_HPP_
#define SRC_MATRIX_HPP_

#include <array>
#include <iostream>

template<int N>
class Matrix
{
public:
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

private:
	std::array<std::array<float, N>, N> data;
};


#endif /* SRC_MATRIX_HPP_ */
