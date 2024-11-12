//
// Created by Tereza on 11/12/2024.
//

#pragma once
#include <chrono>

#include "EcsSystem.h"
#include "flecs.h"
#include "Transform.h"

class MeshRotate : public EcsSystem{
public:
    void createSystem(flecs::world &ecs) override {
        ecs.system<Rotation>("MeshRotate").with<MeshRotate>().kind(flecs::OnUpdate).each(
           [](Rotation &rot) {
               static auto startTime = std::chrono::high_resolution_clock::now();
               auto currentTime = std::chrono::high_resolution_clock::now();
               auto time = std::chrono::duration<float>(currentTime - startTime).count();
               auto euler = glm::vec3{0.0f, 0, glm::radians(90.0f) * time};
               auto result = glm::quat(euler);
               rot.rotation = result;
           });
    }
};
