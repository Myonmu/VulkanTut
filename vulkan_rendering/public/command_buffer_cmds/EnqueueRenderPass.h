//
// Created by miska on 10/9/2024.
//

#ifndef ENQUEUERENDERPASS_H
#define ENQUEUERENDERPASS_H

#include "CommandBufferCmd.h"
#include "RenderPassRecorder.h"

class EnqueueRenderPass final : public CommandBufferCmd{
    RenderPassRecorder& recorder;
public:
    explicit EnqueueRenderPass(RenderPassRecorder& recorder): recorder(recorder) {

    }
    void execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) override;
};

inline void EnqueueRenderPass::execute(const CommandBuffer &commandBuffer, const VulkanAppContext &context, const FrameInfo &frameInfo) {
    recorder.recordRenderPass(commandBuffer, context,  frameInfo);
}




#endif //ENQUEUERENDERPASS_H
