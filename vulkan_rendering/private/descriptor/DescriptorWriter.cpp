//
// Created by miska on 2024/10/27.
//

#include "DescriptorWriter.h"

#include <Buffer.h>

#include "ImageView.h"
#include "TextureSampler.h"

void DescriptorWriter::writeImage(int binding,
                                  const ImageView &image,
                                  const TextureSampler &sampler,
                                  VkImageLayout layout,
                                  VkDescriptorType type) {
    VkDescriptorImageInfo &info = imageInfos.emplace_back(VkDescriptorImageInfo{
        .sampler = sampler,
        .imageView = image,
        .imageLayout = layout
    });

    VkWriteDescriptorSet write = {.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};

    write.dstBinding = binding;
    write.dstSet = VK_NULL_HANDLE; //left empty for now until we need to write it
    write.descriptorCount = 1;
    write.descriptorType = type;
    write.pImageInfo = &info;

    writes.push_back(write);
}

void DescriptorWriter::writeBuffer(int binding, const Buffer &buffer, size_t size, size_t offset,
                                   VkDescriptorType type) {
    VkDescriptorBufferInfo &info = bufferInfos.emplace_back(VkDescriptorBufferInfo{
        .buffer = buffer,
        .offset = offset,
        .range = size
    });

    VkWriteDescriptorSet write = {.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};

    write.dstBinding = binding;
    write.dstSet = VK_NULL_HANDLE; //left empty for now until we need to write it
    write.descriptorCount = 1;
    write.descriptorType = type;
    write.pBufferInfo = &info;

    writes.push_back(write);
}

void DescriptorWriter::clear() {
    imageInfos.clear();
    writes.clear();
    bufferInfos.clear();
}

void DescriptorWriter::updateSet(VkDevice device, VkDescriptorSet set) {
    for (VkWriteDescriptorSet &write: writes) {
        write.dstSet = set;
    }

    vkUpdateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(),
                           0, nullptr);
}
