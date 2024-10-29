//
// Created by Tereza on 10/29/2024.
//

#pragma once
#include <DescriptorSets.h>
#include <map>

#include "DescriptorWriter.h"


class UniformBufferGroup;
class TextureImage;
struct FrameInfo;
class Material;

class MaterialInstance {
    Material &srcMaterial;
    DeviceContext &ctx;
    DescriptorWriter descriptorWriter{};

public:
    std::map<uint32_t, DescriptorSets> descriptorSets;
    explicit MaterialInstance(Material &material);

    ~MaterialInstance();

    // TODO: Remove "i"
    void updateDescriptorSet(uint32_t setId, uint32_t i);

    void setCombinedImageSampler(uint32_t binding, const TextureImage &textureImage, const TextureSampler &sampler,
                                 const ImageView &imageView);

    // TODO: Remove "i"
    void setUBO(uint32_t binding, uint32_t i, UniformBufferGroup& ubo);

    DescriptorSets& getDescriptorSet(uint32_t setId);
};
