/*
 * Camera.cpp
 *
 *  Created on: Oct 2, 2021
 *      Author: nic
 */

#include "Camera.hpp"
#include <omp.h>

Ray Camera::rayForPixel(const uint32_t px, const uint32_t py) const noexcept
{
    const float xOffset = (static_cast<float>(px) + 0.5f) * pixelSize;
    const float yOffset = (static_cast<float>(py) + 0.5f) * pixelSize;

    const float worldX = halfWidth - xOffset;
    const float worldY = halfHeight - yOffset;

    const Tuple pixel = transform.inverse() * Point(worldX, worldY, -1);
    const Tuple origin = transform.inverse() * Point(0, 0, 0);
    const Tuple direction = (pixel - origin).normalize();

    return Ray(origin, direction);
}

Canvas Camera::Render(const World& w) const noexcept
{
    Canvas image = Canvas(hSize, vSize);

#pragma omp parallel for
    for (uint32_t i = 0; i < vSize; i++)
    {
        for (uint32_t j = 0; j < hSize; j++)
        {
            const Ray r = rayForPixel(j, i);
            const Color c = w.colorAt(r);
#pragma omp critical
            image.pixels[i][j] = c;
        }
    }
    return image;
}
