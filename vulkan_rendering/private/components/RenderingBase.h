//
// Created by miska on 2024/11/24.
//

#pragma once
#include "Material.h"

class RenderPassRecorder;

class RenderingBase {
protected:
    CommandBufferRecorder recorder{true};

    void enqueueSharedCommands(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder);

public:
    const MaterialInstance &materialInstance;
    RenderingBase(DeviceContext &ctx, const MaterialInstance &materialInstance): materialInstance(materialInstance) {
    };

    virtual ~RenderingBase() = default;

    virtual void enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder);
};


class RenderFullScreenQuad : public RenderingBase {
public:
    RenderFullScreenQuad(DeviceContext &ctx, const MaterialInstance &materialInstance): RenderingBase(
        ctx, materialInstance) {
    }

    void enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder) override;
};
