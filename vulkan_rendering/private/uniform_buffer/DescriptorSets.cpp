//
// Created by miska on 2024/08/27.
//

#include "DescriptorSets.h"

#include <UniformBufferObject.h>
#include <VulkanAppContext.h>

DescriptorSets::DescriptorSets(VulkanAppContext &ctx): VulkanResource(ctx) {
    std::vector<VkDescriptorSetLayout> layouts(ctx.MAX_FRAMES_IN_FLIGHT, ctx.descriptorSetLayout);
    resource.resize(ctx.MAX_FRAMES_IN_FLIGHT);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = ctx.descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(ctx.MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();
    if (vkAllocateDescriptorSets(ctx.logicalDevice, &allocInfo, resource.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
    configureDescriptorSets();
}

DescriptorSets::~DescriptorSets() = default;

void DescriptorSets::configureDescriptorSets() const {
    for (size_t i = 0; i < resource.size(); i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = ctx.uniformBufferGroup[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = resource[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;

        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;

        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;
        vkUpdateDescriptorSets(ctx.logicalDevice, 1, &descriptorWrite, 0, nullptr);
    }
}

VkDescriptorSet &DescriptorSets::operator[](size_t index) {
    return resource[index];
}

