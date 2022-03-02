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
    float value = NAN;
    std::from_chars(f.begin(), f.end(), value);
    return value;
}

uint32_t ParseIntValue(std::string_view i)
{
    uint32_t value = 0;
    std::from_chars(i.begin(), i.end(), value);
    return value;
}

void YamlParser::ParseCommandAt(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 6)
    {
        throw std::runtime_error("'at:' command in invalid format. Expected: 'at: [ x, y, z ]'");
    }
    Tuple position = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
    position.w = 1.0F;
    SetVectorProperty(SubCommandType::at, position);
}

void YamlParser::ParseCommandIntensity(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 6)
    {
        throw std::runtime_error("'intensity:' command in invalid format. Expected: 'intensity: [ x, y, z ]'");
    }
    SetVectorProperty(SubCommandType::intensity, ParseVectorValue(tokens[2], tokens[3], tokens[4]));
}

void YamlParser::ParseCommandWidth(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 2)
    {
        throw std::runtime_error("'width:' command in invalid format. Expected: 'width: i'");
    }
    SetIntProperty(SubCommandType::width, ParseIntValue(tokens[1]));
}

void YamlParser::ParseCommandHeight(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 2)
    {
        throw std::runtime_error("'height:' command in invalid format. Expected: 'height: i'");
    }
    SetIntProperty(SubCommandType::height, ParseIntValue(tokens[1]));
}

void YamlParser::ParseCommandFOV(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 2)
    {
        throw std::runtime_error("'field-of-view:' command in invalid format. Expected: 'field-of-view: f'");
    }
    SetFloatProperty(SubCommandType::fov, ParseFloatValue(tokens[1]));
}

void YamlParser::ParseCommandFrom(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 6)
    {
        throw std::runtime_error("'from:' command in invalid format. Expected: 'from: [ x, y, z ]'");
    }
    Tuple fromPosition = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
    fromPosition.w = 1.0F;
    SetVectorProperty(SubCommandType::from, fromPosition);
}

void YamlParser::ParseCommandTo(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 6)
    {
        throw std::runtime_error("'to:' command in invalid format. Expected: 'to: [ x, y, z ]'");
    }
    Tuple toPosition = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
    toPosition.w = 1.0F;
    SetVectorProperty(SubCommandType::to, toPosition);
}

void YamlParser::ParseCommandUp(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 6)
    {
        throw std::runtime_error("'up:' command in invalid format. Expected: 'up: [ x, y, z ]'");
    }
    SetVectorProperty(SubCommandType::up, ParseVectorValue(tokens[2], tokens[3], tokens[4]));
}

void YamlParser::ParseCommandAdd(std::vector<std::string_view>& tokens)
{
    if (tokens[2].ends_with("camera"))
    {
        activeCommand = CommandType::camera;
    } else if (tokens[2].ends_with("light"))
    {
        activeCommand = CommandType::light;
    }
}

