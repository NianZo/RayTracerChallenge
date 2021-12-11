/*
 * Color.hpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#ifndef SRC_COLOR_HPP_
#define SRC_COLOR_HPP_

#define COLOR_EPSILON 0.001F

class Color
{
  public:
    float r = 0.0F;
    float g = 0.0F;
    float b = 0.0F;

    Color() noexcept = default;
    Color(float red, float green, float blue) noexcept : r(red), g(green), b(blue){};

    // Operator overloads
    bool operator==(const Color& other) const noexcept;
    bool operator!=(const Color& other) const noexcept;
    Color operator+(const Color& other) const noexcept;
    Color operator-(const Color& other) const noexcept;
    Color operator*(float scalar) const noexcept;
    Color operator*(const Color& other) const noexcept;

    const static Color Black;
    const static Color White;
};

#endif /* SRC_COLOR_HPP_ */
