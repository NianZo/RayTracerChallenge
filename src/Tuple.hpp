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

	bool IsPoint() const;
	bool IsVector() const;
};

// TODO implementation does not guarantee that w = 1.0f for a point. Can it?
class Point : public Tuple
{
public:
	Point(float xIn, float yIn, float zIn) : Tuple(xIn, yIn, zIn, 1.0f) {};
};

// TODO implementation does not guarantee that w = 0.0f for a vector. Can it?
class Vector : public Tuple
{
public:
	Vector(float xIn, float yIn, float zIn) : Tuple(xIn, yIn, zIn, 0.0f) {};
};


#endif /* SOURCE_TUPLE_HPP_ */
