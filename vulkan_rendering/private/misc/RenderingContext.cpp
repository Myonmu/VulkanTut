//
// Created by miska on 2024/11/04.
//

#include "RenderingContext.h"
#include <DescriptorSetLayout.h>
#include <VulkanFrame.h>


VkPushConstantRange PerObjectVertexPushConstants::getPushConstantsRange() {
    return {
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .offset = 0,
        .size = sizeof(PerObjectVertexPushConstants),
    };
}

RenderingContext::RenderingContext(DeviceContext& ctx)
    : SubContext(ctx){
}


void RenderingContext::prepareFrame(const FrameInfo &frameInfo) {
    auto& frame = renderer->getCurrentFrame();
    auto& perFrameSet =  frame.get_perFrameDescriptorSet();
    perFrameSet.writeBuffer(0, &cameraUboData, sizeof(CameraUboData));
    perFrameSet.writeBuffer(1, &mainLightUboData, sizeof(MainLightUboData));
    perFrameSet.updateSet();
    f();
}

LogicalDevice &RenderingContext::getLogicalDevice() const {
    return context.get_logicalDevice();
}
