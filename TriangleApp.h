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
#include "SDL3/SDL.h"
#include "scripts/FlycamSystem.h"
#include "scripts/MeshRotate.h"

class TriangleApp {
public :
    void Run() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error(SDL_GetError());
        }
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


        auto &deviceCtx = context->get_deviceContexts_at(0);
        auto f = FileUtility::ReadSpv("./shaders/shader.vert.spv");
        shaders.emplace_back(f, VK_SHADER_STAGE_VERTEX_BIT);
        shaders.emplace_back(FileUtility::ReadSpv("./shaders/shader.frag.spv"), VK_SHADER_STAGE_FRAGMENT_BIT);

        auto &material = deviceCtx.createObject<Material>(deviceCtx, shaders, deviceCtx.get_renderPass_at(0));
        auto &materialInstance = material.createInstance();

        auto const &tex =
                deviceCtx.createObject<UnifiedTexture2D>(deviceCtx, "./assets/viking_room.png", VK_FILTER_LINEAR);
        auto const &sampler = deviceCtx.createObject<TextureSampler>(
            deviceCtx,
            TextureAddressMode::REPEAT,
            TextureFilterMode::LINEAR,
            TextureAnisotropyInfo::getAutoAnisotropyInfo(),
            TextureCompareInfo(),
            TextureMipmapInfo{
                .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
                .mipmapBias = 0.0f,
                .minLod = 0,
                .maxLod = static_cast<float>(tex.get_cpuTexture().getMaxMipLevel())
            },
            VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            VK_FALSE);


        materialInstance.setCombinedImageSampler(0, tex, sampler);
        materialInstance.updateDescriptorSet(1);

        obj.LoadGeometry("./assets/viking_room.obj");
        auto &meshBuffer = deviceCtx.createObject<MeshBuffer>(deviceCtx, obj.vertices, obj.indices);
        //auto &vertexBuffer = deviceCtx.createObject<VertexBuffer>(deviceCtx, obj.vertices);
        //auto &indexBuffer = deviceCtx.createObject<IndexBuffer>(deviceCtx,obj.indices);
        auto &entt = ecs.entity("Some Object")
                .emplace<MeshRenderer>(deviceCtx, meshBuffer, materialInstance)
                //.emplace<MeshRendererSplitBuffer>(deviceCtx, vertexBuffer, indexBuffer, materialInstance)
                //.add<MeshRotate>()
                .is_a(transformPrefab);


        auto &light = ecs.entity("Main Light")
                .set(MainLight{
                    .direction = glm::normalize(glm::vec3{-1.f, -2.f, -1.f}),
                    .color = {1.f, 1.f, 1.f}
                });


        mainPass = std::make_unique<RenderPassRecorder>(deviceCtx.get_renderPass_at(0));
        auto &mainRenderer = deviceCtx.get_windowContext_at(0).get_renderer();
        mainRenderer.recorder->enqueueCommand<EnqueueRenderPass>(*mainPass);
        auto &otherRenderer = deviceCtx.get_windowContext_at(1).get_renderer();
        otherRenderer.recorder->enqueueCommand<EnqueueRenderPass>(*mainPass);

        auto &swapchain = deviceCtx.get_windowContext_at(0).get_swapChain();
        auto &camera = ecs.entity("Camera")
                .is_a(transformPrefab).set(Position{{0, 0, 2}})
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
                auto &windowCtx = context->get_deviceContexts_at(0).getWindowContextFromSdlId(windowId);
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
        mainPass->clear();
        mainPass->enqueueCommand<SetViewport>();
        mainPass->enqueueCommand<SetScissors>();

        // TODO: Move to ECS system (currently they depend on main context and main pass, refactor this dependency)
        ecs.system<MeshRenderer>("RenderMesh").kind(flecs::OnStore).each(
            [this](MeshRenderer &renderer) {
                renderer.enqueueDrawCall(*mainContext, *mainPass);
            });

        ecs.system<MeshRendererSplitBuffer>("RenderMeshSplit").kind(flecs::OnStore).each(
            [this](MeshRendererSplitBuffer &renderer) {
                renderer.enqueueDrawCall(*mainContext, *mainPass);
            });

        ecs.system<Rotation, Position, Camera>("RenderCamera").kind(flecs::OnStore).each(
            [this](Rotation &t, Position &p, Camera &cam) {
                auto &cameraData = mainContext->cameraUboData;
                cameraData.projection = cam.getProjectionMatrix();
                cameraData.view = Camera::getViewMatrix(p, t);
            }
        );
        //TODO: currently only support 1 main light, subsequent main lights overwrite the previous
        ecs.system<MainLight>("MainLight").kind(flecs::OnStore).each(
            [this](MainLight &l) {
                auto &lightData = mainContext->mainLightUboData;
                lightData.color = glm::vec4(l.color, 1.0f);
                lightData.direction = glm::vec4(l.direction, 0.0f);
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
                mainContext->renderer = &window->get_renderer();
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
        vkDeviceWaitIdle(context->get_deviceContexts_at(0).getLogicalDevice());
    }
};
