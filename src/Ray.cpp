/*
 * Ray.cpp
 *
 *  Created on: Apr 22, 2021
 *      Author: nic
 */

#ifndef SRC_RAY_CPP_
#define SRC_RAY_CPP_

#include "Ray.hpp"
#include <algorithm>
#include <cmath>

Tuple Ray::cast(const float t) const noexcept
{
    return origin + direction * t;
}

std::optional<Intersection> Ray::hit(const std::vector<Intersection>& intersections) noexcept
{
    auto sortedIntersections = intersections;
    std::sort(sortedIntersections.begin(), sortedIntersections.end());
    for (const auto intersection : sortedIntersections)
    {
        if (intersection.t > 0)
        {
            return intersection;
        }
    }
    return std::nullopt;
}

Ray Ray::transform(const Matrix<4>& m) const noexcept
{
    return Ray(m * this->origin, m * this->direction);
}

IntersectionDetails Ray::precomputeDetails(const Intersection& i, const std::vector<Intersection>& intersections) const noexcept
{
    const Tuple position = cast(i.t);
    const Tuple eyeVector = -direction;
    Tuple normalVector = i.object->normal(position);
    const bool inside = normalVector.dot(eyeVector) < 0;
    if (inside)
    {
        normalVector = -normalVector;
    }
    const Tuple overPosition = position + normalVector * TUPLE_EPSILON;
    const Tuple underPosition = position - normalVector * TUPLE_EPSILON;
    const Tuple reflectionVector = direction.reflect(normalVector);

    std::vector<Shape> containers;
    float n1 = 0.0F;
    float n2 = 0.0F;
    for (auto intersection : intersections)
    {
        if (i == intersection)
        {
            if (containers.empty())
            {
                n1 = 1.0F;
            } else
            {
                n1 = containers.back().material.refractiveIndex;
            }
        }

        auto containerPosition = std::find(containers.begin(), containers.end(), *intersection.object);
        if (containerPosition != containers.end())
        {
            containers.erase(containerPosition);
        } else
        {
            containers.push_back(*intersection.object);
        }

        if (i == intersection)
        {
            if (containers.empty())
            {
                n2 = 1.0F;
            } else
            {
                n2 = containers.back().material.refractiveIndex;
            }
            break;
        }
    }

    // Schlick reflectance - Algorithm from "Reflections and Refractions in Ray Tracing" by Bram de Greve
    float cos = eyeVector.dot(normalVector);
    //float reflectance = 0.0f;
    float sin2T = 0.0F;
    if (n1 > n2)
    {
        const float nRatio = n1 / n2;
        sin2T = nRatio * nRatio * (1 - cos * cos);
        //    	if (sin2T > 1.0f)
        //    	{
        //    		reflectance = 1.0f;
        //    	}
        cos = sqrtf(1.0F - sin2T);
    }
    const float r0 = powf(((n1 - n2) / (n1 + n2)), 2);
    const float reflectance = sin2T > 1.0F ? 1.0F : r0 + (1 - r0) * powf(1 - cos, 5);

    IntersectionDetails id = {position, overPosition, underPosition, eyeVector, normalVector, reflectionVector, *(i.object), i.t, reflectance, n1, n2, inside};
    return id;
}

#endif /* SRC_RAY_CPP_ */
