/*
 * Canvas.hpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#ifndef SRC_CANVAS_HPP_
#define SRC_CANVAS_HPP_

#include <vector>
#include <string>
#include "Color.hpp"

class Canvas
{
public:
	const int width;
	const int height;
	std::vector<std::vector<Color>> pixels;

	Canvas(const int width, const int height);
	std::string GetPPMString() const;
};


#endif /* SRC_CANVAS_HPP_ */
