//
// Created by miska on 2024/02/07.
//
#pragma once


#include <FileUtility.h>
#include <vector>
#include <optional>
#include <Texture2D.h>
#include <TextureImage.h>

#include "AppSetup.h"
#include "BindDescriptorSet.h"
#include "BindIndexBuffer.h"
#include "BindPipeline.h"
#include "BindVertexBuffer.h"
#include "DrawIndexed.h"
#include "EnqueueRenderPass.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "RenderPassRecorder.h"
#include "SetScissors.h"
#include "SetViewport.h"
#include "Shader.h"
#include "TextureAddressMode.h"
#include "TextureAnisotropyInfo.h"
#include "TextureCompareInfo.h"
#include "TextureFilterMode.h"
#include "TextureMipmapInfo.h"
#include "TextureSampler.h"
#include "GLFW/glfw3.h"
#include "VulkanAppContext.h"


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class TriangleApp {
public :
    void Run() {
        context = std::make_unique<VulkanAppContext>("VulkanApp", appSetup);
        setup();
        mainLoop();
    }

    ~TriangleApp() {
        delete sampler;
        delete textureImageView;
        delete textureImage;
        delete mainPass;
    }

private:
    AppSetup appSetup{};
    std::unique_ptr<VulkanAppContext> context;
    std::vector<Shader> shaders;
    std::unique_ptr<Material> material;
    std::unique_ptr<MaterialInstance> materialInstance;
    RenderPassRecorder *mainPass = nullptr;
    TextureImage *textureImage = nullptr;
    ImageView *textureImageView = nullptr;
    TextureSampler *sampler = nullptr;

    void setup() {
        auto& deviceCtx = *context->deviceContexts[0];

        shaders.emplace_back(FileUtility::ReadSpv("../shaders/vert.spv"), VK_SHADER_STAGE_VERTEX_BIT);
        shaders.emplace_back(FileUtility::ReadSpv("../shaders/frag.spv"), VK_SHADER_STAGE_FRAGMENT_BIT);

        material = std::make_unique<Material>(deviceCtx, shaders, deviceCtx.get_renderPass_at(0));
        materialInstance = std::make_unique<MaterialInstance>(*material);

        Texture2D t2d{"../textures/texture.jpg"};
        textureImage = new TextureImage(deviceCtx, t2d);
        textureImage->transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        textureImage->stage();
        textureImage->transitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        textureImageView = new ImageView(deviceCtx, textureImage->getRaw(), t2d.getFormat());

        sampler = new TextureSampler(deviceCtx,
                                     TextureAddressMode::REPEAT,
                                     TextureFilterMode::LINEAR,
                                     TextureAnisotropyInfo::getAutoAnisotropyInfo(),
                                     TextureCompareInfo(),
                                     TextureMipmapInfo::DEFAULT,
                                     VK_BORDER_COLOR_INT_OPAQUE_BLACK,
                                     VK_FALSE);

        materialInstance->setCombinedImageSampler(1, *textureImage, *sampler, *textureImageView);
        materialInstance->updateDescriptorSet(0, 0);

        mainPass = new RenderPassRecorder(deviceCtx.get_renderPass_at(0));
        mainPass->enqueueCommand<BindPipeline>( *material->pipeline , VK_PIPELINE_BIND_POINT_GRAPHICS);
        mainPass->enqueueCommand<SetViewport>();
        mainPass->enqueueCommand<SetScissors>();
        mainPass->enqueueCommand<BindVertexBuffer>();
        mainPass->enqueueCommand<BindIndexBuffer>();
        mainPass->enqueueCommand<BindDescriptorSet>(*material->pipelineLayout, *materialInstance->descriptorSets[0]);
        mainPass->enqueueCommand<DrawIndexed>(static_cast<uint32_t>(Vertex::testIndices.size()));
        //TODO: stub
        //auto &mainRecorder = context->frameDrawer;
        //mainRecorder.enqueueCommand<EnqueueRenderPass>(*mainPass);
    }

    void mainLoop() {
        for (auto& device : context->deviceContexts) {
            for (auto& window: device->windowContext) {
                window->get_renderer().drawFrame();
            }
        }
        /*
        while (!glfwWindowShouldClose(context->window)) {
            glfwPollEvents();
            context->drawFrame();
        }
        */
        vkDeviceWaitIdle(context->getLogicalDevice());
    }
};

