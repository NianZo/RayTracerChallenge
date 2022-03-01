/*
 * YamlParserTest.cpp
 *
 *  Created on: Feb 22, 2022
 *      Author: nic
 */

#include "gtest/gtest.h"
#include "YamlParser.hpp"
#include "Transformation.hpp"

TEST(YamlParser, IgnoresGibberish)
{
	std::string gibberish =
			"There was a young lady named Bright\n"
			"who traveled much faster than light.\n"
			"She set out one day\n"
			"in a relative way,\n"
			"and came back the previous night.\n"
			"virgo";
	YamlParser parser(gibberish);

	EXPECT_EQ(parser.world.light, Light());
	EXPECT_TRUE(parser.world.spheres.empty());
	EXPECT_TRUE(parser.world.planes.empty());
	EXPECT_TRUE(parser.world.cubes.empty());
	EXPECT_TRUE(parser.world.cylinders.empty());
	EXPECT_TRUE(parser.world.cones.empty());
	EXPECT_TRUE(parser.world.groups.empty());
}

TEST(YamlParser, AddCamera)
{
	std::string cameraString =
			"- add: camera\n"
			"  width: 100\n"
			"  height: 200\n"
			"  field-of-view: 0.875\n"
			"  from: [ -6, 6, -10 ]\n"
			"  to: [ 6, 0, 6 ]\n"
			"  up: [ -0.45, 1, 0 ]\n";
	YamlParser parser(cameraString);

	EXPECT_EQ(parser.worldCamera, Camera(100, 200, 0.875, ViewTransform(Point(-6, 6, -10), Point(6, 0, 6), Vector(-0.45, 1, 0))));
}

TEST(YamlParser, AddLight)
{
	std::string lightString =
			"- add: light\n"
			"  at: [ 1, 2, 3 ]\n"
			"  intensity: [ 0.25, 0.5, 0.75 ]\n";
	YamlParser parser(lightString);

	EXPECT_EQ(parser.world.light, Light(Point(1, 2, 3), Color(0.25, 0.5, 0.75)));
}
