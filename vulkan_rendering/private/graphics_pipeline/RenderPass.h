//
// Created by miska on 2024/02/11.
//

#pragma once


#include <vector>
#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>

struct AttachmentRef;
struct DeviceContext;

class SubPass {

};

class RenderPass : public VulkanResource<VkRenderPass, DeviceContext>{
public:
    RenderPass(DeviceContext& context, std::vector<AttachmentRef>& attachments);
    ~RenderPass() override;
};

