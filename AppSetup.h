//
// Created by miska on 2024/10/20.
//

#pragma once

#include <QueueFamilyIndices.h>
#include <VulkanAppContext.h>

#include "DeviceContext.h"
#include "VulkanSetupProcedure.h"

class DeviceSetup final : public VulkanDeviceSetupProcedure {
public:
    void createWindows(DeviceContext &deviceContext) override {
        deviceContext.createWindow(
            deviceContext.context.name, 800, 600,
            QueueFamily::QUEUE_FAMILY_GRAPHICS | QueueFamily::QUEUE_FAMILY_PRESENT
        );
    }

    void createDescriptorContexts(DeviceContext &deviceContext) override {
        deviceContext.create_descriptorContext(std::vector<DescriptorSetLayoutBinding>{
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        });
    }

    void createRenderPasses(DeviceContext &deviceContext) override {
        auto format = deviceContext.get_windowContext_at(0).swapChain->swapChainImageFormat;
        const auto passId = deviceContext.create_renderPass(format);
        deviceContext.bindRenderPassToWindow(0,passId);
    }
};

class AppSetup final : public VulkanSetupProcedure {
public:
    void createDeviceContexts(VulkanAppContext &appContext) override {
        appContext.createDeviceContext<AppSetup>();
    }
};



