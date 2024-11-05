//
// Created by miska on 2024/11/03.
//
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "Camera.h"

#include <stdexcept>

glm::mat4x4 Camera::getViewMatrix() const {
    const auto transform = parent.getComponent<Transform>();
    if(transform == nullptr) {
        throw std::runtime_error("Camera::getViewMatrix(): transform is null");
    }
    glm::mat4 rotationMatrix = transform->getRotationMatrix();
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -transform->translation);
    return rotationMatrix * translationMatrix;
}

glm::mat4x4 Camera::getProjectionMatrix() const {
    auto projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    projection[1][1] *= -1;
    return projection;
}
