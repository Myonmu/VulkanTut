//
// Created by miska on 2024/08/06.
//

#pragma once

#include <CommandBufferRecorder.h>
#include <vulkan/vulkan_core.h>

#include "DescriptorPoolAllocator.h"
#include "FrameInfo.h"


struct RenderingContext;
struct WindowContext;
class CommandBuffer;

struct PerFrameResources {
    std::unique_ptr<DescriptorAllocator> frameDescriptorAllocator;
};

class VulkanFrame {
public:
    explicit VulkanFrame(WindowContext& context);
    ~VulkanFrame();

    void drawFrame(uint32_t currentFrameIndex, RenderingContext& renderingCtx);

    void signalResize();
private:
    WindowContext& context;
    CommandBuffer commandBuffer;
    FrameInfo frameInfo{};

    VkSemaphore imageAvailableSemaphore{};
    VkSemaphore renderFinishedSemaphore{};
    VkFence inFlightFence{};
    uint32_t imageIndex = 0;

    bool frameBufferResized = false;
};

