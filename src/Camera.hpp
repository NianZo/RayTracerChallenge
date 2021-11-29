/*
 * Camera.hpp
 *
 *  Created on: Sep 26, 2021
 *      Author: nic
 */

#ifndef SRC_CAMERA_HPP_
#define SRC_CAMERA_HPP_

#include "Canvas.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "World.hpp"

#include <cmath>

class Camera
{
  public:
    const uint32_t hSize;
    const uint32_t vSize;
    const float fov;
    Matrix<4> transform;
    float pixelSize;
    float halfWidth;
    float halfHeight;

    Camera(const uint32_t horizontalSize, const uint32_t verticalSize, const float fieldOfView) noexcept : hSize(horizontalSize), vSize(verticalSize), fov(fieldOfView), transform(IdentityMatrix())
    {
        const float halfView = std::tan(fov / 2);
        const float aspectRatio = static_cast<float>(hSize) / static_cast<float>(vSize);
        halfWidth = hSize >= vSize ? halfView : halfView * aspectRatio;
        halfHeight = hSize < vSize ? halfView : halfView / aspectRatio;

        pixelSize = halfWidth * 2.0f / static_cast<float>(hSize);
    };

    [[nodiscard]] Ray rayForPixel(const uint32_t x, const uint32_t y) const noexcept;
    [[nodiscard]] Canvas Render(const World& w) const noexcept;
};

#endif /* SRC_CAMERA_HPP_ */
