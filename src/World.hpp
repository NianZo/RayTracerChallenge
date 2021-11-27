/*
 * World.hpp
 *
 *  Created on: Sep 19, 2021
 *      Author: nic
 */

#ifndef SRC_WORLD_HPP_
#define SRC_WORLD_HPP_

#include "Light.hpp"
#include "Ray.hpp"
#include "Shape.hpp"
#include <functional>
#include <vector>

class World
{
  public:
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<Cube> cubes;
    std::vector<Cylinder> cylinders;
    std::vector<Cone> cones;
    Light light;

    World() = default;

    [[nodiscard]] std::vector<std::reference_wrapper<const Shape>> objects() const;
    [[nodiscard]] std::vector<Intersection> intersect(Ray r) const;
    [[nodiscard]] Color shadeHit(const IntersectionDetails& id, int remainingCalls = 4) const;
    [[nodiscard]] Color reflectedColor(const IntersectionDetails& id, int remainingCalls = 4) const;
    [[nodiscard]] Color refractedColor(const IntersectionDetails& id, int remainingCalls = 4) const;
    [[nodiscard]] Color colorAt(Ray r, int remainingCalls = 4) const;
    [[nodiscard]] bool isShadowed(const Tuple& point) const;

    static World BaseWorld();
};

#endif /* SRC_WORLD_HPP_ */
