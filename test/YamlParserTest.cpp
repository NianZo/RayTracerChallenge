/*
 * YamlParserTest.cpp
 *
 *  Created on: Feb 22, 2022
 *      Author: nic
 */

#include "gtest/gtest.h"
#include "YamlParser.hpp"
#include "Transformation.hpp"

TEST(YamlParser, EmptyStringGivesDefaultWorld)
{
	std::string empty = "";
	YamlParser parser(empty);

	EXPECT_EQ(parser.world.light, Light());
	EXPECT_TRUE(parser.world.spheres.empty());
	EXPECT_TRUE(parser.world.planes.empty());
	EXPECT_TRUE(parser.world.cubes.empty());
	EXPECT_TRUE(parser.world.cylinders.empty());
	EXPECT_TRUE(parser.world.cones.empty());
	EXPECT_TRUE(parser.world.groups.empty());
	EXPECT_EQ(parser.worldCamera, Camera(100, 100, 0.5, IdentityMatrix()));
}

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
	EXPECT_EQ(parser.worldCamera, Camera(100, 100, 0.5, IdentityMatrix()));
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

TEST(YamlParser, DefineMaterial)
{
	std::string s =
			"- define: white-material\n"
			"  value:\n"
			"    color: [ 0.25, 0.5, 0.75 ]\n"
			"    diffuse: 0.1\n"
			"    ambient: 0.2\n"
			"    specular: 0.3\n"
			"    shininess: 0.6\n"
			"    reflective: 0.4\n"
			"    transparency: 0.7\n"
			"    refractive-index: 0.5\n";
	YamlParser parser(s);

	EXPECT_EQ(parser.materials["white-material"], Material(Color(0.25, 0.5, 0.75), 0.2, 0.1, 0.3, 0.6, 0.4, 0.7, 0.5));
}

TEST(YamlParser, ExtendMaterial)
{
	std::string s =
			"- define: white-material\n"
			"  value:\n"
			"    color: [ 0.25, 0.5, 0.75 ]\n"
			"    diffuse: 0.1\n"
			"    ambient: 0.2\n"
			"    specular: 0.3\n"
			"    shininess: 0.6\n"
			"    reflective: 0.4\n"
			"    transparency: 0.7\n"
			"    refractive-index: 0.5\n"
			"- define: blue-material\n"
			"  extend: white-material\n"
			"  value:\n"
			"    color: [ 0, 0, 1 ]";
	YamlParser parser(s);

	EXPECT_EQ(parser.materials["blue-material"], Material(Color(0, 0, 1), 0.2, 0.1, 0.3, 0.6, 0.4, 0.7, 0.5));
}

TEST(YamlParser, ImproperAtCommand)
{
	std::string s =
			"  at: [1, 2, 3]\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "'at:' command in invalid format. Expected: 'at: [ x, y, z ]'");
}

TEST(YamlParser, ImproperIntensityCommand)
{
	std::string s =
			"  intensity: [1, 2, 3]\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "'intensity:' command in invalid format. Expected: 'intensity: [ x, y, z ]'");
}

TEST(YamlParser, ImproperWidthCommand)
{
	std::string s =
			"  width: 100, 2\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "'width:' command in invalid format. Expected: 'width: i'");
}

TEST(YamlParser, ImproperHeightCommand)
{
	std::string s =
			"  height: 100, 2\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "'height:' command in invalid format. Expected: 'height: i'");
}

TEST(YamlParser, ImproperFOVCommand)
{
	std::string s =
			"  field-of-view: 0.5 3\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "'field-of-view:' command in invalid format. Expected: 'field-of-view: f'");
}

TEST(YamlParser, ImproperFromCommand)
{
	std::string s =
			"  from: 2\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "'from:' command in invalid format. Expected: 'from: [ x, y, z ]'");
}

TEST(YamlParser, ImproperToCommand)
{
	std::string s =
			"  to: 3\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "'to:' command in invalid format. Expected: 'to: [ x, y, z ]'");
}

TEST(YamlParser, ImproperUpCommand)
{
	std::string s =
			"  up: 4\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "'up:' command in invalid format. Expected: 'up: [ x, y, z ]'");
}

TEST(YamlParser, ImproperUseOfAtCommand)
{
	std::string s =
			"  at: [ 1, 2, 3 ]\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "Invalid 'at:' specifier for '- add: light' command.");
}

TEST(YamlParser, ImproperUseOfIntensityCommand)
{
	std::string s =
			"  intensity: [ 1, 2, 3 ]\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "Invalid 'intensity:' specifier for '- add: light' command.");
}

TEST(YamlParser, ImproperUseOfFromCommand)
{
	std::string s =
			"  from: [ 1, 2, 3 ]\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "Invalid 'from:' specifier for '- add: camera' command.");
}

TEST(YamlParser, ImproperUseOfToCommand)
{
	std::string s =
			"  to: [ 1, 2, 3 ]\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "Invalid 'to:' specifier for '- add: camera' command.");
}

TEST(YamlParser, ImproperUseOfUpCommand)
{
	std::string s =
			"  up: [ 1, 2, 3 ]\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "Invalid 'up:' specifier for '- add: camera' command.");
}

TEST(YamlParser, ImproperUseOfFOVCommand)
{
	std::string s =
			"  field-of-view: 0.5\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "Invalid 'field-of-view:' specifier for '- add: camera' command.");
}

TEST(YamlParser, ImproperUseOfWidthCommand)
{
	std::string s =
			"  width: 500\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "Invalid 'width:' specifier for '- add: camera' command.");
}

TEST(YamlParser, ImproperUseOfHeightCommand)
{
	std::string s =
			"  height: 500\n";
	std::string errorCode = "";
	try
	{
		YamlParser parser(s);
	} catch (std::runtime_error& e)
	{
		errorCode = std::string(e.what());
	}
	EXPECT_EQ(errorCode, "Invalid 'height:' specifier for '- add: camera' command.");
}





