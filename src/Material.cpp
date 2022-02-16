/*
 * Material.cpp
 *
 *  Created on: Sep 18, 2021
 *      Author: nic
 */

#include "Material.hpp"
#include <cmath>

bool Material::operator==(const Material& other) const noexcept
{
    return color == other.color && ambient == other.ambient && diffuse == other.diffuse && shininess == other.shininess;
}

Color Material::light(const Light& light, const Tuple& point, const Tuple& eyeVector, const Tuple& normalVector, const bool inShadow) const noexcept
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

Color Pattern::colorAt(const Tuple& p) const noexcept
{
    return f(a, b, p);
}

Pattern Pattern::Test() noexcept
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = []([[maybe_unused]] const Color& aP, [[maybe_unused]] const Color& bP, const Tuple& p) -> Color {
        return {p.x, p.y, p.z};
    };
    return {Color::Black, Color::Black, IdentityMatrix(), f};
}

Pattern Pattern::Stripe(const Color& aIn, const Color& bIn) noexcept
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = [](const Color& aP, const Color& bP, const Tuple& p) -> Color {
        return std::fmod(p.x, 2.0F) >= 1.0F || (std::fmod(p.x, 2.0F) < 0.0F && std::fmod(p.x, 2.0F) >= -1.0F) ? bP : aP;
    };

    return {aIn, bIn, IdentityMatrix(), f};
}

Pattern Pattern::Gradient(const Color& aIn, const Color& bIn) noexcept
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = [](const Color& aP, const Color& bP, const Tuple& p) -> Color {
        return aP + (bP - aP) * (p.x - std::floor(p.x));
    };

    return {aIn, bIn, IdentityMatrix(), f};
}

Pattern Pattern::Ring(const Color& aIn, const Color& bIn) noexcept
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = [](const Color& aP, const Color& bP, const Tuple& p) -> Color {
        return std::fmod(std::floor(sqrtf(p.x * p.x + p.z + p.z)), 2) == 0 ? aP : bP;
    };

    return {aIn, bIn, IdentityMatrix(), f};
}

Pattern Pattern::Checker(const Color& aIn, const Color& bIn) noexcept
{
    std::function<Color(const Color&, const Color&, const Tuple&)> f = [](const Color& aP, const Color& bP, const Tuple& p) -> Color {
        return std::floor(std::fmod(std::floor(p.x) + std::floor(p.y) + std::floor(p.z), 2.0F)) == 0 ? aP : bP;
    };

    return {aIn, bIn, IdentityMatrix(), f};
}
