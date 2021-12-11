/*
 * Transformation.hpp
 *
 *  Created on: Apr 10, 2021
 *      Author: nic
 */

#ifndef SRC_TRANSFORMATION_HPP_
#define SRC_TRANSFORMATION_HPP_

#include "Matrix.hpp"
#include "Tuple.hpp"

Matrix<4> translation(float x, float y, float z) noexcept;
Matrix<4> scaling(float x, float y, float z) noexcept;
Matrix<4> rotationX(float r) noexcept;
Matrix<4> rotationY(float r) noexcept;
Matrix<4> rotationZ(float r) noexcept;
Matrix<4> shearing(float xSuby, float xSubz, float ySubx, float ySubz, float zSubx, float zSuby) noexcept;
Matrix<4> ViewTransform() noexcept;
Matrix<4> ViewTransform(const Tuple& from, const Tuple& to, const Tuple& up) noexcept;

#endif /* SRC_TRANSFORMATION_HPP_ */
