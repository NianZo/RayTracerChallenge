#include <iostream>
#include <cstdlib>
#include "BallisticsSimulator.cpp"
#include "ClockFace.cpp"


int main(int argc, char **argv) {
	if (argc == 6)
	{
		RunSimulation(std::stof(argv[1]), std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]), std::stof(argv[5]));
	} else if (argc == 2)
	{
		RenderClockFace(argv[1]);
	} else
	{
		return 1;
	}
	return 0;
}
