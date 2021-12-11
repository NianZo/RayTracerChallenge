/*
 * Tuple.hpp
 *
 *  Created on: Jan 9, 2021
 *      Author: nic
 */

#ifndef SOURCE_TUPLE_HPP_
#define SOURCE_TUPLE_HPP_

constexpr float TUPLE_EPSILON = 0.002F; // Changed to be much much higher since it is now being used for acne removal offset; needed to be this high to work

class Tuple
{
  public:
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;
    float w = 0.0F;

    Tuple() noexcept = default;
    Tuple(float xIn, float yIn, float zIn, float wIn) noexcept; // : x(xIn), y(yIn), z(zIn), w(wIn) {};

    // Operator overloads
    bool operator==(const Tuple& other) const noexcept;
    bool operator!=(const Tuple& other) const noexcept;
    Tuple operator+(const Tuple& other) const noexcept;
    Tuple operator-(const Tuple& other) const noexcept;
    Tuple operator-() const noexcept;
    Tuple operator*(float s) const noexcept;
    Tuple operator/(float s) const noexcept;

    [[nodiscard]] bool IsPoint() const noexcept;
    [[nodiscard]] bool IsVector() const noexcept;
    [[nodiscard]] float magnitude() const noexcept;
    [[nodiscard]] Tuple normalize() const noexcept;
    [[nodiscard]] float dot(const Tuple& other) const noexcept;
    [[nodiscard]] Tuple cross(const Tuple& other) const noexcept;
    [[nodiscard]] Tuple reflect(const Tuple& normal) const noexcept;
};

Tuple Point(float xIn, float yIn, float zIn) noexcept;
Tuple Vector(float xIn, float yIn, float zIn) noexcept;

#endif /* SOURCE_TUPLE_HPP_ */
