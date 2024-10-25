//
// Created by miska on 2024/10/21.
//

#include "Material.h"

#include <PipelineLayout.h>
#include <set>

#include "DescriptorContext.h"
#include "Shader.h"
#include "ShaderReflectionResult.h"

Material::Material(DeviceContext& ctx, std::vector<Shader> shaders, RenderPass& renderPass) {
    for (auto& shader: shaders) {
        combinedReflectionResult.merge(shader.reflectionResult);
    }
    for(auto& [set, layouts] : combinedReflectionResult.descriptorSets) {
        descriptorContexts[set] = std::make_unique<DescriptorContext>(ctx, layouts);
        vkLayouts.push_back(descriptorContexts[set]->get_descriptorSetLayout());
        descriptorSets[set] = std::make_unique<DescriptorSets>(*descriptorContexts[set]);
    }
    pipelineLayout = std::make_unique<PipelineLayout>(ctx, vkLayouts);
    pipeline = std::make_unique<VulkanPipeline>(ctx, shaders, *pipelineLayout, renderPass);

}

Material::~Material() {

}
