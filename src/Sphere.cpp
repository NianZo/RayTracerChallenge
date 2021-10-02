/*
 * Sphere.cpp
 *
 *  Created on: Sep 17, 2021
 *      Author: nic
 */

#include "Sphere.hpp"

// Implicitly assumes that p is on the sphere surface and is a valid point (w = 1)
Tuple Sphere::normal(const Tuple& p) const
{
	const Tuple objectSpaceP = transform.inverse() * p;
	const Tuple objectSpaceNormal = (objectSpaceP - Point(0, 0, 0));
	Tuple worldSpaceNormal = transform.inverse().transpose() * objectSpaceNormal; // This is technically a hack and messes with w; set it to 0 at the end;
	worldSpaceNormal.w = 0;
	return worldSpaceNormal.normalize();
}
