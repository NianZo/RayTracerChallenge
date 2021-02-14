/*
 * Canvas.cpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#include "Canvas.hpp"

Canvas::Canvas(const int width, const int height) : width(width), height(height)
{
	this->pixels.reserve(height);
	for (int row = 0; row < height; row++)
	{
		this->pixels.push_back(std::vector<Color>());
		this->pixels[row].reserve(width);
		for (int col = 0; col < width; col++)
		{
			this->pixels[row].push_back(Color());
		}
	}
}

std::string Canvas::GetPPMString() const
{
	return "Yolo";
}
