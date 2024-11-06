//
// Created by miska on 2024/08/21.
//

#include "PerFrameBufferGroup.h"

#include <UniformBufferObject.h>
#include <VulkanAppContext.h>

#include "DeviceContext.h"

PerFrameBufferGroup::PerFrameBufferGroup(DeviceContext &ctx, VkDeviceSize bufferSize) : ctx(ctx) {
    auto maxFramesInFlight = ctx.context.MAX_FRAMES_IN_FLIGHT;
    buffers.resize(maxFramesInFlight);
    data.resize(maxFramesInFlight);

    for (auto i = 0; i < maxFramesInFlight; i++) {
        buffers[i] = std::make_unique<Buffer>
        (ctx, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
         VMA_MEMORY_USAGE_CPU_TO_GPU
         //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
    }
}

PerFrameBufferGroup::~PerFrameBufferGroup() = default;

void PerFrameBufferGroup::CopyMemoryToBuffer(const uint32_t &slot, const void *data, const size_t &size) const {
    buffers[slot].get()->copyToBufferMemory(data, 0);
}


Buffer &PerFrameBufferGroup::operator[](const size_t index) const {
    return *buffers[index];
}
