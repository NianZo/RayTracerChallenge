/*
 * Material.cpp
 *
 *  Created on: Sep 18, 2021
 *      Author: nic
 */

#include "Material.hpp"
#include <cmath>

bool Material::operator==(const Material& other) const
{
	return color == other.color && ambient == other.ambient && diffuse == other.diffuse && shininess == other.shininess;
}

Color Material::light(const Light& light, const Tuple& point, const Tuple& eyeVector, const Tuple& normalVector, const bool inShadow) const
{
	const Color effectiveColor = color * light.intensity;
	const Tuple lightVector = (light.position - point).normalize();
	const Color ambientLight = effectiveColor * ambient;
	const float lightDotNormal = lightVector.dot(normalVector);

	Color diffuseLight = Color::Black();
	Color specularLight = Color::Black();

	if (lightDotNormal > 0 && !inShadow)
	{
		diffuseLight = effectiveColor * diffuse * lightDotNormal;
		const Tuple reflectionVector = (-lightVector).reflect(normalVector);
		const float reflectionDotEye = reflectionVector.dot(eyeVector);

		if (reflectionDotEye <= 0)
		{
			specularLight = Color::Black();
		} else
		{
			const float factor = std::pow(reflectionDotEye, shininess);
			specularLight = light.intensity * specular * factor;
		}
	}

	return ambientLight + diffuseLight + specularLight;
}
