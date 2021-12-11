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

    Pattern() noexcept : a(Color::White), b(Color::Black), transform(IdentityMatrix()), f([]([[maybe_unused]] Color aF, [[maybe_unused]] Color bF, [[maybe_unused]] Tuple pF) { return Color::Black; }){};
    Pattern(const Color& aIn, const Color& bIn, const Matrix<4>& transformIn, const std::function<Color(const Color& aF, const Color& bF, const Tuple& pF)> fIn) noexcept : a(aIn), b(bIn), transform(transformIn), f(std::move(fIn)){};
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
    Color color = Color::White;
    std::optional<Pattern> pattern;
    float ambient = 0.1F;
    float diffuse = 0.9F;
    float specular = 0.9F;
    float shininess = 200.0F;
    float reflectivity = 0.0F;
    float transparency = 0.0F;
    float refractiveIndex = 1.0F;

    Material() noexcept = default;
    Material(const Color& colorIn, float ambientIn, float diffuseIn, float specularIn, float shininessIn, float reflectivityIn, float transparencyIn, float refractiveIndexIn) noexcept : color(colorIn), ambient(ambientIn), diffuse(diffuseIn), specular(specularIn), shininess(shininessIn), reflectivity(reflectivityIn), transparency(transparencyIn), refractiveIndex(refractiveIndexIn){};

    [[nodiscard]] bool operator==(const Material& other) const noexcept;
    [[nodiscard]] Color light(const Light& light, const Tuple& point, const Tuple& eyeVector, const Tuple& normalVector, bool inShadow) const noexcept;
};

#endif /* SRC_MATERIAL_HPP_ */
