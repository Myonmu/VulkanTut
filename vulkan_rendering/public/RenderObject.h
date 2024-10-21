//
// Created by miska on 2024/10/13.
//

#pragma once

#include "UniformBufferGroup.h"
#include "DescriptorSets.h"

class RenderObject {
public:
    DescriptorSets descriptorSets;
    UniformBufferGroup uniformBufferGroup;

    std::vector<ImageView*> images{};
    std::vector<TextureSampler*> samplers{};

    RenderObject();
    ~RenderObject();
};