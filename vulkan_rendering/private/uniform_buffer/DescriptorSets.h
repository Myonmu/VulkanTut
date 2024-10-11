//
// Created by miska on 2024/08/27.
//

#ifndef DESCRIPTORSETS_H
#define DESCRIPTORSETS_H
#include <TextureImage.h>
#include <vector>
#include <VulkanResource.h>
#include <vulkan/vulkan_core.h>

#include "ImageView.h"
#include "TextureSampler.h"


class DescriptorSets : public VulkanResource<std::vector<VkDescriptorSet>> {

    std::vector<ImageView*> images{};
    std::vector<TextureSampler*> samplers{};
    uint32_t textureResIndex = 0;
public:
    explicit DescriptorSets(VulkanAppContext& ctx);
    ~DescriptorSets() override;
    VkDescriptorSet& operator[](size_t index);

    void configureDescriptorSets();

    void AddTextureImageView(ImageView* textureImage);
    void AddTextureSampler(TextureSampler* textureSampler);
};



#endif //DESCRIPTORSETS_H
