/*
 * ObjParser.cpp
 *
 *  Created on: Feb 2, 2022
 *      Author: nic
 */

#include "ObjParser.hpp"

ObjParser::ObjParser(const std::string& inputData) noexcept
{
	uint64_t oldLinePos = 0;
	uint64_t newLinePos = inputData.find('\n');
	while (newLinePos != std::string::npos)
	{
		//const std::string_view textLine = inputData.substr(oldLinePos, newLinePos - oldLinePos);

		// Unrecognized line
		ignoredLines++;

		oldLinePos = newLinePos;
		newLinePos = inputData.find('\n', oldLinePos);
	}
}


