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
#include "Sphere.hpp"
#include <functional>
#include <vector>

class World {
  public:
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    //std::vector<std::reference_wrapper<Shape>> objects;
    //std::vector<Light> lights;
    Light light;

    World(){};

    std::vector<std::reference_wrapper<const Shape>> objects() const;
    std::vector<Intersection> intersect(Ray r) const;
    Color shadeHit(IntersectionDetails id) const;
    Color colorAt(Ray r) const;
    bool isShadowed(const Tuple& point) const;

    static World BaseWorld();
};

#endif /* SRC_WORLD_HPP_ */
