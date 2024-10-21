//
// Created by miska on 2024/08/05.
//

#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H
#include <CommandBufferRecorder.h>
#include <vector>

struct WindowContext;
class VulkanFrame;
struct VulkanAppContext;

class VulkanRenderer {
public:
    explicit VulkanRenderer(WindowContext& context);
    ~VulkanRenderer();

    void drawFrame();
    void signalResize();

    void recordCommandBuffer(const CommandBuffer & command_buffer, const DeviceContext & context, FrameInfo frame_info) const;

    std::unique_ptr<CommandBufferRecorder> recorder;
private:
    WindowContext& ctx;
    std::vector<std::unique_ptr<VulkanFrame>> frames;
    uint32_t currentFrame = 0;
};



#endif //VULKANRENDERER_H
