//
// Created by miska on 2024/10/27.
//

#pragma once
#include <deque>
#include <vector>
#include "vulkan/vulkan_core.h"

class Buffer;
class TextureSampler;
class ImageView;

struct  DescriptorWriter {
    std::deque<VkDescriptorImageInfo> imageInfos;
    std::deque<VkDescriptorBufferInfo> bufferInfos;
    std::vector<VkWriteDescriptorSet> writes;

    void writeImage(int binding, const ImageView& image, const TextureSampler &sampler , VkImageLayout layout, VkDescriptorType type);
    void writeBuffer(int binding,const Buffer& buffer,size_t size, size_t offset,VkDescriptorType type);

    void clear();
    void updateSet(VkDevice device, VkDescriptorSet set);
};
