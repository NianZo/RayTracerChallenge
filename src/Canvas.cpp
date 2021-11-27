/*
 * Canvas.cpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#include "Canvas.hpp"
#include <cmath>

Canvas::Canvas(const uint32_t widthIn, const uint32_t heightIn) : width(widthIn), height(heightIn)
{
    this->pixels.reserve(heightIn);
    for (uint32_t row = 0; row < heightIn; row++)
    {
        this->pixels.push_back(std::vector<Color>());
        this->pixels[row].reserve(widthIn);
        for (uint32_t col = 0; col < widthIn; col++)
        {
            this->pixels[row].push_back(Color());
        }
    }
}

std::string Canvas::GetPPMString() const
{
    // PPM Header
    std::string ppmData = "P3\n" + std::to_string(this->width) + " " + std::to_string(this->height) + "\n255\n";

    for (const auto& row : this->pixels)
    {
        uint64_t charCount = 0;
        for (const auto& pixel : row)
        {
            std::string redString = std::to_string(std::clamp(static_cast<int>(std::round(pixel.r * 255.0f)), 0, 255)) + " ";
            if (charCount + redString.length() > 70)
            {
                ppmData.pop_back();
                ppmData += "\n";
                charCount = 0;
            }
            ppmData += redString;
            charCount += redString.length();

            std::string greenString = std::to_string(std::clamp(static_cast<int>(std::round(pixel.g * 255.0f)), 0, 255)) + " ";
            if (charCount + greenString.length() > 70)
            {
                ppmData.pop_back();
                ppmData += "\n";
                charCount = 0;
            }
            ppmData += greenString;
            charCount += greenString.length();

            std::string blueString = std::to_string(std::clamp(static_cast<int>(std::round(pixel.b * 255.0f)), 0, 255)) + " ";
            if (charCount + blueString.length() > 70)
            {
                ppmData.pop_back();
                ppmData += "\n";
                charCount = 0;
            }
            ppmData += blueString;
            charCount += blueString.length();
        }
        // Remove trailing space and add newline
        ppmData.pop_back();
        ppmData += "\n";
    }
    return ppmData;
}
