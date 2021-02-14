/*
 * Color.hpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#ifndef SRC_COLOR_HPP_
#define SRC_COLOR_HPP_

#define COLOR_EPSILON 0.00001f

class Color
{
public:
	float r;
	float g;
	float b;

	Color() : r(0.0f), g(0.0f), b(0.0f) {};
	Color(float red, float green, float blue) : r(red), g(green), b(blue) {};

	// Operator overloads
	bool operator==(const Color&) const;
	bool operator!=(const Color&) const;
	Color operator+(const Color&) const;
	Color operator-(const Color&) const;
	Color operator*(const float) const;
	Color operator*(const Color&) const;
};



#endif /* SRC_COLOR_HPP_ */
