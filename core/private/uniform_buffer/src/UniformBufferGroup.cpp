//
// Created by miska on 2024/08/21.
//

#include "UniformBufferGroup.h"

#include <UniformBufferObject.h>
#include <VulkanAppContext.h>

UniformBufferGroup::UniformBufferGroup(VulkanAppContext &ctx) :ctx(ctx) {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(ctx.MAX_FRAMES_IN_FLIGHT);
    uniformBuffersData.resize(ctx.MAX_FRAMES_IN_FLIGHT);

    for (auto i = 0; i < ctx.MAX_FRAMES_IN_FLIGHT; i++) {
        uniformBuffers[i] = new Buffer(ctx, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        uniformBuffers[i]->mapBufferMemory(uniformBuffersData[i]);
    }
}

UniformBufferGroup::~UniformBufferGroup() {
    for (auto i = 0; i < uniformBuffers.size(); i++) {
        delete uniformBuffers[i];
    }
}

void UniformBufferGroup::CopyMemoryToBuffer(const uint32_t &slot, const void *data, const size_t &size) const {
    memcpy(uniformBuffersData[slot], data, size);
}


Buffer &UniformBufferGroup::operator[](const size_t index) const {
    return *uniformBuffers[index];
}
