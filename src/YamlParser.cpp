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
#include <functional>

std::vector<std::string_view> tokenizeString(std::string_view textLine, char delimiter, bool allowEmptyTokens);

YamlParser::YamlParser([[maybe_unused]] const std::string& inputData) : worldCamera(Camera(100, 100, 0.5, IdentityMatrix()))
{
    if (inputData.empty())
    {
        return;
    }

    using std::placeholders::_1;
    subCommandMap.emplace("at:", std::bind(&YamlParser::ParseCommandAt, this, _1));
    subCommandMap.emplace("intensity:", std::bind(&YamlParser::ParseCommandIntensity, this, _1));
    subCommandMap.emplace("width:", std::bind(&YamlParser::ParseCommandWidth, this, _1));
    subCommandMap.emplace("height:", std::bind(&YamlParser::ParseCommandHeight, this, _1));
    subCommandMap.emplace("field-of-view:", std::bind(&YamlParser::ParseCommandFOV, this, _1));
    subCommandMap.emplace("from:", std::bind(&YamlParser::ParseCommandFrom, this, _1));
    subCommandMap.emplace("to:", std::bind(&YamlParser::ParseCommandTo, this, _1));
    subCommandMap.emplace("up:", std::bind(&YamlParser::ParseCommandUp, this, _1));
    subCommandMap.emplace("color:", std::bind(&YamlParser::ParseCommandColor, this, _1));
    subCommandMap.emplace("ambient:", std::bind(&YamlParser::ParseCommandAmbient, this, _1));
    subCommandMap.emplace("diffuse:", std::bind(&YamlParser::ParseCommandDiffuse, this, _1));
    subCommandMap.emplace("specular:", std::bind(&YamlParser::ParseCommandSpecular, this, _1));
    subCommandMap.emplace("shininess:", std::bind(&YamlParser::ParseCommandShininess, this, _1));
    subCommandMap.emplace("reflective:", std::bind(&YamlParser::ParseCommandReflective, this, _1));
    subCommandMap.emplace("transparency:", std::bind(&YamlParser::ParseCommandTransparency, this, _1));
    subCommandMap.emplace("refractive-index:", std::bind(&YamlParser::ParseCommandRefractiveIndex, this, _1));
    subCommandMap.emplace("extend:", std::bind(&YamlParser::ParseCommandExtend, this, _1));

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
    if (activeCommand != light)
    {
        throw std::runtime_error("Invalid 'at:' specifier for '- add: light' command.");
    }
    world.light.position = position;
}

void YamlParser::ParseCommandIntensity(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 6)
    {
        throw std::runtime_error("'intensity:' command in invalid format. Expected: 'intensity: [ x, y, z ]'");
    }
    if (activeCommand != light)
    {
        throw std::runtime_error("Invalid 'intensity:' specifier for '- add: light' command.");
    }
    world.light.intensity = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
}

void YamlParser::ParseCommandWidth(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 2)
    {
        throw std::runtime_error("'width:' command in invalid format. Expected: 'width: i'");
    }
    if (activeCommand != camera)
    {
        throw std::runtime_error("Invalid 'width:' specifier for '- add: camera' command.");
    }
    worldCamera.hSize = ParseIntValue(tokens[1]);
    worldCamera.RecalculateProperties();
}

void YamlParser::ParseCommandHeight(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 2)
    {
        throw std::runtime_error("'height:' command in invalid format. Expected: 'height: i'");
    }
    if (activeCommand != camera)
    {
        throw std::runtime_error("Invalid 'height:' specifier for '- add: camera' command.");
    }
    worldCamera.vSize = ParseIntValue(tokens[1]);
    worldCamera.RecalculateProperties();
}

void YamlParser::ParseCommandFOV(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 2)
    {
        throw std::runtime_error("'field-of-view:' command in invalid format. Expected: 'field-of-view: f'");
    }
    if (activeCommand != camera)
    {
        throw std::runtime_error("Invalid 'field-of-view:' specifier for '- add: camera' command.");
    }
    worldCamera.fov = ParseFloatValue(tokens[1]);
    worldCamera.RecalculateProperties();
}

void YamlParser::ParseCommandFrom(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 6)
    {
        throw std::runtime_error("'from:' command in invalid format. Expected: 'from: [ x, y, z ]'");
    }
    Tuple fromPosition = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
    fromPosition.w = 1.0F;
    if (activeCommand != camera)
    {
        throw std::runtime_error("Invalid 'from:' specifier for '- add: camera' command.");
    }
    cameraFrom = fromPosition;
    worldCamera.transform = ViewTransform(cameraFrom, cameraTo, cameraUp);
    worldCamera.RecalculateProperties();
}

