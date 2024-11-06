//
// Created by miska on 2024/11/03.
//

#pragma once
#include <glm/mat4x4.hpp>


struct Rotation;
struct Position;

//TODO: Support ortho camera
class Camera {
public:
    float fov = 45.0f;  // Field of view in degrees
    float aspectRatio = 16.0f / 9.0f;  // Aspect ratio (width/height)
    float nearPlane = 0.1f;  // Near clipping plane
    float farPlane = 100.0f;  // Far clipping plane

    explicit Camera() {}
    ~Camera()= default;
    [[nodiscard]] static glm::mat4x4 getViewMatrix(const Position &position, const Rotation &rotation) ;

    [[nodiscard]] glm::mat4x4 getProjectionMatrix() const ;
};
