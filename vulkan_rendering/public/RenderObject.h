//
// Created by miska on 2024/10/13.
//

#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H
#include "UniformBufferGroup.h"
#include "DescriptorSets.h"


class RenderObject {
public:
    DescriptorSets descriptorSets;
    UniformBufferGroup uniformBufferGroup;
    std::vector<ImageView*> images{};
    std::vector<TextureSampler*> samplers{};
};



#endif //RENDEROBJECT_H