void YamlParser::ParseCommandTo(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 6)
    {
        throw std::runtime_error("'to:' command in invalid format. Expected: 'to: [ x, y, z ]'");
    }
    Tuple toPosition = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
    toPosition.w = 1.0F;
    if (activeCommand != camera)
    {
        throw std::runtime_error("Invalid 'to:' specifier for '- add: camera' command.");
    }
    cameraTo = toPosition;
    worldCamera.transform = ViewTransform(cameraFrom, cameraTo, cameraUp);
    worldCamera.RecalculateProperties();
}

void YamlParser::ParseCommandUp(std::vector<std::string_view>& tokens)
{
    if (tokens.size() != 6)
    {
        throw std::runtime_error("'up:' command in invalid format. Expected: 'up: [ x, y, z ]'");
    }
    if (activeCommand != camera)
    {
        throw std::runtime_error("Invalid 'up:' specifier for '- add: camera' command.");
    }
    cameraUp = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
    worldCamera.transform = ViewTransform(cameraFrom, cameraTo, cameraUp);
    worldCamera.RecalculateProperties();
}

void YamlParser::ParseCommandAdd(std::vector<std::string_view>& tokens)
{
    if (tokens[2].ends_with("camera"))
    {
        activeCommand = CommandType::camera;
    } else if (tokens[2].ends_with("light"))
    {
        activeCommand = CommandType::light;
    } else if (tokens[2] == "plane")
    {
        activeCommand = CommandType::plane;
        world.planes.emplace_back(Plane());
        activeMaterial = &world.planes.back().material;
        activeTransform = &world.planes.back().transform;
    } else if (tokens[2] == "cube")
    {
        activeCommand = CommandType::cube;
        world.cubes.emplace_back(Cube());
        activeMaterial = &world.cubes.back().material;
        activeTransform = &world.cubes.back().transform;
    } else if (tokens[2] == "sphere")
    {
        activeCommand = CommandType::sphere;
        world.spheres.emplace_back(Sphere());
        activeMaterial = &world.spheres.back().material;
        activeTransform = &world.spheres.back().transform;
    }
}

void YamlParser::ParseCommandDefine(std::vector<std::string_view> &tokens) {
	if (tokens[2].ends_with("material")) {
		activeCommand = CommandType::material;
		activeItemName = tokens[2];
		materials.emplace(activeItemName, Material());
		activeMaterial = &materials[activeItemName];
	} else if (tokens[2].ends_with("transform")) {
		activeCommand = CommandType::transform;
		activeItemName = tokens[2];
		transforms.emplace(activeItemName, IdentityMatrix());
		activeTransform = &transforms[activeItemName];
	}
}

void YamlParser::ParseCommandTransformParameter(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 7) {
		throw std::runtime_error("Transform parameter command in invalid format. Expected: '- [ op, x, y, z ]'");
	}
	if (activeCommand != transform && activeCommand != plane && activeCommand != cube && activeCommand != sphere) {
		throw std::runtime_error("Transform parameter only valid for transform definition.");
	}
	if (tokens[2] == "translate,") {
		*activeTransform = translation(ParseFloatValue(tokens[3]),
				ParseFloatValue(tokens[4]), ParseFloatValue(tokens[5]))
				* *activeTransform;
	} else if (tokens[2] == "scale,") {
		*activeTransform = scaling(ParseFloatValue(tokens[3]),
				ParseFloatValue(tokens[4]), ParseFloatValue(tokens[5]))
				* *activeTransform;
	} else if (tokens[2] == "rotate,") {
		*activeTransform = rotationZ(ParseFloatValue(tokens[5]))
				* rotationY(ParseFloatValue(tokens[4]))
				* rotationX(ParseFloatValue(tokens[3])) * *activeTransform;
	} else {
		throw std::runtime_error("Invalid transform operation. Expected: 'scale', 'rotate', or 'translate'");
	}
}

void YamlParser::ParseCommandColor(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 6) {
		throw std::runtime_error("'color:' command in invalid format. Expected: 'color: [ x, y, z ]'");
	}
    if (activeCommand != material && activeCommand != plane && activeCommand != cube && activeCommand != sphere)
    {
        throw std::runtime_error("Invalid 'color:' specifier for '- define: material' command.");
    }
    activeMaterial->color = ParseVectorValue(tokens[2], tokens[3], tokens[4]);
}

