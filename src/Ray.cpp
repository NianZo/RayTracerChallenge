/*
 * Ray.cpp
 *
 *  Created on: Apr 22, 2021
 *      Author: nic
 */

#ifndef SRC_RAY_CPP_
#define SRC_RAY_CPP_

#include "Ray.hpp"

const Tuple Ray::cast(const float t) const
{
	return origin + direction * t;
}

#endif /* SRC_RAY_CPP_ */
