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

    void createRenderPasses(DeviceContext &deviceContext) override {
        auto colorFormat = deviceContext.get_windowContext_at(0).get_colorAttachment().get_format();
        auto depthFormat = deviceContext.get_windowContext_at(0).get_depthAttachment().get_format();
        const auto passId = deviceContext.create_renderPass(colorFormat, depthFormat);
        deviceContext.bindRenderPassToWindow(0,passId);
    }
};

class AppSetup final : public VulkanSetupProcedure {
    DeviceSetup deviceSetup{};
public:
    void createDeviceContexts(VulkanAppContext &appContext) override {
        appContext.createDeviceContext(deviceSetup);
    }
};



