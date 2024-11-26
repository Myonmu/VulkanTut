//
// Created by miska on 2024/11/24.
//

#include "RenderingBase.h"

#include <VulkanFrame.h>

#include "CBC_Drawing.h"
#include "CBC_Misc.h"
#include "RenderingContext.h"
#include "RenderPassRecorder.h"

void RenderingBase::enqueueSharedCommands(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder) {
    recorder.clear();
    //TODO: this will cause a NextSubpass call per material instance, which is wrong (move to render graph)
    auto subpassId = materialInstance.getMaterial().get_pipeline().pipelineInfo.subpass;
    if (subpassId > 0) {
        recorder.enqueueCommand<RawCommand>(
            [](const CommandBuffer &cmd, const DeviceContext &deviceCtx, const FrameInfo &fInfo) {
                vkCmdNextSubpass(cmd, VK_SUBPASS_CONTENTS_INLINE);
            });
    }
    // TODO: same pipeline could be batched
    recorder.enqueueCommand<BindPipeline>(materialInstance.getPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
    // bind per-frame set
    auto &perFrameSet = *ctx.renderer->getCurrentFrame().get_perFrameDescriptorSet().perFrameSet;
    recorder.enqueueCommand<BindDescriptorSet>(materialInstance.getPipelineLayout(), perFrameSet, PER_FRAME_SET_ID);
    for (auto &[setId, set]: materialInstance.descriptorSets) {
        if (setId <= 0)continue; // 0 is reserved for global set
        recorder.enqueueCommand<BindDescriptorSet>(materialInstance.getPipelineLayout(), *set, setId);
    }
}

void RenderingBase::enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder) {
    recorder.clear();
    enqueueSharedCommands(ctx, renderPassRecorder);

    renderPassRecorder.enqueueCommand<EnqueueRecorder>(recorder);
}

void RenderFullScreenQuad::enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder) {
    recorder.clear();
    enqueueSharedCommands(ctx, renderPassRecorder);
    recorder.enqueueCommand<RawCommand>(
        [](const CommandBuffer &cmd, const DeviceContext &deviceCtx, const FrameInfo &fInfo) {
            vkCmdDraw(cmd, 3, 1, 0 ,0);
        });
    renderPassRecorder.enqueueCommand<EnqueueRecorder>(recorder);
}