void YamlParser::ParseTokens(std::vector<std::string_view>& tokens)
{
    if (tokens[0] == "-" && tokens[1] == "add:")
    {
        ParseCommandAdd(tokens);
    } else if (tokens[0] == "-" && tokens[1] == "define:")
    {
        if (tokens[2].ends_with("material"))
        {
            activeCommand = CommandType::material;
            activeItemName = tokens[2];
            materials.emplace(activeItemName, Material());
        }
    } else if (tokens[0] == "at:")
    {
        ParseCommandAt(tokens);
    } else if (tokens[0] == "intensity:")
    {
        ParseCommandIntensity(tokens);
    } else if (tokens[0] == "width:")
    {
        ParseCommandWidth(tokens);
    } else if (tokens[0] == "height:")
    {
        ParseCommandHeight(tokens);
    } else if (tokens[0] == "field-of-view:")
    {
        ParseCommandFOV(tokens);
    } else if (tokens[0] == "from:")
    {
        ParseCommandFrom(tokens);
    } else if (tokens[0] == "to:")
    {
        ParseCommandTo(tokens);
    } else if (tokens[0] == "up:")
    {
        ParseCommandUp(tokens);
    } else if (tokens[0] == "color:")
    {
        if (tokens.size() != 6)
        {
            throw std::runtime_error("'color:' command in invalid format. Expected: 'color: [ x, y, z ]'");
        }
        SetVectorProperty(SubCommandType::color, ParseVectorValue(tokens[2], tokens[3], tokens[4]));
    } else if (tokens[0] == "ambient:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'ambient:' command in invalid format. Expected: 'ambient: f'");
        }
        SetFloatProperty(SubCommandType::ambient, ParseFloatValue(tokens[1]));
    } else if (tokens[0] == "diffuse:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'diffuse:' command in invalid format. Expected: 'diffuse: f'");
        }
        SetFloatProperty(SubCommandType::diffuse, ParseFloatValue(tokens[1]));
    } else if (tokens[0] == "specular:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'specular:' command in invalid format. Expected: 'diffuse: f'");
        }
        SetFloatProperty(SubCommandType::specular, ParseFloatValue(tokens[1]));
    } else if (tokens[0] == "shininess:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'shininess:' command in invalid format. Expected: 'diffuse: f'");
        }
        SetFloatProperty(SubCommandType::shininess, ParseFloatValue(tokens[1]));
    } else if (tokens[0] == "reflective:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'reflective:' command in invalid format. Expected: 'diffuse: f'");
        }
        SetFloatProperty(SubCommandType::reflective, ParseFloatValue(tokens[1]));
    } else if (tokens[0] == "transparency:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'transparency:' command in invalid format. Expected: 'diffuse: f'");
        }
        SetFloatProperty(SubCommandType::transparency, ParseFloatValue(tokens[1]));
    } else if (tokens[0] == "refractive-index:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'refractive-index:' command in invalid format. Expected: 'diffuse: f'");
        }
        SetFloatProperty(SubCommandType::refractiveIndex, ParseFloatValue(tokens[1]));
    } else if (tokens[0] == "extend:")
    {
        if (tokens.size() != 2)
        {
            throw std::runtime_error("'extend:' command in invalid format. Expected: 'extend: name'");
        }
        switch (activeCommand)
        {
        case material:
            materials[activeItemName] = materials[std::string(tokens[1])];
            break;
        default:
            throw std::runtime_error("'extend:' option must be used with: material or transform command.");
        }
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
    case color:
        if (activeCommand != material)
        {
            throw std::runtime_error("Invalid 'color:' specifier for '- define: material' command.");
        }
        materials[activeItemName].color = value;
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
    case ambient:
        if (activeCommand != material)
        {
            throw std::runtime_error("Invalid 'ambient:' specifier for '- define: material' command.");
        }
        materials[activeItemName].ambient = value;
        break;
    case diffuse:
        if (activeCommand != material)
        {
            throw std::runtime_error("Invalid 'diffuse:' specifier for '- define: material' command.");
        }
        materials[activeItemName].diffuse = value;
        break;
    case specular:
        if (activeCommand != material)
        {
            throw std::runtime_error("Invalid 'specular:' specifier for '- define: material' command.");
        }
        materials[activeItemName].specular = value;
        break;
    case shininess:
        if (activeCommand != material)
        {
            throw std::runtime_error("Invalid 'shininess:' specifier for '- define: material' command.");
        }
        materials[activeItemName].shininess = value;
        break;
    case reflective:
        if (activeCommand != material)
        {
            throw std::runtime_error("Invalid 'reflective:' specifier for '- define: material' command.");
        }
        materials[activeItemName].reflectivity = value;
        break;
    case transparency:
        if (activeCommand != material)
        {
            throw std::runtime_error("Invalid 'transparency:' specifier for '- define: material' command.");
        }
        materials[activeItemName].transparency = value;
        break;
    case refractiveIndex:
        if (activeCommand != material)
        {
            throw std::runtime_error("Invalid 'refractive-index:' specifier for '- define: material' command.");
        }
        materials[activeItemName].refractiveIndex = value;
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
