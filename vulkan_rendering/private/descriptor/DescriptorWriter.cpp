//
// Created by miska on 2024/10/27.
//

#include "DescriptorWriter.h"

#include <Buffer.h>
#include <DescriptorSet.h>
#include <DescriptorSetLayout.h>
#include <fmt/core.h>
#include <TextureSampler.h>

void DescriptorWriter::writeImage(uint32_t binding,
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

void DescriptorWriter::writeBuffer(uint32_t binding, const Buffer &buffer, size_t size, size_t offset,
                                   VkDescriptorType type) {
    VkDescriptorBufferInfo const &info = bufferInfos.emplace_back(VkDescriptorBufferInfo{
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

void DescriptorWriter::updateSet(LogicalDevice& device, DescriptorSet& set) {
    //fmt::println("------- UPDATE DESCRIPTOR SET {} ---------", set.layout.setId);

    for (VkWriteDescriptorSet &write: writes) {
        write.dstSet = set;
        //fmt::println("binding {} ({}) cnt {}", write.dstBinding, static_cast<uint32_t>(write.descriptorType), write.descriptorCount);
    }

    vkUpdateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(),
                           0, nullptr);

    //fmt::println("^^^^^^ END OF UPDATE CALL FOR SET {} ^^^^^^", set.layout.setId);
}
