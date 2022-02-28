/*
 * YamlParser.cpp
 *
 *  Created on: Feb 27, 2022
 *      Author: nic
 */

#include "YamlParser.hpp"

std::vector<std::string_view> tokenizeString(std::string_view textLine, char delimiter);
void ParseTokens(std::vector<std::string_view>& tokens);

YamlParser::YamlParser([[maybe_unused]] const std::string& inputData) : camera(Camera(100, 100, 0.5, IdentityMatrix()))
{
    if (inputData.empty())
    {
        return;
    }

    uint64_t oldLinePos = 0;
    uint64_t newLinePos = inputData.find('\n');
    newLinePos = newLinePos == std::string::npos ? inputData.size() : newLinePos;

    while (oldLinePos <= inputData.size())
    {
        std::string_view textLine = std::string_view(inputData).substr(oldLinePos, newLinePos - oldLinePos);

        std::vector<std::string_view> tokens = tokenizeString(textLine, ' ');
        if (!tokens.empty())
        {
        	ParseTokens(tokens);
        }

        oldLinePos = newLinePos + 1;
        newLinePos = inputData.find('\n', oldLinePos);
        newLinePos = newLinePos == std::string::npos ? inputData.size() : newLinePos;
    }
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

void ParseTokens(std::vector<std::string_view>& tokens)
{
    if (tokens[0] == "v")
    {
        ParseVertexData(tokens, vertices);
    } else if (tokens[0] == "vn")
    {
        ParseNormalData(tokens, normals);
    } else if (tokens[0] == "f")
    {
        ParseFaceData(tokens, vertices, normals, currentGroup);
    } else if (tokens[0] == "g")
    {
        ParseGroupData(tokens, namedGroups, currentGroup);
    } else
    {
        ignoredLines++;
    }
}
