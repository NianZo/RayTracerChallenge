/*
 * YamlParser.hpp
 *
 *  Created on: Feb 27, 2022
 *      Author: nic
 */

#ifndef SRC_YAMLPARSER_HPP_
#define SRC_YAMLPARSER_HPP_

#include "World.hpp"
#include "Camera.hpp"
#include <string>

class YamlParser
{
public:
	World world;
	Camera camera;

	explicit YamlParser(const std::string& inputData);
};


#endif /* SRC_YAMLPARSER_HPP_ */
