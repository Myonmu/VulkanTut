//
// Created by miska on 2024/08/06.
//

#pragma once

#include <CommandBufferRecorder.h>
#include <vulkan/vulkan_core.h>
#include <map>
#include "DescriptorPoolAllocator.h"
#include "DescriptorSetLayout.h"
#include "DescriptorWriter.h"
#include "EngineDescriptorDef.h"
#include "FrameInfo.h"

struct RenderingContext;
struct WindowContext;
class CommandBuffer;

#define PER_FRAME_SET_ID 0

class PerFrameDescriptorAllocator {
    friend struct PerFrameDescriptorSet;

    std::unique_ptr<DescriptorSetLayout> layout;
    std::unique_ptr<DescriptorAllocator> allocator;
    EngineDescriptorSetCreateInfo createInfo;

    [[nodiscard]] std::unique_ptr<DescriptorSet> allocate() const;
public:
    explicit PerFrameDescriptorAllocator(WindowContext& ctx);
    PerFrameDescriptorAllocator(WindowContext& ctx, EngineDescriptorSetCreateInfo createInfo);
    ~PerFrameDescriptorAllocator() = default;
};

// Set 1, binding 0 = camera data
struct PerFrameDescriptorSet {
    const WindowContext& ctx;
    std::unique_ptr<DescriptorSet> perFrameSet;
    std::map<uint32_t, std::unique_ptr<Buffer>> buffers;
    std::unique_ptr<DescriptorWriter> writer;
    PerFrameDescriptorSet(WindowContext& ctx, PerFrameDescriptorAllocator& allocator);
    void writeBuffer(uint32_t binding, void* data, uint32_t size);
    void updateSet() const;
};

class VulkanFrame {
public:
    VulkanFrame(WindowContext &context, VulkanRenderer& renderer);

    ~VulkanFrame();

    void drawFrame(uint32_t currentFrameIndex, RenderingContext &renderingCtx);

    void signalResize();

    CTX_PROPERTY(PerFrameDescriptorSet, perFrameDescriptorSet)

    DescriptorSet& allocatePerFrameDescriptorSetTemp(DescriptorSetLayout& layout);
private:
    WindowContext &context;
    CommandBuffer commandBuffer;
    FrameInfo frameInfo{};

    DescriptorAllocatorCollection freeformAllocator;
    std::vector<std::unique_ptr<DescriptorSet>> tempDescriptors;

    VkSemaphore imageAvailableSemaphore{};
    VkSemaphore renderFinishedSemaphore{};
    VkFence inFlightFence{};
    uint32_t imageIndex = 0;

    bool frameBufferResized = false;
};
