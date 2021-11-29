/*
 * Tuple.cpp
 *
 *  Created on: Jan 9, 2021
 *      Author: nic
 */

#include "Tuple.hpp"
#include <cmath>
#include <cstdlib>

Tuple::Tuple(float xIn, float yIn, float zIn, float wIn) noexcept
{
    x = xIn;
    y = yIn;
    z = zIn;
    w = wIn;
}

bool Tuple::operator==(const Tuple& other) const noexcept
{
    return (std::abs(this->x - other.x) < TUPLE_EPSILON &&
            std::abs(this->y - other.y) < TUPLE_EPSILON &&
            std::abs(this->z - other.z) < TUPLE_EPSILON &&
            std::abs(this->w - other.w) < TUPLE_EPSILON);
}

bool Tuple::operator!=(const Tuple& other) const noexcept
{
    return (std::abs(this->x - other.x) > TUPLE_EPSILON ||
            std::abs(this->y - other.y) > TUPLE_EPSILON ||
            std::abs(this->z - other.z) > TUPLE_EPSILON ||
            std::abs(this->w - other.w) > TUPLE_EPSILON);
}

Tuple Tuple::operator+(const Tuple& other) const noexcept
{
    return Tuple(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
}

Tuple Tuple::operator-(const Tuple& other) const noexcept
{
    return Tuple(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
}

Tuple Tuple::operator-() const noexcept
{
    return Tuple(-this->x, -this->y, -this->z, -this->w);
}

Tuple Tuple::operator*(const float s) const noexcept
{
    return Tuple(this->x * s, this->y * s, this->z * s, this->w * s);
}

Tuple Tuple::operator/(const float s) const noexcept
{
    return Tuple(this->x / s, this->y / s, this->z / s, this->w / s);
}

bool Tuple::IsPoint() const noexcept
{
    return std::abs(this->w - 1.0f) < TUPLE_EPSILON;
}

bool Tuple::IsVector() const noexcept
{
    return std::abs(this->w) < TUPLE_EPSILON;
}

float Tuple::magnitude() const noexcept
{
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
}

Tuple Tuple::normalize() const noexcept
{
    const float mag = this->magnitude();
    return Tuple(this->x / mag, this->y / mag, this->z / mag, this->w / mag);
}

float Tuple::dot(const Tuple& other) const noexcept
{
    return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
}

Tuple Tuple::cross(const Tuple& other) const noexcept
{
    return Vector(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x);
}

Tuple Tuple::reflect(const Tuple& normal) const noexcept
{
    return *this - normal * 2 * this->dot(normal);
}

Tuple Point(float xIn, float yIn, float zIn) noexcept
{
    return Tuple(xIn, yIn, zIn, 1.0f);
}

Tuple Vector(float xIn, float yIn, float zIn) noexcept
{
    return Tuple(xIn, yIn, zIn, 0.0f);
}
