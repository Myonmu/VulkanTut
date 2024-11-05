//
// Created by miska on 2024/11/04.
//

#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include "Ecs.h"
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>


struct Transform: public Component{
    Transform* parentTransform{};

    glm::vec3 translation{};
    glm::vec3 scale{1, 1, 1};
    glm::quat rotation{};

    explicit Transform(Entity& e) : Component(e){}

    [[nodiscard]] glm::mat4x4 getTranslateMatrix() const {
        return glm::translate(glm::mat4x4(1), translation);
    }

    [[nodiscard]] glm::mat4x4 getScaleMatrix() const {
        return glm::scale(glm::mat4x4(1), scale);
    }

    [[nodiscard]] glm::mat4x4 getRotationMatrix() const {
        return glm::toMat4(rotation);
    }

    [[nodiscard]] glm::mat4x4 getModelMatrix() const {
        return getTranslateMatrix() * getRotationMatrix() * getScaleMatrix();
    }

    void lookAt(glm::vec3 pos) {
        rotation = glm::quatLookAt(pos - translation, glm::vec3(0,0,1));
    }
};
