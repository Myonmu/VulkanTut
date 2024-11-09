//
// Created by miska on 2024/11/03.
//

#pragma once
#include <SwapChain.h>
#include <glm/mat4x4.hpp>


struct Rotation;
struct Position;

//TODO: Support ortho camera
class Camera {
public:
    SwapChain* output;
    float fov = 45.0f;  // Field of view in degrees
    float aspectRatio = 8.0f / 6.0f;  // Aspect ratio (width/height)
    float nearPlane = 0.1f;  // Near clipping plane
    float farPlane = 100.0f;  // Far clipping plane

    explicit Camera(SwapChain* swapChain): output(swapChain) {}
    ~Camera()= default;
    [[nodiscard]] static glm::mat4x4 getViewMatrix(const Position &position, const Rotation &rotation) ;

    [[nodiscard]] glm::mat4x4 getProjectionMatrix();
};
