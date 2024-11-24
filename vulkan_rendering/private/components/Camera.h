//
// Created by miska on 2024/11/03.
//

#pragma once
#include <map>
#include <SwapChain.h>
#include <glm/mat4x4.hpp>
#include "DescriptorPoolAllocator.h"
#include "DescriptorSetLayoutBinding.h"


struct Rotation;
struct Position;

struct CameraUboData {
    glm::mat4 view;
    glm::mat4 projection;
    // .x near plane, .y far plane, .z aspect, .w fov
    glm::vec4 camProps;
};

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

    glm::vec4 getCameraPropertyVector() const {
        return glm::vec4(nearPlane, farPlane, aspectRatio, fov);
    }
};
