//
// Created by miska on 2024/02/11.
//

#pragma once

#include <optional>
#include <vector>
#include "VulkanResource.h"
#include <vulkan/vulkan_core.h>
#include "ContextMacros.h"
#include "RenderGraph.h"

class RenderAttachment;
struct AttachmentInfo;
struct DeviceContext;

class RenderPass;

class Subpass {
    RenderPass &renderPass;
public:
    VkSubpassDescriptionFlags flags = 0;
    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    std::vector<VkAttachmentReference> inputAttachments;
    std::vector<VkAttachmentReference> colorAttachments;
    std::vector<uint32_t> preserveAttachments;
    std::optional<VkAttachmentReference> depthStencilAttachment;
    std::vector<VkAttachmentReference> resolveAttachments;

    Subpass(RenderPass &renderPass): renderPass(renderPass) {
    };

    void useAttachment(uint32_t id, VkImageLayout layout, ResourceUsageDeclType usageType);

    void preserveAttachment(uint32_t id);

    [[nodiscard]] VkSubpassDescription getSubpassDescription() const;
};

class SubpassDependency {
    VkSubpassDependency dependency{};
public:
    SubpassDependency& setSrc(uint32_t id, VkPipelineStageFlags stageFlags, VkAccessFlags accessFlags);
    SubpassDependency& setDest(uint32_t id, VkPipelineStageFlags stageFlags, VkAccessFlags accessFlags);
    SubpassDependency& setFlags(VkDependencyFlags flags);
    [[nodiscard]] VkSubpassDependency getSubpassDependency() const;
};

class RenderPass : public VulkanResource<VkRenderPass, DeviceContext> {
    friend class Subpass;
    std::vector<RenderAttachment *> attachments;
    std::vector<std::unique_ptr<Subpass> > subpasses;
    std::vector<VkSubpassDependency> subpassDependencies;
    bool msaaEnabled = false;

public:
    explicit RenderPass(DeviceContext &context);

    void build();

    ~RenderPass() override;

    Subpass &createSubpass();

    uint32_t addAttachment(RenderAttachment *attachment);

    void addDependency(SubpassDependency &dependency);

    [[nodiscard]] Subpass& getSubpass(const uint32_t id) const {
        return *subpasses[id];
    }
};