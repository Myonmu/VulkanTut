//
// Created by miska on 2024/10/21.
//

#pragma once
#include <VulkanPipeline.h>


struct DeviceContext;

class Material {

    std::unique_ptr<VulkanPipeline> pipeline;
    Material(const std::vector<char>& code ,DeviceContext& context);
    ~Material();
};
