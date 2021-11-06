/*
 * RenderChapter7Scene.cpp
 *
 *  Created on: Oct 2, 2021
 *      Author: nic
 */

#include "Camera.hpp"
#include "Canvas.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Transformation.hpp"
#include "World.hpp"
#include <fstream>
#include <iostream>
#include <numbers>
#include <string>
#include <chrono>

void RenderChapter7Scene(const std::string& fileName)
{
    Sphere floor;
    floor.transform = scaling(10.0f, 0.01f, 10.0f);
    floor.material.color = Color(1.0f, 0.9f, 0.9f);
    floor.material.specular = 0;

    Sphere leftWall;
    leftWall.transform = translation(0.0f, 0.0f, 5.0f) * rotationY(-std::numbers::pi_v<float> / 4.0f) * rotationX(std::numbers::pi_v<float> / 2.0f) * scaling(10.0f, 0.01f, 10.0f);
    leftWall.material = floor.material;

    Sphere rightWall;
    rightWall.transform = translation(0.0f, 0.0f, 5.0f) * rotationY(std::numbers::pi_v<float> / 4) * rotationX(std::numbers::pi_v<float> / 2) * scaling(10.0f, 0.01f, 10.0f);
    rightWall.material = floor.material;

    Sphere middle;
    middle.transform = translation(-0.5, 1, 4.5) * scaling(4, 4, 4);
    middle.material.pattern = Pattern::Stripe(Color(1, 0, 0), Color(0, 1, 1));
    middle.material.pattern->transform = scaling(0.5, 0.5, 0.5);
    middle.material.color = Color(0.1f, 1.0f, 0.5f);
    middle.material.diffuse = 0.7f;
    middle.material.specular = 0.3f;

    Sphere right;
    right.transform = translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5);
    right.material.color = Color(0.5f, 1.0f, 0.1f);
    right.material.diffuse = 0.7f;
    right.material.specular = 0.3f;

    Sphere left;
    left.transform = translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f);
    left.material.color = Color(1.0f, 0.8f, 0.1f);
    left.material.diffuse = 0.7f;
    left.material.specular = 0.3f;

    Plane p;

    Light light(Point(-10, 10, -10), Color(1, 1, 1));

    World w;
    w.light = light;
    //w.objects.push_back(floor);
    //w.objects.push_back(leftWall);
    //w.objects.push_back(rightWall);
    w.planes.push_back(p);
    w.spheres.push_back(middle);
    w.spheres.push_back(right);
    w.spheres.push_back(left);

    Camera c = Camera(640, 480, std::numbers::pi_v<float> / 3);
    c.transform = ViewTransform(Point(0, 1.5, -5), Point(0, 1, 0), Vector(0, 1, 0));

    auto startRenderTime = std::chrono::steady_clock::now();
    Canvas canvas = c.Render(w);
    auto endRenderTime = std::chrono::steady_clock::now();

    std::cout << "Time to render scene: " << static_cast<std::chrono::duration<double>>(endRenderTime - startRenderTime).count() << std::endl;

    std::ofstream imageFile(fileName, std::ios::out);
    //imageFile.open(fileName, std::ios::out);
    imageFile << canvas.GetPPMString();
    //imageFile.close();
}
