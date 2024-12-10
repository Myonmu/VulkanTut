//
// Created by miska on 2024/11/03.
//
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "Camera.h"
#include <Transform.h>

glm::mat4x4 Camera::getViewMatrix(const Position &position, const Rotation &rotation) {
    glm::mat4 cameraTranslation = glm::translate(glm::mat4(1.f), position.translation);
    glm::mat4 cameraRotation = rotation.getRotationMatrix();
    return glm::inverse(cameraTranslation * cameraRotation);
}

glm::vec3 Camera::getWorldSpaceViewDirection(const Position &position, const Rotation &rotation) {
    glm::vec3 forward = glm::normalize(glm::vec3(rotation.getRotationMatrix()[2])); // third column in the rotation matrix
    // Return the world-space view vector (looking in the direction opposite of where the camera is facing)
    return -forward; // camera looks in negative Z-axis in camera space
}


glm::mat4x4 Camera::getProjectionMatrix() {
    if (output != nullptr) {
        aspectRatio = output->swapChainExtent.width / static_cast<float>(output->swapChainExtent.height);
    }
    auto projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    projection[1][1] *= -1;
    return projection;
}
