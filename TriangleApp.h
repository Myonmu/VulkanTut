//
// Created by miska on 2024/02/07.
//
#pragma once

#include <flecs.h>
#include <chrono>
#include <FileUtility.h>
#include <vector>
#include "AppSetup.h"
#include "CBC_Drawing.h"
#include "CBC_Misc.h"
#include "Material.h"
#include "RenderPassRecorder.h"
#include "Shader.h"
#include "TextureSampler.h"
#include "VulkanAppContext.h"
#include "UnifiedTexture2D.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "Vertex.h"
#include <fmt/core.h>
#include "Camera.h"
#include "EcsSystemsHeader.h"
#include "EnginePipeline.h"
#include "ObjLoader.h"
#include "RenderingContext.h"
#include "fmt/color.h"
#include "libs/SDL/include/SDL3/SDL_init.h"
#include "scripts/FlycamSystem.h"
#include "scripts/MeshRotate.h"

class TriangleApp {
public :
    void Run() {
        SDL_Init(SDL_INIT_VIDEO);
        context = std::make_unique<VulkanAppContext>("VulkanApp", appSetup);
        setup();
        mainLoop();
        SDL_Quit();
    }

private:
    uint64_t frameCounter = 0;
    AppSetup appSetup{};
    std::unique_ptr<VulkanAppContext> context;
    flecs::world ecs{};
    std::vector<Shader> shaders;
    std::unique_ptr<RenderPassRecorder> mainPass;
    EnginePipeline enginePipeline{};
    std::unique_ptr<RenderingContext> mainContext;
    std::unique_ptr<EventSystem> eventSystem;
    flecs::entity transformPrefab;
    ObjLoader obj{};
    void setup() {
        eventSystem = std::make_unique<EventSystem>(ecs);
        EcsSystemsHeader systems_header{};
        systems_header.createSystems(ecs);

        transformPrefab = ecs.prefab("Transform").add<Position>().add<Rotation>().add<Scale>();


        auto &deviceCtx = *context->deviceContexts[0];
        auto f = FileUtility::ReadSpv("./shaders/shader.vert.spv");
        shaders.emplace_back(f, VK_SHADER_STAGE_VERTEX_BIT);
        shaders.emplace_back(FileUtility::ReadSpv("./shaders/shader.frag.spv"), VK_SHADER_STAGE_FRAGMENT_BIT);

        auto &material = deviceCtx.createObject<Material>(deviceCtx, shaders, deviceCtx.get_renderPass_at(0));
        auto &materialInstance = material.createInstance();

        auto const &tex = deviceCtx.createObject<UnifiedTexture2D>(deviceCtx, "./assets/viking_room.png");
        auto const &sampler = deviceCtx.createObject<TextureSampler>(deviceCtx,
                                                                     TextureAddressMode::REPEAT,
                                                                     TextureFilterMode::LINEAR,
                                                                     TextureAnisotropyInfo::getAutoAnisotropyInfo(),
                                                                     TextureCompareInfo(),
                                                                     TextureMipmapInfo::DEFAULT,
                                                                     VK_BORDER_COLOR_INT_OPAQUE_BLACK,
                                                                     VK_FALSE);

        materialInstance.setCombinedImageSampler(0, tex, sampler);
        materialInstance.updateDescriptorSet(1);

        obj.LoadGeometry("./assets/viking_room.obj");
        auto &meshBuffer = deviceCtx.createObject<MeshBuffer>(deviceCtx, obj.vertices, obj.indices);
        auto &entt = ecs.entity("Some Object")

                .emplace<MeshRenderer>(deviceCtx, meshBuffer, materialInstance)
                .add<MeshRotate>().is_a(transformPrefab);


        mainPass = std::make_unique<RenderPassRecorder>(deviceCtx.get_renderPass_at(0));
        auto &mainRenderer = deviceCtx.get_windowContext_at(0).get_renderer();
        mainRenderer.recorder->enqueueCommand<EnqueueRenderPass>(*mainPass);

        auto &swapchain = deviceCtx.get_windowContext_at(0).get_swapChain();
        auto &camera = ecs.entity("Camera")
                .is_a(transformPrefab).set(Position{{0,0,2}})
                .add<Velocity>()
                .emplace<Camera>(&swapchain)
                .add<Flycam>();

        mainContext = std::make_unique<RenderingContext>(deviceCtx);
        mainContext->renderer = &mainRenderer;

        mainContext->f = [this]() { return prepareRenderLoop(); };

        enginePipeline.addLoop([this] { return eventLoop(); });
        enginePipeline.addLoop([this] { return renderLoop(); });
    }

