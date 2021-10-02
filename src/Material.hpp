/*
 * Material.hpp
 *
 *  Created on: Sep 18, 2021
 *      Author: nic
 */

#ifndef SRC_MATERIAL_HPP_
#define SRC_MATERIAL_HPP_

#include "Color.hpp"
#include "Tuple.hpp"
#include "Light.hpp"

// All values should be positive, but I'm not sure how to enforce that without something like c++ contracts
class Material
{
public:
	Color color;
	float ambient;
	float diffuse;
	float specular;
	float shininess;

	Material() : color(Color(1, 1, 1)), ambient(0.1), diffuse(0.9), specular(0.9), shininess(200.0) {};
	Material(Color color, float ambient, float diffuse, float specular, float shininess) : color(color), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {};

	bool operator==(const Material& other) const;

	Color light(Light light, Tuple position, Tuple eyeVector, Tuple normalVector) const;
};


#endif /* SRC_MATERIAL_HPP_ */
