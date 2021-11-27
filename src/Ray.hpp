/*
 * Ray.hpp
 *
 *  Created on: Apr 22, 2021
 *      Author: nic
 */

#ifndef SRC_RAY_HPP_
#define SRC_RAY_HPP_

#include "Shape.hpp"
#include "Tuple.hpp"

#include <optional>
#include <vector>

class Ray
{
  public:
    Tuple origin;
    Tuple direction;

    Ray(const Tuple& originIn, const Tuple& directionIn) : origin(originIn), direction(directionIn){};
    [[nodiscard]] Tuple cast(const float t) const;
    [[nodiscard]] static std::optional<Intersection> hit(const std::vector<Intersection> intersections);
    [[nodiscard]] Ray transform(const Matrix<4>& m) const;
    [[nodiscard]] IntersectionDetails precomputeDetails(Intersection i, const std::vector<Intersection>& intersections) const;
};

#endif /* SRC_RAY_HPP_ */
