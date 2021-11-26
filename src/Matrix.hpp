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

template <uint32_t N>
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
    Matrix(std::array<std::array<float, N>, N> initialData) : data(initialData){};

    std::array<float, N>& operator[](const uint32_t index);
    const std::array<float, N>& operator[](const uint32_t index) const;
    [[nodiscard]] bool operator==(const Matrix<N>& other) const;
    [[nodiscard]] bool operator!=(const Matrix<N>& other) const; // The inequality operator tests passed without this... what?!
    [[nodiscard]] Matrix<N> operator*(const Matrix<N>& other) const;
    [[nodiscard]] Tuple operator*(const Tuple& other) const;

    [[nodiscard]] Matrix<N> transpose() const;
    [[nodiscard]] float determinant() const;
    [[nodiscard]] Matrix<N - 1> submatrix(uint32_t row, uint32_t col) const;
    [[nodiscard]] float minor(uint32_t row, uint32_t col) const;
    [[nodiscard]] float cofactor(uint32_t row, uint32_t col) const;
    [[nodiscard]] bool invertible() const;
    [[nodiscard]] Matrix<N> inverse() const;

  private:
    std::array<std::array<float, N>, N> data;
};

Matrix<4> IdentityMatrix();

// Due to the nature of templates, non-specialized template members must
// be defined in the same header as the template declaration
// Template specializations seem to still need to be in a
template <uint32_t N>
std::array<float, N>& Matrix<N>::operator[](const uint32_t index)
{
    return data[index];
}

template <uint32_t N>
const std::array<float, N>& Matrix<N>::operator[](const uint32_t index) const
{
    return data[index];
}

template <uint32_t N>
bool Matrix<N>::operator==(const Matrix<N>& other) const
{
    for (uint32_t i = 0; i < N; i++)
    {
        for (uint32_t j = 0; j < N; j++)
        {
            if (std::abs(data[i][j] - other.data[i][j]) > MATRIX_EPSILON)
            {
                return false;
            }
        }
    }
    return true;
}

template <uint32_t N>
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

template <uint32_t N>
Matrix<N> Matrix<N>::operator*(const Matrix<N>& other) const
{
    Matrix<N> product;
    for (uint32_t i = 0; i < N; i++)
    {
        for (uint32_t j = 0; j < N; j++)
        {
            float partialProduct = 0;
            for (uint32_t k = 0; k < N; k++)
            {
                partialProduct += data[i][k] * other.data[k][j];
            }
            product[i][j] = partialProduct;
        }
    }
    return product;
}

template <uint32_t N>
Matrix<N> Matrix<N>::transpose() const
{
    Matrix<N> t;

    for (uint32_t i = 0; i < N; i++)
    {
        for (uint32_t j = 0; j < N; j++)
        {
            t.data[i][j] = data[j][i];
        }
    }

    return t;
}

template <>
float Matrix<2>::determinant() const;

template <uint32_t N>
float Matrix<N>::determinant() const
{
    float d = 0;
    for (uint32_t i = 0; i < N; i++)
    {
        d += data[0][i] * this->cofactor(0, i);
    }
    return d;
}

template <uint32_t N>
Matrix<N - 1> Matrix<N>::submatrix(uint32_t row, uint32_t col) const
{
    Matrix<N - 1> s;
    uint32_t rowOffset = 0;
    for (uint32_t i = 0; i < N; i++)
    {
        uint32_t colOffset = 0;
        if (i == row)
        {
            rowOffset++;
        } else
        {
            for (uint32_t j = 0; j < N; j++)
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

template <uint32_t N>
bool Matrix<N>::invertible() const
{
    return this->determinant() != 0;
}

template <uint32_t N>
Matrix<N> Matrix<N>::inverse() const
{
    Matrix<N> I;

    float d = determinant();
    for (uint32_t i = 0; i < N; i++)
    {
        for (uint32_t j = 0; j < N; j++)
        {
            I[j][i] = cofactor(i, j) / d;
        }
    }

    return I;
}

#endif /* SRC_MATRIX_HPP_ */
