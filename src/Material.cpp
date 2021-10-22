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
    const Color effectiveColor = light.intensity * (pattern ? pattern->colorAt(pattern->transform.inverse() * point) : color);
    const Tuple lightVector = (light.position - point).normalize();
    const Color ambientLight = effectiveColor * ambient;
    const float lightDotNormal = lightVector.dot(normalVector);

    Color diffuseLight = Color::Black;
    Color specularLight = Color::Black;

    if (lightDotNormal > 0 && !inShadow)
    {
        diffuseLight = effectiveColor * diffuse * lightDotNormal;
        const Tuple reflectionVector = (-lightVector).reflect(normalVector);
        const float reflectionDotEye = reflectionVector.dot(eyeVector);

        if (reflectionDotEye <= 0)
        {
            specularLight = Color::Black;
        } else
        {
            const float factor = std::pow(reflectionDotEye, shininess);
            specularLight = light.intensity * specular * factor;
        }
    }

    return ambientLight + diffuseLight + specularLight;
}

Color Pattern::colorAt(const Tuple& p) const
{
	return f(a, b, p.x);// ? b : a;
}

Pattern Pattern::Stripe(const Color& aIn, const Color& bIn)
{
	std::function<Color(const Color&, const Color&, const float)> f = [](const Color& a, const Color& b, const float x) -> Color
	{
		return std::fmod(x, 2.0f) >= 1.0f || (std::fmod(x, 2.0f) < 0.0f && std::fmod(x, 2.0f) >= -1.0f) ? b : a;
	};

	return Pattern(aIn, bIn, IdentityMatrix(), f);
}

Pattern Pattern::Gradient(const Color& aIn, const Color& bIn)
{
	std::function<Color(const Color&, const Color&, const float)> f = [](const Color& a, const Color& b, const float x) -> Color
	{
		return a + (b - a) * (x - std::floor(x));
	};

	return Pattern(aIn, bIn, IdentityMatrix(), f);
}
