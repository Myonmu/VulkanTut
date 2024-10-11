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
    //configureDescriptorSets();
}

DescriptorSets::~DescriptorSets() = default;

void DescriptorSets::configureDescriptorSets() {
    for (size_t i = 0; i < resource.size(); i++) {
        textureResIndex = 0;
        std::vector<VkWriteDescriptorSet> descriptorWrites{ctx.descriptorSetLayout.layoutBindings.size()};
        for (size_t j = 0; j < descriptorWrites.size(); j++) {
            const auto &binding = ctx.descriptorSetLayout.layoutBindings[j];
            descriptorWrites[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[j].dstSet = resource[i];
            descriptorWrites[j].dstBinding = j;
            descriptorWrites[j].dstArrayElement = 0;
            descriptorWrites[j].descriptorCount = 1;
            if (binding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
                VkDescriptorBufferInfo bufferInfo = {};
                bufferInfo.buffer = ctx.uniformBufferGroup[i];
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(UniformBufferObject);

                descriptorWrites[j].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[j].pBufferInfo = &bufferInfo;
            } else if (binding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                //TODO: STUB
                imageInfo.imageView = images[textureResIndex]->getRaw();
                imageInfo.sampler = samplers[textureResIndex]->getRaw();
                textureResIndex += 1;

                descriptorWrites[j].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[j].pImageInfo = &imageInfo;
            }
        }

        vkUpdateDescriptorSets(ctx.logicalDevice, descriptorWrites.size(),
                               descriptorWrites.data(),
                               0, nullptr);
    }
}

VkDescriptorSet &DescriptorSets::operator[](size_t index) {
    return resource[index];
}

void DescriptorSets::AddTextureImageView(ImageView *textureImage) {
    images.push_back(textureImage);
}

void DescriptorSets::AddTextureSampler(TextureSampler *textureSampler) {
    samplers.push_back(textureSampler);
}

