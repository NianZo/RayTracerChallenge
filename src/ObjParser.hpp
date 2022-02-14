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
#include <unordered_map>

class ObjParser
{
public:
	std::vector<Tuple> vertices;
	std::vector<Tuple> normals;
	std::unordered_map<std::string, Group> namedGroups;
	Group defaultGroup;
	uint32_t ignoredLines = 0;

	ObjParser(const std::string& inputData);
	Group getGroup();
};



#endif /* SRC_OBJPARSER_HPP_ */
