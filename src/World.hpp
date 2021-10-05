/*
 * World.hpp
 *
 *  Created on: Sep 19, 2021
 *      Author: nic
 */

#ifndef SRC_WORLD_HPP_
#define SRC_WORLD_HPP_

#include <vector>
#include "Sphere.hpp"
#include "Light.hpp"
#include "Ray.hpp"

class World
{
public:
	std::vector<Sphere> objects;
	//std::vector<Light> lights;
	Light light;

	World() {};

	std::vector<Intersection> intersect(Ray r) const;
	Color shadeHit(IntersectionDetails id) const;
	Color colorAt(Ray r) const;
	bool isShadowed(const Tuple& point) const;

	static World BaseWorld();
};


#endif /* SRC_WORLD_HPP_ */
