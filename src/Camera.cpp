/*
 * Camera.cpp
 *
 *  Created on: Oct 2, 2021
 *      Author: nic
 */

#include "Camera.hpp"

Ray Camera::rayForPixel(const int px, const int py) const
{
	const float xOffset = (px + 0.5) * pixelSize;
	const float yOffset = (py + 0.5) * pixelSize;

	const float worldX = halfWidth - xOffset;
	const float worldY = halfHeight - yOffset;

	const Tuple pixel = transform.inverse() * Point(worldX, worldY, -1);
	const Tuple origin = transform.inverse() * Point(0, 0, 0);
	const Tuple direction = (pixel - origin).normalize();

	return Ray(origin, direction);
}

Canvas Camera::Render(World w) const
{
	Canvas image = Canvas(hSize, vSize);

	for (int i = 0; i < vSize; i++)
	{
		for (int j = 0; j < hSize; j++)
		{
			const Ray r = rayForPixel(j, i);
			const Color c = w.colorAt(r);
			image.pixels[i][j] = c;
		}
	}
	return image;
}
