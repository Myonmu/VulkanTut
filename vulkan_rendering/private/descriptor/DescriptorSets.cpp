//
// Created by miska on 2024/08/27.
//

#include "DescriptorSets.h"
#include <UniformBufferObject.h>
#include <VulkanAppContext.h>
#include "DeviceContext.h"
#include "RenderObject.h"
#include "DescriptorPool.h"

DescriptorSets::DescriptorSets(DeviceContext &ctx, DescriptorPool& pool, DescriptorSetLayout& layout): VulkanResource(ctx) {
    auto maxFrameInFlight = ctx.context.MAX_FRAMES_IN_FLIGHT;
    std::vector<VkDescriptorSetLayout> layouts(maxFrameInFlight, layout);
    resource.resize(maxFrameInFlight);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(maxFrameInFlight);
    allocInfo.pSetLayouts = layouts.data();
    const auto result = vkAllocateDescriptorSets(ctx.getLogicalDevice(), &allocInfo, resource.data());
    if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL) {
        throw DescriptorPoolOutOfMemoryException("Descriptor Pool does not have enough memory");
    }
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
    //configureDescriptorSets();
}

DescriptorSets::~DescriptorSets() = default;

void DescriptorSets::configureDescriptorSets(const RenderObject& obj) {
    for (size_t i = 0; i < resource.size(); i++) {
        textureResIndex = 0;
        std::vector<VkWriteDescriptorSet> descriptorWrites{ctx.get_descriptorSetLayout().layoutBindings.size()};
        for (size_t j = 0; j < descriptorWrites.size(); j++) {
            const auto &binding = ctx.get_descriptorSetLayout().layoutBindings[j];
            descriptorWrites[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[j].dstSet = resource[i];
            descriptorWrites[j].dstBinding = j;
            descriptorWrites[j].dstArrayElement = 0;
            descriptorWrites[j].descriptorCount = 1;
            if (binding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
                VkDescriptorBufferInfo bufferInfo = {};
                bufferInfo.buffer = obj.uniformBufferGroup[i];
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

        vkUpdateDescriptorSets(ctx.getLogicalDevice(), descriptorWrites.size(),
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

