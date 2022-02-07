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

ObjParser::ObjParser(const std::string& inputData) noexcept
{
	if (inputData.size() == 0)
	{
		return;
	}

	uint64_t oldLinePos = 0;
	uint64_t newLinePos = inputData.find('\n');
	newLinePos = newLinePos == std::string::npos ? inputData.size() : newLinePos;

	while (oldLinePos <= inputData.size())
	{
		std::string_view textLine = std::string_view(inputData).substr(oldLinePos, newLinePos - oldLinePos);
		//const uint64_t delimiter1 = textLine.find(' ');

		if (textLine.starts_with("v "))
		{
			uint64_t delimiter = 1; //textLine.remove_prefix(2);
			std::array<float, 3> vertexPositions;
			for (uint32_t i = 0; i < 3; i++)
			{
				textLine.remove_prefix(delimiter + 1);
				delimiter = textLine.find(' ');
				delimiter = delimiter == std::string::npos ? textLine.size() : delimiter;
				std::from_chars(textLine.data(), textLine.data() + delimiter, vertexPositions[i]);

			}
//			const uint64_t delimiter2 = textLine.find(' ', delimiter1 + 1);
//			//const float xVal = stof(textLine, &delimiter1);
//			//const float xVal = std::atof(textLine);//atof(textLine.substr(delimiter1, delimiter2 - delimiter1));
//			float xVal;
//			std::from_chars(textLine.data() + delimiter1 + 1, textLine.data() + delimiter2, xVal);
//
//			const uint64_t delimiter3 = textLine.find(' ', delimiter2 + 1);
//			float yVal;
//			std::from_chars(textLine.data() + delimiter2 + 1, textLine.data() + delimiter3, yVal);
//
//			float zVal;
//			std::from_chars(textLine.data() + delimiter3 + 1, textLine.data() + textLine.size(), zVal);

			//vertices.emplace_back(Point(xVal, yVal, zVal));
			vertices.emplace_back(Point(vertexPositions[0], vertexPositions[1], vertexPositions[2]));
		} else
		{
			ignoredLines++;
		}

		oldLinePos = newLinePos + 1;
		newLinePos = inputData.find('\n', oldLinePos);
		newLinePos = newLinePos == std::string::npos ? inputData.size() : newLinePos;
	}
}


