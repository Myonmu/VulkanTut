//
// Created by miska on 2024/02/07.
//
#pragma once


#include <FileUtility.h>
#include <vector>
#include <optional>

#include "AppSetup.h"
#include "BindDescriptorSet.h"
#include "BindIndexBuffer.h"
#include "BindPipeline.h"
#include "BindVertexBuffer.h"
#include "DrawIndexed.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "RenderPassRecorder.h"
#include "SetScissors.h"
#include "SetViewport.h"
#include "Shader.h"
#include "TextureSampler.h"
#include "GLFW/glfw3.h"
#include "VulkanAppContext.h"
#include "UnifiedTexture2D.h"

class TriangleApp {
public :
    void Run() {
        glfwInit();
        context = std::make_unique<VulkanAppContext>("VulkanApp", appSetup);
        setup();
        //mainLoop();
    }

    ~TriangleApp() {
        delete mainPass;
    }

private:
    AppSetup appSetup{};
    std::unique_ptr<VulkanAppContext> context;
    std::vector<Shader> shaders;
    RenderPassRecorder *mainPass = nullptr;

    void setup() {
        auto &deviceCtx = *context->deviceContexts[0];
        auto f = FileUtility::ReadSpv("./shaders/vert.spv");
        shaders.emplace_back(f, VK_SHADER_STAGE_VERTEX_BIT);
        shaders.emplace_back(FileUtility::ReadSpv("./shaders/frag.spv"), VK_SHADER_STAGE_FRAGMENT_BIT);

        auto &material = deviceCtx.createObject<Material>(deviceCtx, shaders, deviceCtx.get_renderPass_at(0));
        auto &materialInstance = material.createInstance();

        auto const &tex = deviceCtx.createObject<UnifiedTexture2D>(deviceCtx, "./textures/texture.jpg");
        auto const &sampler = deviceCtx.createObject<TextureSampler>(deviceCtx,
                                                                     TextureAddressMode::REPEAT,
                                                                     TextureFilterMode::LINEAR,
                                                                     TextureAnisotropyInfo::getAutoAnisotropyInfo(),
                                                                     TextureCompareInfo(),
                                                                     TextureMipmapInfo::DEFAULT,
                                                                     VK_BORDER_COLOR_INT_OPAQUE_BLACK,
                                                                     VK_FALSE);

        materialInstance.setCombinedImageSampler(1, tex, sampler);
        return;
        materialInstance.updateDescriptorSet(0, 0);

        return;
        mainPass = new RenderPassRecorder(deviceCtx.get_renderPass_at(0));
        mainPass->enqueueCommand<BindPipeline>(material.get_pipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);
        mainPass->enqueueCommand<SetViewport>();
        mainPass->enqueueCommand<SetScissors>();
        mainPass->enqueueCommand<BindVertexBuffer>();
        mainPass->enqueueCommand<BindIndexBuffer>();
        mainPass->enqueueCommand<BindDescriptorSet>(material.get_pipelineLayout(), *materialInstance.descriptorSets[0]);
        mainPass->enqueueCommand<DrawIndexed>(static_cast<uint32_t>(Vertex::testIndices.size()));
        //TODO: stub
        //auto &mainRecorder = context->frameDrawer;
        //mainRecorder.enqueueCommand<EnqueueRenderPass>(*mainPass);
    }

    void mainLoop() {
        bool anyWindowAlive = false;
        do {
            for (auto const &device: context->deviceContexts) {
                for (auto const &window: device->windowContext) {
                    anyWindowAlive = true;
                    window->get_renderer().drawFrame();
                }
            }
        } while (anyWindowAlive);
    }
};
