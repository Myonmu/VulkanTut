//
// Created by Tereza on 11/6/2024.
//

#pragma once
#include <chrono>

#include "Ecs.h"
#include "Transform.h"


class MeshRotate: public Script {
public:
    using Script::Script;
    ~MeshRotate() override = default;

    void update() override {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float>(currentTime - startTime).count();

        const auto trans = parent->getComponent<Transform>();

        trans->rotation = glm::quat({0.0f, 0, glm::radians(90.0f)*time}) * trans->rotation;
    }
};
