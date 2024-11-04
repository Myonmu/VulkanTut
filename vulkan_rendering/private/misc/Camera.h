//
// Created by miska on 2024/11/03.
//

#pragma once
#include <glm/mat4x4.hpp>

#include "ObjectHierarchy.h"

//TODO: Support ortho camera
class Camera: public SceneObject {
public:
    float fov = 45.0f;  // Field of view in degrees
    float aspectRatio = 16.0f / 9.0f;  // Aspect ratio (width/height)
    float nearPlane = 0.1f;  // Near clipping plane
    float farPlane = 100.0f;  // Far clipping plane

    [[nodiscard]] glm::mat4x4 getViewMatrix() const ;

    [[nodiscard]] glm::mat4x4 getProjectionMatrix() const ;
};
