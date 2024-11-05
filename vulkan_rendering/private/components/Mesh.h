//
// Created by miska on 2024/11/03.
//

#pragma once

#include "Ecs.h"
#include "Material.h"
#include "MeshBuffer.h"
#include "RenderPassRecorder.h"


class MeshRenderer: public Component{
    const MeshBuffer& meshBuffer;
    const MaterialInstance& materialInstance;
    CommandBufferRecorder recorder{true};
public:
    MeshRenderer(Entity& e, const MeshBuffer& meshBuffer, const MaterialInstance& materialInstance);
    void enqueueDrawCall(RenderPassRecorder& renderPassRecorder);
};
