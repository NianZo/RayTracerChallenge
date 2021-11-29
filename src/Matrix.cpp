/*
 * Matrix.cpp
 *
 *  Created on: Apr 2, 2021
 *      Author: nic
 */

#include "Matrix.hpp"
#include <iostream>

template <>
Tuple Matrix<4>::operator*(const Tuple& other) const noexcept // This is only valid for Matrix<4> until Tuple is overhauled
{
    return Tuple(
        data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z + data[0][3] * other.w,
        data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z + data[1][3] * other.w,
        data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z + data[2][3] * other.w,
        data[3][0] * other.x + data[3][1] * other.y + data[3][2] * other.z + data[3][3] * other.w);
}

template <>
float Matrix<2>::determinant() const noexcept
{
    return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}

template <>
float Matrix<3>::minor(uint32_t row, uint32_t col) const noexcept
{
    return this->submatrix(row, col).determinant();
}

template <>
float Matrix<4>::minor(uint32_t row, uint32_t col) const noexcept
{
    return this->submatrix(row, col).determinant();
}

template <>
float Matrix<3>::cofactor(uint32_t row, uint32_t col) const noexcept
{
    return (row + col) % 2 > 0 ? -1 * this->minor(row, col) : this->minor(row, col);
}

template <>
float Matrix<4>::cofactor(uint32_t row, uint32_t col) const noexcept
{
    return (row + col) % 2 > 0 ? -1 * this->minor(row, col) : this->minor(row, col);
}

Matrix<4> IdentityMatrix() noexcept
{
    return Matrix<4>({{{1, 0, 0, 0},
                       {0, 1, 0, 0},
                       {0, 0, 1, 0},
                       {0, 0, 0, 1}}});
}
