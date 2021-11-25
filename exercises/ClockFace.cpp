/*
 * ClockFace.cpp
 *
 *  Created on: Apr 11, 2021
 *      Author: nic
 */

#include "Canvas.hpp"
#include "Transformation.hpp"
#include "Tuple.hpp"
#include <fstream>
#include <iostream>
#include <numbers>
#include <string>

void RenderClockFace(const std::string& fileName)
{
    Canvas c(100, 100);

    for (uint32_t i = 0; i < 12; i++)
    {
        Tuple location = rotationZ(-std::numbers::pi_v<float> * static_cast<float>(i) / 6.0f) * translation(0, 10, 0) * Point(0, 0, 0);
        uint32_t xCoord = static_cast<uint32_t>(location.x) + 50;
        uint32_t yCoord = static_cast<uint32_t>(location.y) + 50;
        c.pixels[xCoord][yCoord] = Color(1.0f, 1.0f, 1.0f);
    }
    std::ofstream imageFile(fileName, std::ios::out);
    //imageFile.open(fileName, std::ios::out);
    imageFile << c.GetPPMString();
    //imageFile.close();
}
