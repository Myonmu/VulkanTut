//
// Created by Tereza on 10/29/2024.
//

#pragma once
#include <DescriptorSets.h>
#include <map>

#include "DescriptorWriter.h"
#include "ObjectHierarchy.h"


class UniformBufferGroup;
class TextureImage;
struct FrameInfo;
class Material;

class MaterialInstance : public ObjectNode {
    Material &srcMaterial;
    DeviceContext &ctx;
    DescriptorWriter descriptorWriter{};

public:
    explicit MaterialInstance(Material &material);

    ~MaterialInstance() override;

    friend class Material;

    std::map<uint32_t, std::unique_ptr<DescriptorSets> > descriptorSets;

    // TODO: Remove "i"
    void updateDescriptorSet(uint32_t setId, uint32_t i);

    void setCombinedImageSampler(uint32_t binding, const TextureImage &textureImage, const TextureSampler &sampler,
                                 const ImageView &imageView);

    // TODO: Remove "i"
    void setUBO(uint32_t binding, uint32_t i, UniformBufferGroup &ubo);

    DescriptorSets &getDescriptorSet(uint32_t setId) const;
};
