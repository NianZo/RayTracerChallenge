/*
 * Light.hpp
 *
 *  Created on: Sep 18, 2021
 *      Author: nic
 */

#ifndef SRC_LIGHT_HPP_
#define SRC_LIGHT_HPP_

#include "Color.hpp"
#include "Tuple.hpp"

// Just a point light
class Light
{
  public:
    Tuple position;
    Color intensity;

    Light() : position(Point(0, 0, 0)), intensity(Color(1, 1, 1)){};
    Light(const Tuple& positionIn, const Color& intensityIn) : position(positionIn), intensity(intensityIn){};

    bool operator==(const Light& other) const { return position == other.position && intensity == other.intensity; };
};

#endif /* SRC_LIGHT_HPP_ */
