//
// Created by miska on 2024/11/03.
//

#pragma once
#include "Material.h"
#include "MeshBuffer.h"
#include "RenderingContext.h"
#include "RenderPassRecorder.h"

class MeshRendererSplitBuffer {
    const VertexBuffer &vertexBuffer;
    const IndexBuffer &indexBuffer;
    const MaterialInstance &materialInstance;
    CommandBufferRecorder recorder{true};

public:
    PerObjectVertexPushConstants vertexPushConstants;

    MeshRendererSplitBuffer(DeviceContext &ctx, const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer,
                            const MaterialInstance &materialInstance);

    ~MeshRendererSplitBuffer() = default;

    void enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder);
};

class MeshRenderer {
    const MeshBuffer &meshBuffer;
    const MaterialInstance &materialInstance;
    CommandBufferRecorder recorder{true};

public:
    PerObjectVertexPushConstants vertexPushConstants;

    MeshRenderer(DeviceContext &ctx, const MeshBuffer &meshBuffer, const MaterialInstance &materialInstance);

    ~MeshRenderer() = default;

    void enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder);
};
