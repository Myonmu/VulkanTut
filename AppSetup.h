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
            QueueFamily::QUEUE_FAMILY_GRAPHICS | QueueFamily::QUEUE_FAMILY_PRESENT | QueueFamily::QUEUE_FAMILY_TRANSFER
        );
        //deviceContext.createWindow("extra window", 400, 300,
        //    QueueFamily::QUEUE_FAMILY_GRAPHICS | QueueFamily::QUEUE_FAMILY_PRESENT);
    }

    void createRenderPasses(DeviceContext &deviceContext) override {
        auto& window = deviceContext.get_windowContext_at(0);
        auto& color = window.get_colorAttachment();
        auto& depth = window.get_depthAttachment();
        auto& msaa = window.get_msaaAttachment();
        std::vector<AttachmentRef> attachments = {
            AttachmentRef{0, msaa, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL},
            AttachmentRef{1, depth, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL},
            AttachmentRef{2, color, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL},
        };
        const auto passId = deviceContext.create_renderPass(attachments);
        deviceContext.bindRenderPassToWindow(0, passId);
        //deviceContext.bindRenderPassToWindow(1, passId);
    }
};

class AppSetup final : public VulkanSetupProcedure {
    DeviceSetup deviceSetup{};
public:
    void createDeviceContexts(VulkanAppContext &appContext) override {
        appContext.createDeviceContext(deviceSetup);
    }
};



