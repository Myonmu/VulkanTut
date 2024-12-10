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
        //                           QueueFamily::QUEUE_FAMILY_GRAPHICS | QueueFamily::QUEUE_FAMILY_PRESENT |
        //                           QueueFamily::QUEUE_FAMILY_TRANSFER);
    }

    void createRenderPasses(DeviceContext &deviceContext) override {
        auto &window = deviceContext.get_windowContext_at(0);
        auto &color = window.get_colorAttachment();
        auto &depth = window.get_depthAttachment();
        auto &albedo = window.get_gbufferAlbedo();
        auto &normal = window.get_gbufferNormal();
        auto &pos = window.get_gbufferPosition();

        const auto passId = deviceContext.create_renderPass();
        auto &pass = deviceContext.get_renderPass_at(passId);

        auto colorId = pass.addAttachment(&color);
        auto posId = pass.addAttachment(&pos);
        auto normalId = pass.addAttachment(&normal);
        auto albedoId = pass.addAttachment(&albedo);
        auto depthId = pass.addAttachment(&depth);


        auto &gbufferPass = pass.createSubpass();
        gbufferPass.useAttachment(colorId, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, ResourceUsageType::WRITE);
        gbufferPass.useAttachment(posId, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, ResourceUsageType::WRITE);
        gbufferPass.useAttachment(normalId, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, ResourceUsageType::WRITE);
        gbufferPass.useAttachment(albedoId, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, ResourceUsageType::WRITE);
        gbufferPass.useAttachment(depthId, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, ResourceUsageType::WRITE);

        auto &lightingPass = pass.createSubpass();
        lightingPass.useAttachment(posId, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, ResourceUsageType::READ);
        lightingPass.useAttachment(normalId, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, ResourceUsageType::READ);
        lightingPass.useAttachment(albedoId, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, ResourceUsageType::READ);
        lightingPass.useAttachment(depthId, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, ResourceUsageType::READ);
        lightingPass.useAttachment(colorId, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, ResourceUsageType::WRITE);

        SubpassDependency depE0_1{};
        /** NOTE
         * Why both EARLY and LATE fragment test?
         * When a shader doesn't write to depth stencil, we only have early frag test.
         * When a shader does write to depth stencil, we only have late frag test.
         * "OR-ing" ensures write on depth stencil is finished for either case.
         */
        depE0_1.setSrc(
                    VK_SUBPASS_EXTERNAL,
                    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, 0)
                .setDest(0, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                         VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT)
                .setFlags(0);
        pass.addDependency(depE0_1);

        SubpassDependency depE0_2{};
        depE0_2.setSrc(VK_SUBPASS_EXTERNAL, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0)
                .setDest(0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
                .setFlags(0);
        pass.addDependency(depE0_2);

        SubpassDependency dep01{};
        dep01.setSrc(0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
                .setDest(1, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT)
                .setFlags(VK_DEPENDENCY_BY_REGION_BIT);
        pass.addDependency(dep01);

        SubpassDependency dep1E{};
        dep1E.setSrc(1, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                     VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
                .setDest(VK_SUBPASS_EXTERNAL, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_MEMORY_READ_BIT)
                .setFlags(VK_DEPENDENCY_BY_REGION_BIT);
        pass.addDependency(dep1E);

        pass.build();

        window.createFrameBuffers(pass);
        //deviceContext.get_windowContext_at(1).createFrameBuffers(pass);
        //deviceContext.bindRenderPassToWindow(0, passId);
        //deviceContext.bindRenderPassToWindow(1, passId);
    }
};

class AppSetup final : public VulkanSetupProcedure {
    DeviceSetup deviceSetup{};

public:
    void createDeviceContexts(VulkanAppContext &appContext) override {
        appContext.create_deviceContexts(deviceSetup);
    }
};
