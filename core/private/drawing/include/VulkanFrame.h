//
// Created by miska on 2024/08/06.
//

#ifndef VULKANFRAME_H
#define VULKANFRAME_H
#include <CommandBufferRecorder.h>
#include <vulkan/vulkan_core.h>


class CommandBuffer;
struct VulkanAppContext;

class VulkanFrame {
public:
    explicit VulkanFrame(VulkanAppContext& context);
    ~VulkanFrame();

    void drawFrame();
    void signalResize();
private:
    VulkanAppContext* context;
    CommandBuffer* commandBuffer;
    CommandBufferRecorder commandBufferRecorder{};

    VkSemaphore imageAvailableSemaphore{};
    VkSemaphore renderFinishedSemaphore{};
    VkFence inFlightFence{};
    uint32_t imageIndex = 0;

    bool frameBufferResized = false;
};



#endif //VULKANFRAME_H
