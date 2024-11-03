//
// Created by miska on 2024/11/03.
//

#pragma once

#include "Material.h"
#include "MeshBuffer.h"
#include "RenderPassRecorder.h"


class MeshRenderer: public ObjectNode {
    MeshBuffer& meshBuffer;
    MaterialInstance& materialInstance;
    CommandBufferRecorder recorder{true};
public:
    MeshRenderer(MeshBuffer& meshBuffer, MaterialInstance& materialInstance);
    void enqueueDrawCall(RenderPassRecorder& renderPassRecorder);
};
