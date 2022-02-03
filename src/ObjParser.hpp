/*
 * ObjParser.hpp
 *
 *  Created on: Feb 2, 2022
 *      Author: nic
 */

#ifndef SRC_OBJPARSER_HPP_
#define SRC_OBJPARSER_HPP_

#include <iostream>

class ObjParser
{
public:
	uint32_t ignoredLines = 0;

	ObjParser(const std::string& inputData) noexcept;
};



#endif /* SRC_OBJPARSER_HPP_ */
