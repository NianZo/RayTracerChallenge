/*
 * ObjParser.cpp
 *
 *  Created on: Feb 2, 2022
 *      Author: nic
 */

#include "ObjParser.hpp"
#include <cstdlib>
#include <charconv>

ObjParser::ObjParser(const std::string& inputData) noexcept
{
	uint64_t oldLinePos = 0;
	uint64_t newLinePos = inputData.find('\n');
	while (newLinePos != std::string::npos)
	{
		const std::string_view textLine = inputData.substr(oldLinePos, newLinePos - oldLinePos);
		const uint64_t delimiter1 = textLine.find(' ');

		if (textLine.substr(0, delimiter1 + 1) == "v ")
		{
			const uint64_t delimiter2 = textLine.find(' ', delimiter1);
			//const float xVal = stof(textLine, &delimiter1);
			//const float xVal = std::atof(textLine);//atof(textLine.substr(delimiter1, delimiter2 - delimiter1));
			float xVal;
			std::from_chars(textLine.data() + delimiter1, textLine.data() + delimiter2, xVal);
		} else
		{
			ignoredLines++;
		}

		oldLinePos = newLinePos + 1;
		newLinePos = inputData.find('\n', oldLinePos);
	}
}


