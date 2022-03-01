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
    uint32_t hSize;
    uint32_t vSize;
    float fov;
    Matrix<4> transform;
    float pixelSize;
    float halfWidth;
    float halfHeight;

    Camera(uint32_t horizontalSize, uint32_t verticalSize, float fieldOfView, const Matrix<4>& viewTransform = IdentityMatrix()) noexcept : hSize(horizontalSize), vSize(verticalSize), fov(fieldOfView), transform(viewTransform)
    {
        RecalculateProperties();
    };
    [[nodiscard]] bool operator==(const Camera& other) const noexcept;

    [[nodiscard]] Ray rayForPixel(uint32_t x, uint32_t y) const noexcept;
    [[nodiscard]] Canvas Render(const World& w) const noexcept;
    void RecalculateProperties() noexcept;
};

#endif /* SRC_CAMERA_HPP_ */