    bool eventLoop() {
        //event processing
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            eventSystem->pushSdlEvent(e);
            // window events
            auto windowId = e.window.windowID;
            try {
                auto &windowCtx = context->deviceContexts[0]->getWindowContextFromSdlId(windowId);
                switch (e.type) {
                    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                        windowCtx.closeWindow();
                        break;
                    case SDL_EVENT_WINDOW_RESIZED:
                        windowCtx.resize();
                        break;
                    case SDL_EVENT_WINDOW_MINIMIZED:
                        windowCtx.get_renderer().isPaused = true;
                        break;
                    case SDL_EVENT_WINDOW_RESTORED:
                        windowCtx.get_renderer().isPaused = false;
                        break;
                    default: continue;
                }
            } catch (std::runtime_error const &ex) {
                continue;
            }
        }
        return true;
    }

    bool prepareRenderLoop() {
        ecs.system<Position, Rotation, Scale, MeshRenderer>("UpdatePerObjectUBO").kind(flecs::PreStore).each(
            [](Position &p, Rotation &r, Scale &s, MeshRenderer &renderer) {
                const auto modelMatrix = Transform::getModelMatrix(p, r, s);
                renderer.vertexPushConstants.model = modelMatrix;
            }
        );
        /*
        ecs.system<Position, Rotation, Scale, MeshRendererSplitBuffer>("UpdatePerObjectUBOSplit").kind(flecs::PreStore).each(
            [](Position &p, Rotation &r, Scale &s, MeshRendererSplitBuffer &renderer) {
                const auto modelMatrix = Transform::getModelMatrix(p, r, s);
                renderer.vertexPushConstants.model = modelMatrix;
            }
        );
        */
        mainPass->clear();
        mainPass->enqueueCommand<SetViewport>();
        mainPass->enqueueCommand<SetScissors>();
        ecs.system<MeshRenderer>("RenderMesh").kind(flecs::OnStore).each(
            [this](MeshRenderer &renderer) {
                renderer.enqueueDrawCall(*mainContext, *mainPass);
            });
        /*
        ecs.system<MeshRendererSplitBuffer>("RenderMeshSplit").kind(flecs::OnStore).each(
            [this](MeshRendererSplitBuffer &renderer) {
                renderer.enqueueDrawCall(*mainContext, *mainPass);
            });
        */
        ecs.system<Rotation, Position, Camera>("RenderCamera").kind(flecs::OnStore).each(
            [this](Rotation &t, Position &p, Camera &cam) {
                auto &perSceneData = mainContext->perSceneData;
                perSceneData.projection = cam.getProjectionMatrix();
                perSceneData.view = Camera::getViewMatrix(p, t);
            }
        );
        if (const auto result = ecs.progress(); !result) {
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
                window->get_renderer().drawFrame(*mainContext);
            }
        }
        return anyWindowAlive;
    }

    void mainLoop() {
        auto shouldExit = false;
        while (!shouldExit) {
            //fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold, "vvvvvv FRAME {}\n", frameCounter);
            shouldExit = !enginePipeline.mainLoop();
            //fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "^^^^^^ END FRAME {}\n", frameCounter);
            frameCounter++;
        }
        vkDeviceWaitIdle(context->deviceContexts[0]->getLogicalDevice());
    }
};
