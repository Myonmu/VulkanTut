//
// Created by miska on 2024/11/03.
//

#pragma once

#include "Material.h"
#include "MeshBuffer.h"
#include "RenderPassRecorder.h"


class MeshRenderer {
    const MeshBuffer& meshBuffer;
    const MaterialInstance& materialInstance;
    CommandBufferRecorder recorder{true};
public:
    MeshRenderer(const MeshBuffer& meshBuffer, const MaterialInstance& materialInstance);
    void enqueueDrawCall(RenderPassRecorder& renderPassRecorder);
};
