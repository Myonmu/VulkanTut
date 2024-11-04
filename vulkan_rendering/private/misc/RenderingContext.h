//
// Created by miska on 2024/11/04.
//

#pragma once
#include <CommandBufferRecorder.h>
#include <map>

#include "Camera.h"
#include "DescriptorPoolAllocator.h"
#include "DescriptorSetLayoutBinding.h"
#include "glm/mat4x4.hpp"

struct PerSceneRenderingData {
    glm::mat4 view;
    glm::mat4 projection;
    static std::map<uint32_t, DescriptorSetLayoutBinding> bindings;
    static std::vector<DescriptorAllocator::PoolSizeRatio> poolSizes;
};

struct RenderingContext: public SubContext<DeviceContext> {
    Camera &camera;
    ObjectNode &sceneRoot;
    PerSceneRenderingData perSceneData{};
    CommandBufferRecorder recorder{0, false};

    RenderingContext(DeviceContext& ctx, Camera& camera, ObjectNode& sceneRoot);
    std::unique_ptr<DescriptorSetLayout> perSceneDescriptorLayout;
    std::unique_ptr<DescriptorAllocator> descriptorAllocator;
    void prepareFrame(const FrameInfo& frameInfo);
};
