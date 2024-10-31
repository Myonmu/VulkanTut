//
// Created by miska on 2024/10/21.
//
#include "Material.h"
#include <DescriptorSetLayout.h>
#include <PipelineLayout.h>
#include <set>
#include "DeviceContext.h"
#include "Shader.h"
#include "ShaderReflectionResult.h"
#include "MaterialInstance.h"

Material::Material(DeviceContext& ctx, std::vector<Shader>& shaders, RenderPass& renderPass):
ctx(ctx)
{
    for (auto& shader: shaders) {
        combinedReflectionResult.merge(shader.reflectionResult);
    }

    descriptorAllocator = std::make_unique<DescriptorAllocator>(ctx);
    auto stats = combinedReflectionResult.getCountByType();
    std::vector<DescriptorAllocator::PoolSizeRatio> ratios;
    for (auto const&[type, count]: stats) {
        ratios.push_back({type, static_cast<float>(count)});
    }
    descriptorAllocator->init(100, ratios);

    std::vector<VkDescriptorSetLayout> vkLayouts;
    for(auto& [set, layouts] : combinedReflectionResult.descriptorSets) {
        descriptorSetLayouts[set] = std::make_unique<DescriptorSetLayout>(ctx, layouts);
        vkLayouts.push_back(descriptorSetLayouts[set]->getRaw());
    }
    pipelineLayout = std::make_unique<PipelineLayout>(ctx, vkLayouts);
    pipeline = std::make_unique<VulkanPipeline>(ctx, shaders, *pipelineLayout, renderPass);
}

MaterialInstance &Material::createInstance() {
    return createChildObject<MaterialInstance>(*this);
}

