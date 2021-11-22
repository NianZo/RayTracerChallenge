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
    return f(a, b, p); // ? b : a;
}

Pattern Pattern::Test()
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = [](const Color&, const Color&, const Tuple& p) -> Color {
        return Color(p.x, p.y, p.z);
    };
    return Pattern(Color::Black, Color::Black, IdentityMatrix(), f);
}

Pattern Pattern::Stripe(const Color& aIn, const Color& bIn)
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = [](const Color& a, const Color& b, const Tuple& p) -> Color {
        return std::fmod(p.x, 2.0f) >= 1.0f || (std::fmod(p.x, 2.0f) < 0.0f && std::fmod(p.x, 2.0f) >= -1.0f) ? b : a;
    };

    return Pattern(aIn, bIn, IdentityMatrix(), f);
}

Pattern Pattern::Gradient(const Color& aIn, const Color& bIn)
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = [](const Color& a, const Color& b, const Tuple& p) -> Color {
        return a + (b - a) * (p.x - std::floor(p.x));
    };

    return Pattern(aIn, bIn, IdentityMatrix(), f);
}

Pattern Pattern::Ring(const Color& aIn, const Color& bIn)
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = [](const Color& a, const Color& b, const Tuple& p) -> Color {
        return std::fmod(std::floor(sqrt(p.x * p.x + p.z + p.z)), 2) == 0 ? a : b;
    };

    return Pattern(aIn, bIn, IdentityMatrix(), f);
}

Pattern Pattern::Checker(const Color& aIn, const Color& bIn)
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = [](const Color& a, const Color& b, const Tuple& p) -> Color {
        return std::floor(std::fmod(std::floor(p.x) + std::floor(p.y) + std::floor(p.z), 2.0f)) == 0 ? a : b;
    };

    return Pattern(aIn, bIn, IdentityMatrix(), f);
}
