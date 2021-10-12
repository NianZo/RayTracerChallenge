/*
 * Ray.hpp
 *
 *  Created on: Apr 22, 2021
 *      Author: nic
 */

#ifndef SRC_RAY_HPP_
#define SRC_RAY_HPP_

#include "Tuple.hpp"
#include "Sphere.hpp"

#include <vector>
#include <optional>

class Ray
{
public:
	Tuple origin;
	Tuple direction;

	Ray(const Tuple& originIn, const Tuple& directionIn) : origin(originIn), direction(directionIn) {};
	const Tuple cast(const float t) const;
	//const std::vector<Intersection> intersect(const Sphere& s) const;
	static const std::optional<Intersection> hit(const std::vector<Intersection> intersections);
	Ray transform(const Matrix<4>& m) const;
	IntersectionDetails precomputeDetails(Intersection i) const;
};



#endif /* SRC_RAY_HPP_ */
