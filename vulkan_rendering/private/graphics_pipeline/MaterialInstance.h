//
// Created by Tereza on 10/29/2024.
//

#pragma once
#include <DescriptorSets.h>

#include "DescriptorWriter.h"


class Material;

class MaterialInstance {
    Material& srcMaterial;
    DeviceContext& ctx;
    std::unique_ptr<DescriptorSets> descriptorSets;
    DescriptorWriter descriptorWriter{};

public:
    explicit MaterialInstance(Material &material);
    ~MaterialInstance();
};
