/*
 * YamlParser.cpp
 *
 *  Created on: Feb 27, 2022
 *      Author: nic
 */

#include "YamlParser.hpp"
#include "Transformation.hpp"
#include <charconv>
#include <stdexcept>

std::vector<std::string_view> tokenizeString(std::string_view textLine, char delimiter, bool allowEmptyTokens);

YamlParser::YamlParser([[maybe_unused]] const std::string& inputData) : worldCamera(Camera(100, 100, 0.5, IdentityMatrix()))
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

        std::vector<std::string_view> tokens = tokenizeString(textLine, ' ', false);
        if (!tokens.empty())
        {
            ParseTokens(tokens);
        }

        oldLinePos = newLinePos + 1;
        newLinePos = inputData.find('\n', oldLinePos);
        newLinePos = newLinePos == std::string::npos ? inputData.size() : newLinePos;
    }
}

// std::vector<std::string_view> tokenizeString(std::string_view textLine, char delimiter)
//{
//     std::vector<std::string_view> tokens;
//
//     while (!textLine.empty())
//     {
//         uint64_t delimiterPos = textLine.find(delimiter);
//         delimiterPos = delimiterPos == std::string::npos ? textLine.size() : delimiterPos;
//         tokens.emplace_back(textLine.substr(0, delimiterPos));
//         textLine.remove_prefix(delimiterPos == textLine.size() ? delimiterPos : delimiterPos + 1);
//     }
//
//     return tokens;
// }

Tuple ParseVectorValue(std::string_view x, std::string_view y, std::string_view z)
{
    Tuple value = Vector(0, 0, 0);
    std::from_chars(x.begin(), x.end(), value.x);
    std::from_chars(y.begin(), y.end(), value.y);
    std::from_chars(z.begin(), z.end(), value.z);

    return value;
}

float ParseFloatValue(std::string_view f)
{
    float value;
    std::from_chars(f.begin(), f.end(), value);
    return value;
}

uint32_t ParseIntValue(std::string_view i)
{
    uint32_t value;
    std::from_chars(i.begin(), i.end(), value);
    return value;
}

void YamlParser::ParseTokens(std::vector<std::string_view>& tokens)
{
    if (tokens[0] == "-" && tokens[1] == "add:")
    {
        if (tokens[2].ends_with("camera"))
        {
            activeCommand = CommandType::camera;
        } else if (tokens[2].ends_with("light"))
        {
            activeCommand = CommandType::light;
        }
    }
    if (tokens[0] == "at:")
    {
        if (tokens.size() != 6)
        {
            throw std::runtime_error("'at:' command in invalid format. Expected: 'at: [ x, y, z ]'");
        }
        Tuple position = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
        position.w = 1.0F;
        SetVectorProperty(SubCommandType::at, position);
    } else if (tokens[0] == "intensity:")
    {
        if (tokens.size() != 6)
        {
            throw std::runtime_error("'intensity:' command in invalid format. Expected: 'intensity: [ x, y, z ]'");
        }
        SetVectorProperty(SubCommandType::intensity, ParseVectorValue(tokens[2], tokens[3], tokens[4]));
    } else if (tokens[0] == "width:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'width:' command in invalid format. Expected: 'width: i'");
        }
        SetIntProperty(SubCommandType::width, ParseIntValue(tokens[1]));
    } else if (tokens[0] == "height:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'height:' command in invalid format. Expected: 'height: i'");
        }
        SetIntProperty(SubCommandType::height, ParseIntValue(tokens[1]));
    } else if (tokens[0] == "field-of-view:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'field-of-view:' command in invalid format. Expected: 'field-of-view: f'");
        }
        SetFloatProperty(SubCommandType::fov, ParseFloatValue(tokens[1]));
    } else if (tokens[0] == "from:")
    {
        if (tokens.size() != 6)
        {
            throw std::runtime_error("'from:' command in invalid format. Expected: 'from: [ x, y, z ]'");
        }
        Tuple fromPosition = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
        fromPosition.w = 1.0F;
        SetVectorProperty(SubCommandType::from, fromPosition);
    } else if (tokens[0] == "to:")
    {
        if (tokens.size() != 6)
        {
            throw std::runtime_error("'to:' command in invalid format. Expected: 'to: [ x, y, z ]'");
        }
        Tuple toPosition = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
        toPosition.w = 1.0F;
        SetVectorProperty(SubCommandType::to, toPosition);
    } else if (tokens[0] == "up:")
    {
        if (tokens.size() != 6)
        {
            throw std::runtime_error("'up:' command in invalid format. Expected: 'up: [ x, y, z ]'");
        }
        SetVectorProperty(SubCommandType::up, ParseVectorValue(tokens[2], tokens[3], tokens[4]));
    }
}

void YamlParser::SetVectorProperty(SubCommandType subCommandType, const Tuple& value)
{
    switch (subCommandType)
    {
    case at:
        if (activeCommand != light)
        {
            throw std::runtime_error("Invalid 'at:' specifier for '- add: light' command.");
        }
        world.light.position = value;
        break;
    case intensity:
        if (activeCommand != light)
        {
            throw std::runtime_error("Invalid 'intensity:' specifier for '- add: light' command.");
        }
        world.light.intensity = value;
        break;
    case from:
        if (activeCommand != camera)
        {
            throw std::runtime_error("Invalid 'from:' specifier for '- add: camera' command.");
        }
        cameraFrom = value;
        worldCamera.transform = ViewTransform(cameraFrom, cameraTo, cameraUp);
        worldCamera.RecalculateProperties();
        break;
    case to:
        if (activeCommand != camera)
        {
            throw std::runtime_error("Invalid 'to:' specifier for '- add: camera' command.");
        }
        cameraTo = value;
        worldCamera.transform = ViewTransform(cameraFrom, cameraTo, cameraUp);
        worldCamera.RecalculateProperties();
        break;
    case up:
        if (activeCommand != camera)
        {
            throw std::runtime_error("Invalid 'up:' specifier for '- add: camera' command.");
        }
        cameraUp = value;
        worldCamera.transform = ViewTransform(cameraFrom, cameraTo, cameraUp);
        worldCamera.RecalculateProperties();
        break;
    default:
        throw std::runtime_error("Invalid SubCommandType parsed");
    }
}

void YamlParser::SetFloatProperty(SubCommandType subCommandType, float value)
{
    switch (subCommandType)
    {
    case fov:
        if (activeCommand != camera)
        {
            throw std::runtime_error("Invalid 'field-of-view:' specifier for '- add: camera' command.");
        }
        worldCamera.fov = value;
        worldCamera.RecalculateProperties();
        break;
    default:
        throw std::runtime_error("Invalid SubCommandType parsed");
    }
}

void YamlParser::SetIntProperty(SubCommandType subCommandType, uint32_t value)
{
    switch (subCommandType)
    {
    case width:
        if (activeCommand != camera)
        {
            throw std::runtime_error("Invalid 'width:' specifier for '- add: camera' command.");
        }
        worldCamera.hSize = value;
        worldCamera.RecalculateProperties();
        break;
    case height:
        if (activeCommand != camera)
        {
            throw std::runtime_error("Invalid 'height:' specifier for '- add: camera' command.");
        }
        worldCamera.vSize = value;
        worldCamera.RecalculateProperties();
        break;
    default:
        throw std::runtime_error("Invalid SubCommandType parsed");
    }
}
