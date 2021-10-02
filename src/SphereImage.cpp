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
	s.transform = translation(500, 500, -18.99) * scaling(1, 0.5, 1);
	s.material.color = Color(1, 0.2, 1);

	Light light(Point(480, 480, -30), Color(1, 1, 1));

	for (int i = 0; i < 1000; i++)
	{
		for(int j = 0; j < 1000; j++)
		{
			Ray r(Point(500, 500, -20), (Point(i, j, 0) - Point(500, 500, -20)).normalize());
			auto hitPoint = Ray::hit(r.intersect(s));
			if (hitPoint)
			{
				Tuple point = r.cast(hitPoint->t);
				c.pixels[j][i] = s.material.light(light, point, -r.direction, hitPoint->object->normal(point));
			}
		}

	}
	std::ofstream imageFile;
	imageFile.open(fileName, std::ios::out);
	imageFile << c.GetPPMString();
	imageFile.close();
}


