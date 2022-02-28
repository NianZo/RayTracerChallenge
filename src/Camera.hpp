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

    Camera(uint32_t horizontalSize, uint32_t verticalSize, float fieldOfView, const Matrix<4>& viewTransform=IdentityMatrix()) noexcept : hSize(horizontalSize), vSize(verticalSize), fov(fieldOfView), transform(viewTransform)
    {
        const float halfView = std::tan(fov / 2);
        const float aspectRatio = static_cast<float>(hSize) / static_cast<float>(vSize);
        halfWidth = hSize >= vSize ? halfView : halfView * aspectRatio;
        halfHeight = hSize < vSize ? halfView : halfView / aspectRatio;

        pixelSize = halfWidth * 2.0F / static_cast<float>(hSize);
    };
    [[nodiscard]] bool operator==(const Camera& other) const noexcept;

    [[nodiscard]] Ray rayForPixel(uint32_t x, uint32_t y) const noexcept;
    [[nodiscard]] Canvas Render(const World& w) const noexcept;
};

#endif /* SRC_CAMERA_HPP_ */
