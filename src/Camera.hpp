/*
 * Camera.hpp
 *
 *  Created on: Sep 26, 2021
 *      Author: nic
 */

#ifndef SRC_CAMERA_HPP_
#define SRC_CAMERA_HPP_

#include "Matrix.hpp"
#include "Ray.hpp"
#include "World.hpp"
#include "Canvas.hpp"

#include <cmath>

class Camera
{
public:
	const int hSize;
	const int vSize;
	const float fov;
	Matrix<4> transform;
	float pixelSize;
	float halfWidth;
	float halfHeight;

	Camera(const int hSize, const int vSize, const float fov) : hSize(hSize), vSize(vSize), fov(fov), transform(IdentityMatrix())
	{
		const float halfView = std::tan(fov / 2);
		const float aspectRatio = static_cast<float>(hSize) / vSize;
		halfWidth = hSize >= vSize ? halfView : halfView * aspectRatio;
		halfHeight = hSize < vSize ? halfView : halfView / aspectRatio;

		pixelSize = halfWidth * 2 / hSize;
	};

	Ray rayForPixel(const int x, const int y) const;
	Canvas Render(World w) const;
};


#endif /* SRC_CAMERA_HPP_ */
