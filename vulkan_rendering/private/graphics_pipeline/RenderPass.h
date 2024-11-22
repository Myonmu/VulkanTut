//
// Created by miska on 2024/02/11.
//

#pragma once


#include <optional>
#include <vector>
#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>

struct AttachmentRef;
struct DeviceContext;

class Subpass {
    VkSubpassDescriptionFlags flags = 0;
    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    std::vector<VkAttachmentReference> inputAttachments;
    std::vector<VkAttachmentReference> colorAttachments;
    std::vector<uint32_t> preserveAttachments;
    std::optional<VkAttachmentReference> depthStencilAttachment;
    std::vector<VkAttachmentReference> resolveAttachments;
public:
    [[nodiscard]] VkSubpassDescription getSubpassDescription() const;
};

class SubpassDependency {
public:
    [[nodiscard]] VkSubpassDependency getSubpassDependency() const;
};

class RenderPass : public VulkanResource<VkRenderPass, DeviceContext>{
    std::vector<VkAttachmentDescription> attachments;
    std::vector<Subpass*> subpasses;
    std::vector<VkSubpassDescription> subpassDescriptions;
    std::vector<VkSubpassDependency> subpassDependencies;
public:
    explicit RenderPass(DeviceContext& context);
    ~RenderPass() override;

    void addSubpass(Subpass& subpass);
};

