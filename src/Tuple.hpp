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

    Tuple() : x(0.0f), y(0.0f), z(0.0f), w(0.0f){};
    Tuple(float xIn, float yIn, float zIn, float wIn); // : x(xIn), y(yIn), z(zIn), w(wIn) {};

    // Operator overloads
    bool operator==(const Tuple&) const;
    bool operator!=(const Tuple&) const;
    Tuple operator+(const Tuple&) const;
    Tuple operator-(const Tuple&) const;
    Tuple operator-() const;
    Tuple operator*(const float) const;
    Tuple operator/(const float) const;

    [[nodiscard]] bool IsPoint() const;
    [[nodiscard]] bool IsVector() const;
    [[nodiscard]] float magnitude() const;
    [[nodiscard]] Tuple normalize() const;
    [[nodiscard]] float dot(const Tuple&) const;
    [[nodiscard]] Tuple cross(const Tuple&) const;
    [[nodiscard]] Tuple reflect(const Tuple&) const;
};

Tuple Point(float xIn, float yIn, float zIn);
Tuple Vector(float xIn, float yIn, float zIn);

#endif /* SOURCE_TUPLE_HPP_ */
