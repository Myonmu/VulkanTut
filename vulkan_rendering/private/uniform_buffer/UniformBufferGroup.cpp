//
// Created by miska on 2024/08/21.
//

#include "UniformBufferGroup.h"

#include <UniformBufferObject.h>
#include <VulkanAppContext.h>

#include "DeviceContext.h"

UniformBufferGroup::UniformBufferGroup(DeviceContext &ctx) :ctx(ctx) {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    auto maxFramesInFlight = ctx.context.MAX_FRAMES_IN_FLIGHT;
    uniformBuffers.resize(maxFramesInFlight);
    uniformBuffersData.resize(maxFramesInFlight);

    for (auto i = 0; i < maxFramesInFlight; i++) {
        uniformBuffers[i] = new Buffer(ctx, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VMA_MEMORY_USAGE_CPU_TO_GPU
            //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
        uniformBuffers[i]->mapBufferMemory(uniformBuffersData[i]);
    }
}

UniformBufferGroup::~UniformBufferGroup() {
    for (const auto & uniformBuffer : uniformBuffers) {
        delete uniformBuffer;
    }
}

void UniformBufferGroup::CopyMemoryToBuffer(const uint32_t &slot, const void *data, const size_t &size) const {
    memcpy(uniformBuffersData[slot], data, size);
}


Buffer &UniformBufferGroup::operator[](const size_t index) const {
    return *uniformBuffers[index];
}
