//
// Created by Tereza on 11/12/2024.
//

#pragma once
#include "EcsSystem.h"
#include "Transform.h"
#include <flecs.h>


class LinearPositionUpdate : public EcsSystem {
public:
    void createSystem(flecs::world &ecs) override {
        ecs.system<const Velocity, Position, const Rotation>("LinearPositionUpdate").kind(flecs::OnUpdate).run(
            [](flecs::iter &it) {
                while (it.next()) {
                    auto p = it.field<Position>(1);
                    auto v = it.field<const Velocity>(0);
                    auto r = it.field<const Rotation>(2);
                    for (auto i: it) {
                        glm::mat4 rotMat = r[i].getRotationMatrix();
                        p[i].translation += glm::vec3(rotMat * glm::vec4(v[i].velocity * it.delta_time(), 0.f));
                    }
                }
            });
    }
};
