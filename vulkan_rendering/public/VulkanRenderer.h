//
// Created by miska on 2024/08/05.
//

#pragma once

#include <CommandBufferRecorder.h>
#include <vector>

struct RenderingContext;
struct WindowContext;
class VulkanFrame;
struct VulkanAppContext;

class VulkanRenderer {
public:
    explicit VulkanRenderer(WindowContext& context);
    ~VulkanRenderer();

    void drawFrame(RenderingContext& renderingCtx);
    void signalResize();

    void recordCommandBuffer(const CommandBuffer & command_buffer, const DeviceContext & context, FrameInfo frame_info) const;

    std::unique_ptr<CommandBufferRecorder> recorder;

    FrameInfo getCurrentFrameInfo() const;
private:
    WindowContext& ctx;
    std::vector<std::unique_ptr<VulkanFrame>> frames;
    uint32_t currentFrame = 0;
};
