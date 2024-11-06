//
// Created by miska on 2024/11/03.
//
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "Camera.h"
#include <Transform.h>

glm::mat4x4 Camera::getViewMatrix(const Position &position, const Rotation &rotation) {
    const glm::mat4 rotationMatrix = rotation.getRotationMatrix();
    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position.translation);
    return rotationMatrix * translationMatrix;
}

glm::mat4x4 Camera::getProjectionMatrix() const {
    auto projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    projection[1][1] *= -1;
    return projection;
}
