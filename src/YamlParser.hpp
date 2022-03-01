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
	Camera worldCamera;

	explicit YamlParser(const std::string& inputData);

private:
	enum CommandType
	{
		camera,
		light
	};

	enum SubCommandType
	{
		width,
		height,
		fov,
		from,
		to,
		up,
		at,
		intensity
	};

	CommandType activeCommand;
	std::string activeItemName;

	Tuple cameraFrom;
	Tuple cameraTo;
	Tuple cameraUp;

	void ParseTokens(std::vector<std::string_view>& tokens);
	void SetVectorProperty(SubCommandType subCommandType, const Tuple& value);
	void SetFloatProperty(SubCommandType subCommandType, float value);
	void SetIntProperty(SubCommandType subCommandType, uint32_t value);
};


#endif /* SRC_YAMLPARSER_HPP_ */
