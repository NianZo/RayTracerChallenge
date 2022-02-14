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
#include <limits>

std::vector<std::string_view> tokenizeString(std::string_view textLine, char delimiter);

ObjParser::ObjParser(const std::string& inputData)
{
	if (inputData.size() == 0)
	{
		return;
	}

	Group* currentGroup = &defaultGroup;

	uint64_t oldLinePos = 0;
	uint64_t newLinePos = inputData.find('\n');
	newLinePos = newLinePos == std::string::npos ? inputData.size() : newLinePos;

	while (oldLinePos <= inputData.size())
	{
		std::string_view textLine = std::string_view(inputData).substr(oldLinePos, newLinePos - oldLinePos);

		std::vector<std::string_view> tokens = tokenizeString(textLine, ' ');
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
		} else if (tokens[0] == "vn")
		{
			if (tokens.size() != 4)
			{
				throw std::runtime_error("ObjParser: Unable to parse vertex normal");
			}
			std::array<float, 3> vertexPositions;
			for (uint32_t i = 1; i < 4; i++)
			{
				std::from_chars(tokens[i].begin(), tokens[i].end(), vertexPositions[i - 1]);
			}
			normals.emplace_back(Vector(vertexPositions[0], vertexPositions[1], vertexPositions[2]));
		} else if (tokens[0] == "f")
		{
			if (tokens.size() < 4)
			{
				throw std::runtime_error("ObjParser: Unable to parse face");
			}
			std::vector<std::pair<uint64_t, uint64_t>> vertexIndices(tokens.size() - 1);
			for (uint32_t i = 1; i < tokens.size(); i++)
			{
				std::vector<std::string_view> vertexTokens = tokenizeString(tokens[i], '/');
				if (vertexTokens.size() == 3)
				{
					std::from_chars(vertexTokens[2].begin(), vertexTokens[2].end(), vertexIndices[i - 1].second);
				} else
				{
					vertexIndices[i - 1].second = std::numeric_limits<uint64_t>::max();
				}
				std::from_chars(vertexTokens[0].begin(), vertexTokens[0].end(), vertexIndices[i - 1].first);
			}
			if (vertexIndices[0].second == std::numeric_limits<uint64_t>::max())
			{
				for (uint32_t i = 2; i < vertexIndices.size(); i++)
				{
					currentGroup->addChild(Triangle(vertices[vertexIndices[0].first - 1], vertices[vertexIndices[i - 1].first - 1], vertices[vertexIndices[i].first - 1]));
				}
			} else
			{
				for (uint32_t i = 2; i < vertexIndices.size(); i++)
				{
					currentGroup->addChild(SmoothTriangle(vertices[vertexIndices[0].first - 1], vertices[vertexIndices[i - 1].first - 1], vertices[vertexIndices[i].first - 1],
							normals[vertexIndices[0].second - 1], normals[vertexIndices[i - 1].second - 1], normals[vertexIndices[i].second - 1]));
				}
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

Group ObjParser::getGroup()
{
	Group fullGroup = defaultGroup;
	for (auto& group : namedGroups)
	{
		fullGroup.addChild(group.second);
	}
	return fullGroup;
}

std::vector<std::string_view> tokenizeString(std::string_view textLine, char delimiter)
{
	std::vector<std::string_view> tokens;

	while (!textLine.empty())
	{
		uint64_t delimiterPos = textLine.find(delimiter);
		delimiterPos = delimiterPos == std::string::npos ? textLine.size() : delimiterPos;
		tokens.emplace_back(textLine.substr(0, delimiterPos));
		textLine.remove_prefix(delimiterPos == textLine.size() ? delimiterPos : delimiterPos + 1);
	}

	return tokens;
}


