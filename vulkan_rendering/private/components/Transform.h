//
// Created by miska on 2024/11/04.
//

#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

struct Position {
    glm::vec3 translation{};

    [[nodiscard]] glm::mat4x4 getTranslateMatrix() const {
        return glm::translate(glm::mat4x4(1), translation);
    }
};

struct Rotation {
    glm::quat rotation{0.0f,0,0,1};

    [[nodiscard]] glm::mat4x4 getRotationMatrix() const {
        return glm::toMat4(rotation);
    }
};

struct Scale {
    glm::vec3 scale{1, 1, 1};

    [[nodiscard]] glm::mat4x4 getScaleMatrix() const {
        return glm::scale(glm::mat4x4(1), scale);
    }
};

struct Transform {

    static [[nodiscard]] glm::mat4x4 getModelMatrix(const Position& p, const Rotation& r, const Scale& s) {
        return p.getTranslateMatrix() * r.getRotationMatrix() * s.getScaleMatrix();
    }

    static void lookAt(Position& p, Rotation& r, const glm::vec3 pos) {
        r.rotation = glm::quatLookAt(pos - p.translation, glm::vec3(0, 0, 1));
    }
};
