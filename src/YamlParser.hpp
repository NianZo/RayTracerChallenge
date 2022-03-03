/*
 * YamlParser.hpp
 *
 *  Created on: Feb 27, 2022
 *      Author: nic
 */

#ifndef SRC_YAMLPARSER_HPP_
#define SRC_YAMLPARSER_HPP_

#include "Camera.hpp"
#include "World.hpp"
#include <string>

class YamlParser
{
  public:
    World world;
    Camera worldCamera;
    std::unordered_map<std::string, Material> materials;
    std::unordered_map<std::string, Matrix<4>> transforms;

    explicit YamlParser(const std::string& inputData);

  private:
    enum CommandType
    {
        none,
        camera,
        light,
        material,
		transform
    };

    enum SubCommandType
    {
        width,
        height,
        fov,
        from,
        to,
        up,
        at,
        intensity,
        color,
        ambient,
        diffuse,
        specular,
        shininess,
        reflective,
        transparency,
        refractiveIndex
    };

    CommandType activeCommand = none;
    std::string activeItemName;
    Material* currentMaterial = nullptr;
    Matrix<4>* currentTransform = nullptr;

    Tuple cameraFrom;
    Tuple cameraTo;
    Tuple cameraUp;

    void ParseTokens(std::vector<std::string_view>& tokens);
    void SetVectorProperty(SubCommandType subCommandType, const Tuple& value);
    void SetFloatProperty(SubCommandType subCommandType, float value);
    void SetIntProperty(SubCommandType subCommandType, uint32_t value);
    void ParseCommandAt(std::vector<std::string_view>& tokens);
    void ParseCommandIntensity(std::vector<std::string_view>& tokens);
    void ParseCommandWidth(std::vector<std::string_view>& tokens);
    void ParseCommandHeight(std::vector<std::string_view>& tokens);
    void ParseCommandFOV(std::vector<std::string_view>& tokens);
    void ParseCommandFrom(std::vector<std::string_view>& tokens);
    void ParseCommandTo(std::vector<std::string_view>& tokens);
    void ParseCommandUp(std::vector<std::string_view>& tokens);
    void ParseCommandAdd(std::vector<std::string_view>& tokens);
};

#endif /* SRC_YAMLPARSER_HPP_ */