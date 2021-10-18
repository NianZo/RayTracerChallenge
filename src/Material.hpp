/*
 * Material.hpp
 *
 *  Created on: Sep 18, 2021
 *      Author: nic
 */

#ifndef SRC_MATERIAL_HPP_
#define SRC_MATERIAL_HPP_

#include "Color.hpp"
#include "Light.hpp"
#include "Tuple.hpp"
#include <functional>
#include <optional>

class Pattern
{
public:
	Color a;
	Color b;

	Pattern() : a(Color::White), b(Color::Black), f([](float) { return false;}) {};
	Pattern(const Color& aIn, const Color& bIn, const std::function<bool(const float)>& fIn) : a(aIn), b(bIn), f(fIn){};
	Color colorAt(const Tuple& p) const;
private:
	std::function<bool(const float)> f;
};

// All values should be positive, but I'm not sure how to enforce that without something like c++ contracts
class Material
{
  public:
    Color color;
    std::optional<Pattern> pattern;
    float ambient;
    float diffuse;
    float specular;
    float shininess;

    Material() : color(Color(1, 1, 1)), ambient(0.1f), diffuse(0.9f), specular(0.9f), shininess(200.0f){};
    Material(const Color& colorIn, const float ambientIn, const float diffuseIn, const float specularIn, const float shininessIn) : color(colorIn), ambient(ambientIn), diffuse(diffuseIn), specular(specularIn), shininess(shininessIn){};

    bool operator==(const Material& other) const;

    Color light(const Light& light, const Tuple& position, const Tuple& eyeVector, const Tuple& normalVector, const bool inShadow) const;
};

Pattern StripePattern(const Color& aIn, const Color& bIn);

#endif /* SRC_MATERIAL_HPP_ */
