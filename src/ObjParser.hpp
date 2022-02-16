/*
 * ObjParser.hpp
 *
 *  Created on: Feb 2, 2022
 *      Author: nic
 */

#ifndef SRC_OBJPARSER_HPP_
#define SRC_OBJPARSER_HPP_

#include "Shape.hpp"
#include "Tuple.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

class ObjParser
{
  public:
    std::vector<Tuple> vertices;
    std::vector<Tuple> normals;
    std::unordered_map<std::string, Group> namedGroups;
    Group defaultGroup;
    uint32_t ignoredLines = 0;

    explicit ObjParser(const std::string& inputData);
    Group getGroup();

  private:
    void ParseTokens(std::vector<std::string_view>& tokens, Group*& currentGroup);
};

#endif /* SRC_OBJPARSER_HPP_ */
