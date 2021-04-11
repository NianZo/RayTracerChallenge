/*
 * ClockFace.cpp
 *
 *  Created on: Apr 11, 2021
 *      Author: nic
 */

#include <string>
#include "Tuple.hpp"
#include "Canvas.hpp"
#include "Transformation.hpp"
#include <iostream>
#include <fstream>
#include <numbers>

void RenderClockFace(const std::string& fileName)
{
	Canvas c(100, 100);

	for (int i = 0; i < 12; i++)
	{
		Tuple location = rotationZ(-std::numbers::pi * i / 6) * translation(0, 10, 0) * Point(0, 0, 0);
		int xCoord = static_cast<int>(location.x) + 50;
		int yCoord = static_cast<int>(location.y) + 50;
		c.pixels[xCoord][yCoord] = Color(1.0, 1.0, 1.0);
	}
	std::ofstream imageFile;
	imageFile.open(fileName, std::ios::out);
	imageFile << c.GetPPMString();
	imageFile.close();
}


