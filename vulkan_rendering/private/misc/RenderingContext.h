//
// Created by miska on 2024/11/04.
//

#pragma once
#include <CommandBufferRecorder.h>
#include <map>
#include "DescriptorPoolAllocator.h"
#include "DescriptorSetLayoutBinding.h"
#include "PerFrameBufferGroup.h"

struct PerSceneRenderingData {
    glm::mat4 view;
    glm::mat4 projection;
    static std::map<uint32_t, DescriptorSetLayoutBinding> bindings;
    static std::vector<DescriptorAllocator::PoolSizeRatio> poolSizes;
};

struct RenderingContext: public SubContext<DeviceContext> {
    PerSceneRenderingData perSceneData{};
    CommandBufferRecorder recorder{0, false};
    std::unique_ptr<PerFrameBufferGroup> perSceneUbo;
    std::vector<std::unique_ptr<DescriptorSet>> perFrameSets;

    VulkanRenderer* renderer;

    explicit RenderingContext(DeviceContext& ctx);
    std::unique_ptr<DescriptorSetLayout> perSceneDescriptorLayout;
    std::unique_ptr<DescriptorAllocator> descriptorAllocator;
    void prepareFrame(const FrameInfo& frameInfo);

    std::function<bool()> f;

    LogicalDevice &getLogicalDevice() const override;
};
