//
// Created by miska on 2024/11/04.
//

#include "RenderingContext.h"

#include <DescriptorSetLayout.h>

#include "DescriptorWriter.h"
#include "FrameInfo.h"

std::map<uint32_t, DescriptorSetLayoutBinding> PerSceneRenderingData::bindings = {
    {0, {.binding = 0, .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}},
};

std::vector<DescriptorAllocator::PoolSizeRatio> PerSceneRenderingData::poolSizes = {
    {.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER , .ratio = 1}
};

RenderingContext::RenderingContext(DeviceContext& ctx)
    : SubContext(ctx){
    perSceneDescriptorLayout = std::make_unique<DescriptorSetLayout>(ctx, PerSceneRenderingData::bindings);
    descriptorAllocator = std::make_unique<DescriptorAllocator>(ctx);
    descriptorAllocator->init(1000, PerSceneRenderingData::poolSizes);
    perSceneUbo = std::make_unique<PerFrameBufferGroup>(ctx, sizeof(PerSceneRenderingData));
}


void RenderingContext::prepareFrame(const FrameInfo &frameInfo) {
    auto perFrameSet = descriptorAllocator->allocate(*perSceneDescriptorLayout);
    DescriptorWriter writer{};

    writer.writeBuffer(0, (*perSceneUbo)[frameInfo.currentFrameIndex],
        sizeof(perSceneData), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    writer.updateSet(context.get_logicalDevice(), (*perFrameSet)[0]);
}

