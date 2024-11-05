//
// Created by miska on 2024/02/07.
//
#pragma once


#include <FileUtility.h>
#include <vector>
#include "AppSetup.h"
#include "CBC_Drawing.h"
#include "CBC_Misc.h"
#include "Material.h"
#include "RenderPassRecorder.h"
#include "Shader.h"
#include "TextureSampler.h"
#include "GLFW/glfw3.h"
#include "VulkanAppContext.h"
#include "UnifiedTexture2D.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "Vertex.h"
#include "flecs.h"
#include <fmt/core.h>

#include "Camera.h"
#include "Ecs.h"
#include "EnginePipeline.h"

class TriangleApp {
public :
    void Run() {
        glfwInit();
        context = std::make_unique<VulkanAppContext>("VulkanApp", appSetup);
        setup();
        mainLoop();
    }

private:
    AppSetup appSetup{};
    std::unique_ptr<VulkanAppContext> context;
    World ecs{};
    std::vector<Shader> shaders;
    std::unique_ptr<RenderPassRecorder> mainPass;
    EnginePipeline enginePipeline{};

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
        materialInstance.updateDescriptorSet(0, 0);

        auto &meshBuffer = deviceCtx.createObject<MeshBuffer>(deviceCtx, Vertex::testVerts, Vertex::testIndices);
        auto entt = ecs.createEntityWithTransform();
        entt.addComponent<MeshRenderer>(meshBuffer, materialInstance);
        mainPass = std::make_unique<RenderPassRecorder>(deviceCtx.get_renderPass_at(0));
        const auto &mainRecorder = deviceCtx.get_windowContext_at(0).get_renderer();
        mainRecorder.recorder->enqueueCommand<EnqueueRenderPass>(*mainPass);

        auto camera = ecs.createEntityWithTransform("Camera");
        camera.addComponent<Camera>();

        enginePipeline.addLoop([this] { return prepareRenderLoop(); });
        enginePipeline.addLoop([this] { return renderLoop(); });
    }

    bool prepareRenderLoop() {
        mainPass->clear();
        mainPass->enqueueCommand<SetViewport>();
        mainPass->enqueueCommand<SetScissors>();
        ecs.getRaw().system<Transform, Camera>().kind(flecs::OnStore).each(
            [this](Transform &t, Camera &cam) {
            }
        );
        ecs.getRaw().system<MeshRenderer>("RenderMesh").kind(flecs::OnStore).each(
            [this](MeshRenderer &renderer) {
                renderer.enqueueDrawCall(*mainPass);
            });
        if (const auto result = ecs.getRaw().progress(); !result) {
            fmt::println("Failed to progress ECS");
            return false;
        }
        return true;
    }

    bool renderLoop() {
        bool anyWindowAlive = false;
        for (auto const &device: context->deviceContexts) {
            for (auto const &window: device->windowContext) {
                anyWindowAlive = true;
                window->get_renderer().drawFrame();
            }
        }
        return anyWindowAlive;
    }

    void mainLoop() {
        while (enginePipeline.mainLoop()) {
        }
    }
};
