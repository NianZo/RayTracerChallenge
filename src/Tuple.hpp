/*
 * Tuple.hpp
 *
 *  Created on: Jan 9, 2021
 *      Author: nic
 */

#ifndef SOURCE_TUPLE_HPP_
#define SOURCE_TUPLE_HPP_

#define TUPLE_EPSILON 0.002f // Changed to be much much higher since it is now being used for acne removal offset; needed to be this high to work

class Tuple
{
  public:
    float x;
    float y;
    float z;
    float w;

    Tuple() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f){};
    Tuple(float xIn, float yIn, float zIn, float wIn) noexcept; // : x(xIn), y(yIn), z(zIn), w(wIn) {};

    // Operator overloads
    bool operator==(const Tuple&) const noexcept;
    bool operator!=(const Tuple&) const noexcept;
    Tuple operator+(const Tuple&) const noexcept;
    Tuple operator-(const Tuple&) const noexcept;
    Tuple operator-() const noexcept;
    Tuple operator*(const float) const noexcept;
    Tuple operator/(const float) const noexcept;

    [[nodiscard]] bool IsPoint() const noexcept;
    [[nodiscard]] bool IsVector() const noexcept;
    [[nodiscard]] float magnitude() const noexcept;
    [[nodiscard]] Tuple normalize() const noexcept;
    [[nodiscard]] float dot(const Tuple&) const noexcept;
    [[nodiscard]] Tuple cross(const Tuple&) const noexcept;
    [[nodiscard]] Tuple reflect(const Tuple&) const noexcept;
};

Tuple Point(float xIn, float yIn, float zIn) noexcept;
Tuple Vector(float xIn, float yIn, float zIn) noexcept;

#endif /* SOURCE_TUPLE_HPP_ */
