//
// Created by miska on 2024/08/27.
//

#pragma once

#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "ImageView.h"
#include "TextureSampler.h"


class RenderObject;
struct DescriptorContext;

class DescriptorSets : public VulkanResource<std::vector<VkDescriptorSet>, DescriptorContext> {
    std::vector<ImageView*> images{};
    std::vector<TextureSampler*> samplers{};
    uint32_t textureResIndex = 0;
public:
    explicit DescriptorSets(DescriptorContext& ctx);
    ~DescriptorSets() override;
    VkDescriptorSet& operator[](size_t index);

    void configureDescriptorSets(const RenderObject& obj);

    void AddTextureImageView(ImageView* textureImage);
    void AddTextureSampler(TextureSampler* textureSampler);
};
