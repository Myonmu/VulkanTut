//
// Created by miska on 2024/11/04.
//

#pragma once
#include <CommandBufferRecorder.h>
#include <functional>
#include "Camera.h"
#include "DescriptorPoolAllocator.h"
#include "Lights.h"


struct PerObjectVertexPushConstants {
    glm::mat4 model;

    // Used by pipeline layout
    static VkPushConstantRange getPushConstantsRange();
};


struct RenderingContext: public SubContext<DeviceContext> {
    CameraUboData cameraUboData{};
    MainLightUboData mainLightUboData{};
    VulkanRenderer* renderer;

    explicit RenderingContext(DeviceContext& ctx);
    std::unique_ptr<DescriptorSetLayout> perSceneDescriptorLayout;
    std::unique_ptr<DescriptorAllocator> descriptorAllocator;
    void prepareFrame(const FrameInfo& frameInfo);

    std::function<bool()> f;

    LogicalDevice &getLogicalDevice() const override;
};
