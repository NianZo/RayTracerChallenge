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
#include "Matrix.hpp"
#include "Tuple.hpp"
#include <functional>
#include <optional>

class Pattern
{
  public:
    Color a;
    Color b;
    Matrix<4> transform;

    Pattern() noexcept : a(Color::White), b(Color::Black), transform(IdentityMatrix()), f([](Color, Color, Tuple) { return Color::Black; }){};
    Pattern(const Color& aIn, const Color& bIn, const Matrix<4>& transformIn, const std::function<Color(const Color&, const Color&, const Tuple&)> fIn) noexcept : a(aIn), b(bIn), transform(transformIn), f(std::move(fIn)){};
    [[nodiscard]] Color colorAt(const Tuple& p) const noexcept;

    static Pattern Test() noexcept;
    static Pattern Stripe(const Color& aIn, const Color& bIn) noexcept;
    static Pattern Gradient(const Color& aIn, const Color& bIn) noexcept;
    static Pattern Ring(const Color& aIn, const Color& bIn) noexcept;
    static Pattern Checker(const Color& aIn, const Color& bIn) noexcept;

  private:
    std::function<Color(const Color&, const Color&, const Tuple&)> f;
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
    float reflectivity;
    float transparency;
    float refractiveIndex;

    Material() noexcept : color(Color(1, 1, 1)), ambient(0.1f), diffuse(0.9f), specular(0.9f), shininess(200.0f), reflectivity(0.0f), transparency(0.0f), refractiveIndex(1.0f){};
    Material(const Color& colorIn, const float ambientIn, const float diffuseIn, const float specularIn, const float shininessIn, const float reflectivityIn, const float transparencyIn, const float refractiveIndexIn) noexcept : color(colorIn), ambient(ambientIn), diffuse(diffuseIn), specular(specularIn), shininess(shininessIn), reflectivity(reflectivityIn), transparency(transparencyIn), refractiveIndex(refractiveIndexIn){};

    [[nodiscard]] bool operator==(const Material& other) const noexcept;
    [[nodiscard]] Color light(const Light& light, const Tuple& position, const Tuple& eyeVector, const Tuple& normalVector, const bool inShadow) const noexcept;
};

#endif /* SRC_MATERIAL_HPP_ */
