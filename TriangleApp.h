//
// Created by miska on 2024/02/07.
//

#ifndef VULKANTUT_TRIANGLEAPP_H
#define VULKANTUT_TRIANGLEAPP_H

#include <vector>
#include <optional>
#include <Texture2D.h>
#include <TextureImage.h>

#include "BindDescriptorSet.h"
#include "BindIndexBuffer.h"
#include "BindPipeline.h"
#include "BindVertexBuffer.h"
#include "DrawIndexed.h"
#include "EnqueueRenderPass.h"
#include "RenderPassRecorder.h"
#include "SetScissors.h"
#include "SetViewport.h"
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
        context = new VulkanAppContext(WIDTH, HEIGHT, "Vulkan");
        setup();
        mainLoop();
    }

    ~TriangleApp() {
        delete sampler;
        delete textureImageView;
        delete textureImage;
        delete mainPass;
        delete context;
    }

private:
    VulkanAppContext *context = nullptr;
    RenderPassRecorder *mainPass = nullptr;
    TextureImage *textureImage = nullptr;
    ImageView *textureImageView = nullptr;
    TextureSampler *sampler = nullptr;

    void setup() {
        Texture2D t2d{"../textures/texture.jpg"};
        textureImage = new TextureImage(*context, t2d);
        textureImage->transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        textureImage->stage();
        textureImage->transitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        textureImageView = new ImageView(*context, textureImage->getRaw(), t2d.getFormat());

        sampler = new TextureSampler(*context,
                                     TextureAddressMode::REPEAT,
                                     TextureFilterMode::LINEAR,
                                     TextureAnisotropyInfo::getAutoAnisotropyInfo(),
                                     TextureCompareInfo(),
                                     TextureMipmapInfo::DEFAULT,
                                     VK_BORDER_COLOR_INT_OPAQUE_BLACK,
                                     VK_FALSE);

        mainPass = new RenderPassRecorder(context->renderPass);
        mainPass->enqueueCommand<BindPipeline>(VK_PIPELINE_BIND_POINT_GRAPHICS);
        mainPass->enqueueCommand<SetViewport>();
        mainPass->enqueueCommand<SetScissors>();
        mainPass->enqueueCommand<BindVertexBuffer>();
        mainPass->enqueueCommand<BindIndexBuffer>();
        mainPass->enqueueCommand<BindDescriptorSet>();
        mainPass->enqueueCommand<DrawIndexed>(static_cast<uint32_t>(Vertex::testIndices.size()));
        //TODO: stub
        auto &mainRecorder = context->frameDrawer;
        mainRecorder.enqueueCommand<EnqueueRenderPass>(*mainPass);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(context->window)) {
            glfwPollEvents();
            context->drawFrame();
        }

        vkDeviceWaitIdle(context->logicalDevice);
    }
};


#endif //VULKANTUT_TRIANGLEAPP_H
