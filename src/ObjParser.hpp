/*
 * ObjParser.hpp
 *
 *  Created on: Feb 2, 2022
 *      Author: nic
 */

#ifndef SRC_OBJPARSER_HPP_
#define SRC_OBJPARSER_HPP_

#include "Tuple.hpp"
#include "Shape.hpp"
#include <iostream>
#include <vector>

class ObjParser
{
public:
	std::vector<Tuple> vertices;
	Group defaultGroup;
	uint32_t ignoredLines = 0;

	ObjParser(const std::string& inputData);
};



#endif /* SRC_OBJPARSER_HPP_ */
