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

const Tuple Ray::cast(const float t) const
{
    return origin + direction * t;
}

//const std::vector<Intersection> Ray::intersect(const Sphere& s) const
//{
//	const Ray ray2 = this->transform(s.transform.inverse());
//	const Tuple sphereToRay = ray2.origin - Point(0, 0, 0);
//
//	const float a = ray2.direction.dot(ray2.direction);
//	const float b = 2 * ray2.direction.dot(sphereToRay);
//	const float c = sphereToRay.dot(sphereToRay) - 1;
//
//	const float discriminant = b * b - 4 * a * c;
//
//	std::vector<Intersection> intersections;
//	if (discriminant >= 0)
//	{
//		intersections.push_back(Intersection((-b - sqrtf(discriminant)) / (2 * a), s));
//		intersections.push_back(Intersection((-b + sqrtf(discriminant)) / (2 * a), s));
//	}
//
//	return intersections;
//}

const std::optional<Intersection> Ray::hit(std::vector<Intersection> intersections){
    std::sort(intersections.begin(), intersections.end());
    for (const auto intersection : intersections)
    {
        if (intersection.t > 0)
        {
            return intersection;
        }
    }
    return std::nullopt;
}

Ray Ray::transform(const Matrix<4>& m) const
{
    return Ray(m * this->origin, m * this->direction);
}

IntersectionDetails Ray::precomputeDetails(Intersection i, const std::vector<Intersection>& intersections) const
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
    const Tuple reflectionVector = direction.reflect(normalVector);

    std::vector<Shape> containers;
    float n1 = 0.0f;
    float n2 = 0.0f;
    for (auto intersection : intersections)
    {
    	if (i == intersection)
    	{
    		if (containers.empty())
    		{
    			n1 = 1.0f;
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
    			n2 = 1.0f;
    		} else
    		{
    			n2 = containers.back().material.refractiveIndex;
    		}
    		break;
    	}
    }


    IntersectionDetails id = {*(i.object), position, overPosition, eyeVector, normalVector, reflectionVector, i.t, inside, n1, n2};
    return id;
}

#endif /* SRC_RAY_CPP_ */