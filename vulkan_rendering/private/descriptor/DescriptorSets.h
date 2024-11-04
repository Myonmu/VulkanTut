//
// Created by miska on 2024/08/27.
//

#pragma once

#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>
#include "DeviceContext.h"

class DescriptorPool;
class DescriptorSetLayout;
class RenderObject;
struct DescriptorContext;

class DescriptorPoolOutOfMemoryException final : public std::runtime_error {
public:
    explicit DescriptorPoolOutOfMemoryException(const std::string& what) : std::runtime_error(what) {}
};

class DescriptorSets : public VulkanResource<std::vector<VkDescriptorSet>, DeviceContext> {
    //std::vector<ImageView*> images{};
    //std::vector<TextureSampler*> samplers{};
    //uint32_t textureResIndex = 0;
public:
    explicit DescriptorSets(DeviceContext& ctx, DescriptorPool& pool, DescriptorSetLayout& layout, bool allocPerFrame = false);
    ~DescriptorSets() override;
    VkDescriptorSet& operator[](size_t index);

    //void configureDescriptorSets(const RenderObject& obj);

    //void AddTextureImageView(ImageView* textureImage);
    //void AddTextureSampler(TextureSampler* textureSampler);
};
