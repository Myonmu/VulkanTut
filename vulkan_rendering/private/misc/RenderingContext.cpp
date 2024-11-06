//
// Created by miska on 2024/11/04.
//

#include "RenderingContext.h"

#include <DescriptorSetLayout.h>
#include <VulkanAppContext.h>

#include "DescriptorWriter.h"
#include "FrameInfo.h"

std::map<uint32_t, DescriptorSetLayoutBinding> PerSceneRenderingData::bindings = {
    {0, {.binding = 0, .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}},
};

std::vector<DescriptorAllocator::PoolSizeRatio> PerSceneRenderingData::poolSizes = {
    {.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER , .ratio = 2}
};

RenderingContext::RenderingContext(DeviceContext& ctx)
    : SubContext(ctx){
    perSceneDescriptorLayout = std::make_unique<DescriptorSetLayout>(ctx, PerSceneRenderingData::bindings);
    descriptorAllocator = std::make_unique<DescriptorAllocator>(ctx);
    descriptorAllocator->init(1000, PerSceneRenderingData::poolSizes);
    perSceneUbo = std::make_unique<PerFrameBufferGroup>(ctx, sizeof(PerSceneRenderingData));
    auto framesInFlight = ctx.context.MAX_FRAMES_IN_FLIGHT;
    for(int i = 0 ; i < framesInFlight; ++i) {
        perFrameSets.emplace_back(descriptorAllocator->allocate(*perSceneDescriptorLayout));
    }
}


void RenderingContext::prepareFrame(const FrameInfo &frameInfo) {
    auto& perFrameSet = *perFrameSets[frameInfo.currentFrameIndex];
    DescriptorWriter writer{};
    perSceneUbo->CopyMemoryToBuffer(frameInfo.currentFrameIndex, &perSceneData, sizeof(perSceneUbo));
    auto& buffer = (*perSceneUbo)[frameInfo.currentFrameIndex];
    writer.writeBuffer(0, buffer,
        sizeof(perSceneData), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    writer.updateSet(context.get_logicalDevice(), (perFrameSet)[0]); //the 0 index is due to a legacy design
}

LogicalDevice &RenderingContext::getLogicalDevice() const {
    return context.get_logicalDevice();
}


