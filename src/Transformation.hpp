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

const Matrix<4> translation(const float x, const float y, const float z);
const Matrix<4> scaling(const float x, const float y, const float z);
const Matrix<4> rotationX(const float r);
const Matrix<4> rotationY(const float r);
const Matrix<4> rotationZ(const float r);
const Matrix<4> shearing(const float xSuby, const float xSubz, const float ySubx, const float ySubz, const float zSubx, const float zSuby);
const Matrix<4> ViewTransform();
const Matrix<4> ViewTransform(const Tuple& from, const Tuple& to, const Tuple& up);

#endif /* SRC_TRANSFORMATION_HPP_ */
