//
// Created by miska on 2024/08/05.
//

#pragma once

#include <CommandBufferRecorder.h>
#include <vector>

class DescriptorSetLayout;
class DescriptorSet;
class PerFrameDescriptorAllocator;
struct RenderingContext;
struct WindowContext;
class VulkanFrame;
struct VulkanAppContext;

class VulkanRenderer {
public:
    bool isPaused { false };
    std::unique_ptr<CommandBufferRecorder> recorder;

    explicit VulkanRenderer(WindowContext& context);
    ~VulkanRenderer();

    void drawFrame(RenderingContext& renderingCtx);
    void signalResize();

    void recordCommandBuffer(const CommandBuffer & command_buffer, const DeviceContext & context, FrameInfo frame_info) const;

    FrameInfo getCurrentFrameInfo() const;

    VulkanFrame& getCurrentFrame() const;

    DescriptorSet& getOrAllocatePerFrameDescriptorSet(DescriptorSetLayout& layout) const;
private:
    friend class VulkanFrame;
    WindowContext& ctx;
    std::unique_ptr<PerFrameDescriptorAllocator> perFrameAllocator;
    std::vector<std::unique_ptr<VulkanFrame>> frames;
    uint32_t currentFrame = 0;
};
