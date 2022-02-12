/*
 * ObjParser.cpp
 *
 *  Created on: Feb 2, 2022
 *      Author: nic
 */

#include "ObjParser.hpp"
#include <cstdlib>
#include <charconv>
#include <array>

std::vector<std::string_view> tokenizeString(std::string_view textLine);

ObjParser::ObjParser(const std::string& inputData)
{
	if (inputData.size() == 0)
	{
		return;
	}

	currentGroup = &defaultGroup;

	uint64_t oldLinePos = 0;
	uint64_t newLinePos = inputData.find('\n');
	newLinePos = newLinePos == std::string::npos ? inputData.size() : newLinePos;

	while (oldLinePos <= inputData.size())
	{
		std::string_view textLine = std::string_view(inputData).substr(oldLinePos, newLinePos - oldLinePos);

		std::vector<std::string_view> tokens = tokenizeString(textLine);
		if (tokens.size() == 0)
		{
			ignoredLines++;
		} else if (tokens[0] == "v")
		{
			if (tokens.size() != 4)
			{
				throw std::runtime_error("ObjParser: Unable to parse vertex");
			}
			std::array<float, 3> vertexPositions;
			for (uint32_t i = 1; i < 4; i++)
			{
				std::from_chars(tokens[i].begin(), tokens[i].end(), vertexPositions[i - 1]);
			}
			vertices.emplace_back(Point(vertexPositions[0], vertexPositions[1], vertexPositions[2]));
		} else if (tokens[0] == "f")
		{
			if (tokens.size() < 4)
			{
				throw std::runtime_error("ObjParser: Unable to parse face");
			}
			std::vector<uint64_t> vertexIndices(tokens.size() - 1);
			for (uint32_t i = 1; i < tokens.size(); i++)
			{
				std::from_chars(tokens[i].begin(), tokens[i].end(), vertexIndices[i - 1]);
			}
			for (uint32_t i = 2; i < vertexIndices.size(); i++)
			{
				currentGroup->addChild(Triangle(vertices[vertexIndices[0] - 1], vertices[vertexIndices[i - 1] - 1], vertices[vertexIndices[i] - 1]));
			}
		} else if (tokens[0] == "g")
		{
			if (tokens.size() < 2)
			{
				throw std::runtime_error("ObjParser: Unable to parse group name");
			}
			namedGroups.emplace(tokens[1], Group());
			currentGroup = &namedGroups[std::string(tokens[1].data(), tokens[1].size())];
		} else
		{
			ignoredLines++;
		}

		oldLinePos = newLinePos + 1;
		newLinePos = inputData.find('\n', oldLinePos);
		newLinePos = newLinePos == std::string::npos ? inputData.size() : newLinePos;
	}
}

std::vector<std::string_view> tokenizeString(std::string_view textLine)
{
	std::vector<std::string_view> tokens;

	while (!textLine.empty())
	{
		uint64_t delimiterPos = textLine.find(' ');
		delimiterPos = delimiterPos == std::string::npos ? textLine.size() : delimiterPos;
		tokens.emplace_back(textLine.substr(0, delimiterPos));
		textLine.remove_prefix(delimiterPos == textLine.size() ? delimiterPos : delimiterPos + 1);
	}

	return tokens;
}


