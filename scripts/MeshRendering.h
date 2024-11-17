//
// Created by miska on 2024/11/17.
//

#pragma once
#include "EcsSystem.h"
#include "Mesh.h"
#include "Transform.h"
#include <flecs.h>

class MeshRendering : public EcsSystem {
public:
    void createSystem(flecs::world &ecs) override {
        ecs.system<Position, Rotation, Scale, MeshRenderer>("UpdatePerObjectUBO")
                .kind(flecs::PreStore).each(
                    [](Position &p, Rotation &r, Scale &s, MeshRenderer &renderer) {
                        const auto modelMatrix = Transform::getModelMatrix(p, r, s);
                        renderer.vertexPushConstants.model = modelMatrix;
                    }
                );

        ecs.system<Position, Rotation, Scale, MeshRendererSplitBuffer>("UpdatePerObjectUBOSplit")
                .kind(flecs::PreStore).
                each(
                    [](Position &p, Rotation &r, Scale &s, MeshRendererSplitBuffer &renderer) {
                        const auto modelMatrix = Transform::getModelMatrix(p, r, s);
                        renderer.vertexPushConstants.model = modelMatrix;
                    }
                );
    }
};