void YamlParser::ParseCommandAmbient(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 2) {
		throw std::runtime_error("'ambient:' command in invalid format. Expected: 'ambient: f'");
	}
    if (activeCommand != material && activeCommand != plane && activeCommand != cube && activeCommand != sphere)
    {
        throw std::runtime_error("Invalid 'ambient:' specifier for '- define: material' command.");
    }
    activeMaterial->ambient = ParseFloatValue(tokens[1]);
}

void YamlParser::ParseCommandDiffuse(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 2) {
		throw std::runtime_error("'diffuse:' command in invalid format. Expected: 'diffuse: f'");
	}
    if (activeCommand != material && activeCommand != plane && activeCommand != cube && activeCommand != sphere)
    {
        throw std::runtime_error("Invalid 'diffuse:' specifier for '- define: material' command.");
    }
    activeMaterial->diffuse = ParseFloatValue(tokens[1]);
}

void YamlParser::ParseCommandSpecular(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 2) {
		throw std::runtime_error("'specular:' command in invalid format. Expected: 'specular: f'");
	}
    if (activeCommand != material && activeCommand != plane && activeCommand != cube && activeCommand != sphere)
    {
        throw std::runtime_error("Invalid 'specular:' specifier for '- define: material' command.");
    }
    activeMaterial->specular = ParseFloatValue(tokens[1]);
}

void YamlParser::ParseCommandShininess(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 2) {
		throw std::runtime_error("'shininess:' command in invalid format. Expected: 'shininess: f'");
	}
    if (activeCommand != material && activeCommand != plane && activeCommand != cube && activeCommand != sphere)
    {
        throw std::runtime_error("Invalid 'shininess:' specifier for '- define: material' command.");
    }
    activeMaterial->shininess = ParseFloatValue(tokens[1]);
}

void YamlParser::ParseCommandReflective(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 2) {
		throw std::runtime_error("'reflective:' command in invalid format. Expected: 'reflective: f'");
	}
    if (activeCommand != material && activeCommand != plane && activeCommand != cube && activeCommand != sphere)
    {
        throw std::runtime_error("Invalid 'reflective:' specifier for '- define: material' command.");
    }
    activeMaterial->reflectivity = ParseFloatValue(tokens[1]);
}

void YamlParser::ParseCommandTransparency(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 2) {
		throw std::runtime_error("'transparency:' command in invalid format. Expected: 'transparency: f'");
	}
    if (activeCommand != material && activeCommand != plane && activeCommand != cube && activeCommand != sphere)
    {
        throw std::runtime_error("Invalid 'transparency:' specifier for '- define: material' command.");
    }
    activeMaterial->transparency = ParseFloatValue(tokens[1]);
}

void YamlParser::ParseCommandRefractiveIndex(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 2) {
		throw std::runtime_error("'refractive-index:' command in invalid format. Expected: 'refractive-index: f'");
	}
    if (activeCommand != material && activeCommand != plane && activeCommand != cube && activeCommand != sphere)
    {
        throw std::runtime_error("Invalid 'refractive-index:' specifier for '- define: material' command.");
    }
    activeMaterial->refractiveIndex = ParseFloatValue(tokens[1]);
}

void YamlParser::ParseCommandExtend(std::vector<std::string_view> &tokens) {
	if (tokens.size() != 2) {
		throw std::runtime_error("'extend:' command in invalid format. Expected: 'extend: name'");
	}
	switch (activeCommand) {
	case material:
		*activeMaterial = materials[std::string(tokens[1])];
		break;
	case transform:
		*activeTransform = transforms[std::string(tokens[1])];
		break;
	default:
		throw std::runtime_error("'extend:' option must be used with: material or transform command.");
	}
}

void YamlParser::ParseTokens(std::vector<std::string_view>& tokens)
{
    if (tokens.size() == 3 && tokens[0] == "-" && tokens[1] == "add:")
    {
        ParseCommandAdd(tokens);
    } else if (tokens.size() == 3 && tokens[0] == "-" && tokens[1] == "define:")
    {
		ParseCommandDefine(tokens);
    } else if (tokens.size() > 1 && tokens[0] == "-" && tokens[1] == "[")
    {
		ParseCommandTransformParameter(tokens);
    } else if (tokens[0] == "material:" && tokens.size() == 2)
    {
        *activeMaterial = materials[std::string(tokens[1])];
    } else if (tokens[0] == "transform:" && tokens.size() == 2)
    {
        *activeTransform = transforms[std::string(tokens[1])];
    } else if (subCommandMap.contains(std::string(tokens[0])))
    {
    	subCommandMap[std::string(tokens[0])](tokens);
    }
}


