//
// Created by miska on 2024/11/03.
//

#pragma once
#include "Material.h"
#include "MeshBuffer.h"
#include "RenderingBase.h"
#include "RenderingContext.h"

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

class MeshRenderer: public RenderingBase {
    const MeshBuffer &meshBuffer;
public:
    PerObjectVertexPushConstants vertexPushConstants{};

    MeshRenderer(DeviceContext &ctx, const MeshBuffer &meshBuffer, const MaterialInstance &materialInstance);

    ~MeshRenderer() override = default;

    void enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder) override;
};
