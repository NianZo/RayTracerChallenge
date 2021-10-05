/*
 * SphereImage.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: nic
 */

#include <string>
#include "Canvas.hpp"
#include "Transformation.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include <iostream>
#include <fstream>
#include <numbers>

void RenderSphere(const std::string& fileName)
{
	Canvas c(1000, 1000);
	Sphere s;
	s.transform = translation(500.0f, 500.0f, -18.99f) * scaling(1.0f, 0.5f, 1.0f);
	s.material.color = Color(1.0f, 0.2f, 1.0f);

	Light light(Point(480, 480, -30), Color(1, 1, 1));

	for (uint32_t i = 0; i < 1000; i++)
	{
		for(uint32_t j = 0; j < 1000; j++)
		{
			Ray r(Point(500, 500, -20), (Point(static_cast<float>(i), static_cast<float>(j), 0) - Point(500, 500, -20)).normalize());
			auto hitPoint = Ray::hit(r.intersect(s));
			if (hitPoint)
			{
				Tuple point = r.cast(hitPoint->t);
				c.pixels[j][i] = s.material.light(light, point, -r.direction, hitPoint->object->normal(point), false);
			}
		}

	}
	std::ofstream imageFile;
	imageFile.open(fileName, std::ios::out);
	imageFile << c.GetPPMString();
	imageFile.close();
}


