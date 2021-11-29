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

    Ray(const Tuple& originIn, const Tuple& directionIn) noexcept : origin(originIn), direction(directionIn){};
    [[nodiscard]] Tuple cast(const float t) const noexcept;
    [[nodiscard]] static std::optional<Intersection> hit(const std::vector<Intersection> intersections) noexcept;
    [[nodiscard]] Ray transform(const Matrix<4>& m) const noexcept;
    [[nodiscard]] IntersectionDetails precomputeDetails(const Intersection i, const std::vector<Intersection>& intersections) const noexcept;
};

#endif /* SRC_RAY_HPP_ */
