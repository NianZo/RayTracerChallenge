/*
 * Canvas.hpp
 *
 *  Created on: Feb 13, 2021
 *      Author: nic
 */

#ifndef SRC_CANVAS_HPP_
#define SRC_CANVAS_HPP_

#include "Color.hpp"
#include <string>
#include <vector>

class Canvas
{
  public:
    const uint32_t width;
    const uint32_t height;
    std::vector<std::vector<Color>> pixels;

    Canvas(const uint32_t widthIn, const uint32_t heightIn);
    std::string GetPPMString() const;
};

#endif /* SRC_CANVAS_HPP_ */
