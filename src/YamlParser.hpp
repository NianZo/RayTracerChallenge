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
        transform,
        plane,
        cube,
        sphere
    };

    std::unordered_map<std::string, std::function<void(const std::vector<std::string_view>&)>> subCommandMap;

    CommandType activeCommand = none;
    std::string activeItemName;
    Material* activeMaterial = nullptr;
    Matrix<4>* activeTransform = nullptr;

    Tuple cameraFrom;
    Tuple cameraTo;
    Tuple cameraUp;

    void ParseTokens(const std::vector<std::string_view>& tokens);
    void ParseCommandAt(const std::vector<std::string_view>& tokens);
    void ParseCommandIntensity(const std::vector<std::string_view>& tokens);
    void ParseCommandWidth(const std::vector<std::string_view>& tokens);
    void ParseCommandHeight(const std::vector<std::string_view>& tokens);
    void ParseCommandFOV(const std::vector<std::string_view>& tokens);
    void ParseCommandFrom(const std::vector<std::string_view>& tokens);
    void ParseCommandTo(const std::vector<std::string_view>& tokens);
    void ParseCommandUp(const std::vector<std::string_view>& tokens);
    void ParseCommandAdd(const std::vector<std::string_view>& tokens);
    void ParseCommandDefine(const std::vector<std::string_view>& tokens);
    void ParseCommandTransformParameter(const std::vector<std::string_view>& tokens);
    void ParseCommandColor(const std::vector<std::string_view>& tokens);
    void ParseCommandAmbient(const std::vector<std::string_view>& tokens);
    void ParseCommandDiffuse(const std::vector<std::string_view>& tokens);
    void ParseCommandSpecular(const std::vector<std::string_view>& tokens);
    void ParseCommandShininess(const std::vector<std::string_view>& tokens);
    void ParseCommandReflective(const std::vector<std::string_view>& tokens);
    void ParseCommandTransparency(const std::vector<std::string_view>& tokens);
    void ParseCommandRefractiveIndex(const std::vector<std::string_view>& tokens);
    void ParseCommandExtend(const std::vector<std::string_view>& tokens);
};

#endif /* SRC_YAMLPARSER_HPP_ */
