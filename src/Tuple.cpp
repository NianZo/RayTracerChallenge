/*
 * Tuple.cpp
 *
 *  Created on: Jan 9, 2021
 *      Author: nic
 */

#include "../src/Tuple.hpp"
#include <cmath>
#include <cstdlib>


Tuple::Tuple(float xIn, float yIn, float zIn, float wIn)
{
	x = xIn;
	y = yIn;
	z = zIn;
	w = wIn;
}

bool Tuple::IsPoint() const
{
	return std::abs(this->w - 1.0f) < TUPLE_EPSILON;
}

bool Tuple::IsVector() const
{
	return std::abs(this->w) < TUPLE_EPSILON;
}
