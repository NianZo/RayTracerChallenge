/*
 * Exercises.hpp
 *
 *  Created on: Nov 24, 2021
 *      Author: nic
 */

#ifndef EXERCISES_EXERCISES_HPP_
#define EXERCISES_EXERCISES_HPP_

#include <string>

void RunSimulation(const float gravity, const float wind, const float startingHeight, const float startingXVelocity, const float deltaTime);
void RenderClockFace(const std::string& fileName);
void RenderSphere(const std::string& fileName);
void RenderChapter7Scene(const std::string& fileName);

#endif /* EXERCISES_EXERCISES_HPP_ */
