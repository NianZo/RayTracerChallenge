/*
 * CameraTest.cpp
 *
 *  Created on: Sep 26, 2021
 *      Author: nic
 */

#include "gtest/gtest.h"

#include <numbers>
#include "../src/Camera.hpp"
#include "../src/Matrix.hpp"
#include "../src/Ray.hpp"
#include "../src/Transformation.hpp"
#include "../src/World.hpp"
#include "../src/Canvas.hpp"

TEST(CameraTest, CameraConstruction)
{
	const int hSize = 160;
	const int vSize = 120;
	const float fov = std::numbers::pi / 2;

	Camera c = Camera(hSize, vSize, fov);

	EXPECT_EQ(c.hSize, hSize);
	EXPECT_EQ(c.vSize, vSize);
	EXPECT_EQ(c.fov, fov);
	EXPECT_EQ(c.transform, IdentityMatrix());
}

TEST(CameraTest, PixelSizeForHorizontalCanvas)
{
	Camera c = Camera(200, 125, std::numbers::pi / 2);

	EXPECT_FLOAT_EQ(c.pixelSize, 0.01);
}

TEST(CameraTest, PixelSizeForVerticalCanvas)
{
	Camera c = Camera(125, 200, std::numbers::pi / 2);

	EXPECT_FLOAT_EQ(c.pixelSize, 0.01);
}

TEST(CameraTest, RayThroughCenterOfCanvas)
{
	Camera c = Camera(201, 101, std::numbers::pi / 2);
	Ray r = c.rayForPixel(100, 50);

	EXPECT_EQ(r.origin, Point(0, 0, 0));
	EXPECT_EQ(r.direction, Vector(0, 0, -1));
}

TEST(CameraTest, RayThroughCornerOfCanvas)
{
	Camera c = Camera(201, 101, std::numbers::pi / 2);
	Ray r = c.rayForPixel(0, 0);

	EXPECT_EQ(r.origin, Point(0, 0, 0));
	EXPECT_EQ(r.direction, Vector(0.66519, 0.33259, -0.66851));
}

TEST(CameraTest, RayThroughCanvasWhenCameraTransformed)
{
	Camera c = Camera(201, 101, std::numbers::pi / 2);
	c.transform = rotationY(std::numbers::pi / 4) * translation(0, -2, 5);
	Ray r = c.rayForPixel(100, 50);

	EXPECT_EQ(r.origin, Point(0, 2, -5));
	EXPECT_EQ(r.direction, Vector(sqrt(2) / 2, 0, -sqrt(2) / 2));
}

TEST(CameraTest, RenderingAWorld)
{
	World w = World::BaseWorld();
	Camera c = Camera(11, 11, std::numbers::pi / 2);
	Tuple from = Point(0, 0, -5);
	Tuple to = Point(0, 0, 0);
	Tuple up = Vector(0, 1, 0);
	c.transform = ViewTransform(from, to, up);
	Canvas image = c.Render(w);

	EXPECT_EQ(image.pixels[5][5], Color(0.38066, 0.47583, 0.2855));
}









