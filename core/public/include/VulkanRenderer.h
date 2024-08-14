//
// Created by miska on 2024/08/05.
//

#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H
#include <CommandBufferRecorder.h>
#include <vector>

class VulkanFrame;
struct VulkanAppContext;

class VulkanRenderer {
public:
    explicit VulkanRenderer(VulkanAppContext& context);
    ~VulkanRenderer();

    void drawFrame();
    void signalResize();
private:
    VulkanAppContext* context;
    std::vector<VulkanFrame*> frames;
    uint32_t currentFrame = 0;
};



#endif //VULKANRENDERER_H
