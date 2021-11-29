/*
 * Transformation.cpp
 *
 *  Created on: Apr 10, 2021
 *      Author: nic
 */

#include "Transformation.hpp"

#include <cmath>

const Matrix<4> translation(const float x, const float y, const float z) noexcept
{
    return Matrix<4>({{{1, 0, 0, x},
                       {0, 1, 0, y},
                       {0, 0, 1, z},
                       {0, 0, 0, 1}}});
}

const Matrix<4> scaling(const float x, const float y, const float z) noexcept
{
    return Matrix<4>({{{x, 0, 0, 0},
                       {0, y, 0, 0},
                       {0, 0, z, 0},
                       {0, 0, 0, 1}}});
}

const Matrix<4> rotationX(const float r) noexcept
{
    return Matrix<4>({{{1, 0, 0, 0},
                       {0, cosf(r), -sinf(r), 0},
                       {0, sinf(r), cosf(r), 0},
                       {0, 0, 0, 1}}});
}

const Matrix<4> rotationY(const float r) noexcept
{
    return Matrix<4>({{{cosf(r), 0, sinf(r), 0},
                       {0, 1, 0.f, 0},
                       {-sinf(r), 0, cosf(r), 0},
                       {0, 0, 0, 1}}});
}

const Matrix<4> rotationZ(const float r) noexcept
{
    return Matrix<4>({{{cosf(r), -sinf(r), 0, 0},
                       {sinf(r), cosf(r), 0, 0},
                       {0, 0, 1, 0},
                       {0, 0, 0, 1}}});
}

const Matrix<4> shearing(const float xSuby, const float xSubz, const float ySubx, const float ySubz, const float zSubx, const float zSuby) noexcept
{
    return Matrix<4>({{{1, xSuby, xSubz, 0},
                       {ySubx, 1, ySubz, 0},
                       {zSubx, zSuby, 1, 0},
                       {0, 0, 0, 1}}});
}

const Matrix<4> ViewTransform() noexcept
{
    return IdentityMatrix();
}

const Matrix<4> ViewTransform(const Tuple& from, const Tuple& to, const Tuple& up) noexcept
{
    const Tuple forward = (to - from).normalize();
    const Tuple left = forward.cross(up.normalize());
    const Tuple trueUp = left.cross(forward);
    const Matrix<4> orientation({{{left.x, left.y, left.z, 0},
                                  {trueUp.x, trueUp.y, trueUp.z, 0},
                                  {-forward.x, -forward.y, -forward.z, 0},
                                  {0, 0, 0, 1}}});
    return orientation * translation(-from.x, -from.y, -from.z);
}
