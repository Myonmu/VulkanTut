//
// Created by miska on 2024/11/03.
//

#pragma once

#include <UniformBufferObject.h>

#include "Material.h"
#include "MeshBuffer.h"
#include "PerFrameBufferGroup.h"
#include "RenderPassRecorder.h"

struct PerObjectBuffer : public ObjectNode {
    std::unique_ptr<PerFrameBufferGroup> bufferGroup;
    explicit PerObjectBuffer(DeviceContext& ctx) {
        bufferGroup = std::make_unique<PerFrameBufferGroup>(ctx, sizeof(UniformBufferObject));
    };
    void updatePerObjectBuffer(const FrameInfo& frameInfo, const Position& pos, const Rotation& rot, const Scale& scale) const;
};

class MeshRenderer{

    const MeshBuffer& meshBuffer;
    const MaterialInstance& materialInstance;
    CommandBufferRecorder recorder{true};
    CTX_PROPERTY(PerObjectBuffer, perObjectBuffer);
public:

    MeshRenderer(DeviceContext& ctx, const MeshBuffer& meshBuffer, const MaterialInstance& materialInstance);
    ~MeshRenderer()  = default;
    void enqueueDrawCall(RenderingContext& ctx, RenderPassRecorder& renderPassRecorder);
    void updatePerObjectDescriptorSet(RenderingContext& ctx);
};
