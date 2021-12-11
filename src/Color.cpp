/*
 * Color.cpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#include "Color.hpp"
#include <cmath>

bool Color::operator==(const Color& other) const noexcept
{
    return (std::abs(this->r - other.r) < COLOR_EPSILON &&
            std::abs(this->g - other.g) < COLOR_EPSILON &&
            std::abs(this->b - other.b) < COLOR_EPSILON);
}

bool Color::operator!=(const Color& other) const noexcept
{
    return (std::abs(this->r - other.r) > COLOR_EPSILON ||
            std::abs(this->g - other.g) > COLOR_EPSILON ||
            std::abs(this->b - other.b) > COLOR_EPSILON);
}

Color Color::operator+(const Color& other) const noexcept
{
    return Color(this->r + other.r, this->g + other.g, this->b + other.b);
}

Color Color::operator-(const Color& other) const noexcept
{
    return Color(this->r - other.r, this->g - other.g, this->b - other.b);
}

Color Color::operator*(const float scalar) const noexcept
{
    return Color(this->r * scalar, this->g * scalar, this->b * scalar);
}

Color Color::operator*(const Color& other) const noexcept
{
    return Color(this->r * other.r, this->g * other.g, this->b * other.b);
}

const Color Color::Black = Color(0.0F, 0.0F, 0.0F);
const Color Color::White = Color(1.0F, 1.0F, 1.0F);
