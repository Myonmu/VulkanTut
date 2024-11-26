//
// Created by miska on 2024/11/17.
//

#include <EngineDescriptorDef.h>
#include "Camera.h"
#include "Lights.h"

EngineDescriptorSetCreateInfo EngineDescriptorDef::getSet(uint32_t setId) {
    return descriptorSets[setId];
}


#define BINDING_DECL_UBO(i) {i, DescriptorSetLayoutBinding{.binding = (i), .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}},
#define BINDING_DECL_UBO_SIZE(i, type) {static_cast<uint32_t>(i), static_cast<uint32_t>(sizeof(type))},

std::map<uint32_t, EngineDescriptorSetCreateInfo> EngineDescriptorDef::descriptorSets{
    // set 0: per-frame
    {
        0,
        EngineDescriptorSetCreateInfo{
            .setId = 0,
            .bindings = {
                BINDING_DECL_UBO(0) // Camera ubo data
                BINDING_DECL_UBO(1) // Main light
            },
            .poolSizes = {
                DescriptorAllocator::PoolSizeRatio{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .ratio = 2.f}
            },
            .bufferSizes = {
                BINDING_DECL_UBO_SIZE(0, CameraUboData) // Camera ubo data
                BINDING_DECL_UBO_SIZE(1, MainLightUboData) // Main light
            }
        }
    },
};
