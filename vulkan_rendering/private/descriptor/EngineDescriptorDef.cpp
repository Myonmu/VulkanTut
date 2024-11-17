//
// Created by miska on 2024/11/17.
//

#include <EngineDescriptorDef.h>
#include "Camera.h"

EngineDescriptorSetCreateInfo EngineDescriptorDef::getSet(uint32_t setId) {
    return descriptorSets[setId];
}


#define BINDING_DECL_UBO(i) {i, DescriptorSetLayoutBinding{.binding = (i), .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}},

std::map<uint32_t, EngineDescriptorSetCreateInfo> EngineDescriptorDef::descriptorSets{
    // set 0: per-frame
    {
        0,
        EngineDescriptorSetCreateInfo{
            .setId = 0,
            .bindings = {
                // Camera ubo data
                BINDING_DECL_UBO(0)
            },
            .poolSizes = {
                DescriptorAllocator::PoolSizeRatio{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .ratio = 1.f}
            },
            .bufferSizes = {
                {static_cast<uint32_t>(0), static_cast<uint32_t>(sizeof(CameraUboData))} // Camera ubo data
            }
        }
    },
};
