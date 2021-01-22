/*
 * Tuple.hpp
 *
 *  Created on: Jan 9, 2021
 *      Author: nic
 */

#ifndef SOURCE_TUPLE_HPP_
#define SOURCE_TUPLE_HPP_

#define TUPLE_EPSILON 0.00001f

class Tuple
{
public:
	float x;
	float y;
	float z;
	float w;

	Tuple(float xIn, float yIn, float zIn, float wIn);// : x(xIn), y(yIn), z(zIn), w(wIn) {};

	// Operator overloads
	bool operator==(const Tuple&) const;
	bool operator!=(const Tuple&) const;
	Tuple operator+(const Tuple&) const;
	Tuple operator-(const Tuple&) const;
	Tuple operator-() const;
	Tuple operator*(const float) const;
	Tuple operator/(const float) const;

	bool IsPoint() const;
	bool IsVector() const;
	float magnitude() const;
	Tuple normalize() const;
	float dot(const Tuple&) const;
	Tuple cross(const Tuple&) const;
};

Tuple Point(float xIn, float yIn, float zIn);
Tuple Vector(float xIn, float yIn, float zIn);


#endif /* SOURCE_TUPLE_HPP_ */
