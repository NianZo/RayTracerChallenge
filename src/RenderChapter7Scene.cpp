/*
 * RenderChapter7Scene.cpp
 *
 *  Created on: Oct 2, 2021
 *      Author: nic
 */

#include <string>
#include "Canvas.hpp"
#include "Transformation.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include <iostream>
#include <fstream>
#include <numbers>

void RenderChapter7Scene(const std::string& fileName)
{
	Sphere floor;
	floor.transform = scaling(10, 0.01, 10);
	floor.material.color = Color(1, 0.9, 0.9);
	floor.material.specular = 0;

	Sphere leftWall;
	leftWall.transform = translation(0, 0, 5) * rotationY(-std::numbers::pi / 4) * rotationX(std::numbers::pi / 2) * scaling(10, 0.01, 10);
	leftWall.material = floor.material;

	Sphere rightWall;
	rightWall.transform = translation(0, 0, 5) * rotationY(std::numbers::pi / 4) * rotationX(std::numbers::pi / 2) * scaling(10, 0.01, 10);
	rightWall.material = floor.material;

	Sphere middle;
	middle.transform = translation(-0.5, 1, 0.5);
	middle.material.color = Color(0.1, 1, 0.5);
	middle.material.diffuse = 0.7;
	middle.material.specular = 0.3;

	Sphere right;
	right.transform = translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5);
	right.material.color = Color(0.5, 1, 0.1);
	right.material.diffuse = 0.7;
	right.material.specular = 0.3;

	Sphere left;
	left.transform = translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33);
	left.material.color = Color(1, 0.8, 0.1);
	left.material.diffuse = 0.7;
	left.material.specular = 0.3;

	Light light(Point(-10, 10, -10), Color(1, 1, 1));

	World w;
	w.light = light;
	w.objects.push_back(floor);
	w.objects.push_back(leftWall);
	w.objects.push_back(rightWall);
	w.objects.push_back(middle);
	w.objects.push_back(right);
	w.objects.push_back(left);

	Camera c = Camera(100, 50, std::numbers::pi / 3);
	c.transform = ViewTransform(Point(0, 1.5, -5), Point(0, 1, 0), Vector(0, 1, 0));

	Canvas canvas = c.Render(w);

	std::ofstream imageFile;
	imageFile.open(fileName, std::ios::out);
	imageFile << canvas.GetPPMString();
	imageFile.close();
}





