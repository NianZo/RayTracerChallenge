/*
 * Color.hpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#ifndef SRC_COLOR_HPP_
#define SRC_COLOR_HPP_

#define COLOR_EPSILON 0.001f

class Color
{
  public:
    float r;
    float g;
    float b;

    Color() noexcept : r(0.0f), g(0.0f), b(0.0f){};
    Color(float red, float green, float blue) noexcept : r(red), g(green), b(blue){};

    // Operator overloads
    bool operator==(const Color&) const noexcept;
    bool operator!=(const Color&) const noexcept;
    Color operator+(const Color&) const noexcept;
    Color operator-(const Color&) const noexcept;
    Color operator*(const float) const noexcept;
    Color operator*(const Color&) const noexcept;

    const static Color Black;
    const static Color White;
};

#endif /* SRC_COLOR_HPP_ */
