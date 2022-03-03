#include "Exercises.hpp"
#include "YamlParser.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>

int main(int argc, char** argv)
{
    if (argc == 6)
    {
        RunSimulation(std::stof(argv[1]), std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]), std::stof(argv[5]));
    } else if (argc == 2)
    {
    	if (std::string(argv[1]).ends_with(".yml"))
    	{
    		std::ifstream yamlFile;
    		yamlFile.open(argv[1]);
    		std::string sceneDescription;
    		std::string line;
    		while (getline(yamlFile, line))
    		{
    			sceneDescription += line + "\n";
    		}
    		yamlFile.close();
    		std::cout << sceneDescription << std::endl;
    		YamlParser parser(sceneDescription);
    		std::cout << "Cubes: " << parser.world.cubes.size() << std::endl;

    	    auto startRenderTime = std::chrono::steady_clock::now();
    	    Canvas canvas = parser.worldCamera.Render(parser.world);
    	    auto endRenderTime = std::chrono::steady_clock::now();

    	    std::cout << "Time to render scene: " << static_cast<std::chrono::duration<double>>(endRenderTime - startRenderTime).count() << std::endl;

    	    std::ofstream imageFile(std::string(argv[1]) + ".ppm", std::ios::out);
    	    //imageFile.open(fileName, std::ios::out);
    	    imageFile << canvas.GetPPMString();
    	} else
    	{
    		//RenderClockFace(argv[1]);
    		RenderChapter7Scene(argv[1]);
    	}
    } else if (argc == 1)
    {
        //RenderSphere("SphereImage.ppm");
        RenderChapter7Scene("Chapter7Scene.ppm");
    } else
    {
        return 1;
    }
    return 0;
}
