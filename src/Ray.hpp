/*
 * Ray.hpp
 *
 *  Created on: Apr 22, 2021
 *      Author: nic
 */

#ifndef SRC_RAY_HPP_
#define SRC_RAY_HPP_

#include "../src/Tuple.hpp"

class Ray
{
public:
	Tuple origin;
	Tuple direction;

	Ray(const Tuple& originIn, const Tuple& directionIn) : origin(originIn), direction(directionIn) {};
	const Tuple cast(const float t) const;
};


#endif /* SRC_RAY_HPP_ */
