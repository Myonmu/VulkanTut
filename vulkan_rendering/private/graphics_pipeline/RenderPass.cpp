//
// Created by miska on 2024/02/11.
//

#include "RenderPass.h"

#include "DeviceContext.h"

RenderPass::~RenderPass() {
    vkDestroyRenderPass(ctx.getLogicalDevice(), resource, nullptr);
}

// TODO: make render pass configurable
RenderPass::RenderPass(DeviceContext &context, std::vector<AttachmentRef> &attachments)
    : VulkanResource(context) {
    std::vector<VkAttachmentDescription> attachmentDesc(attachments.size());
    std::vector<VkAttachmentReference> colorAttachments{};
    std::vector<VkAttachmentReference> depthStencilAttachments{};
    std::vector<VkAttachmentReference> resolveAttachments{};
    bool containsMsaa = false;
    for (auto &attachmentRef: attachments) {
        attachmentDesc[attachmentRef.index] = attachmentRef.description;
        if (attachmentRef.type == AttachmentType::MSAA) {
            containsMsaa = true;
        }
    }

    for (auto &attachmentRef: attachments) {
        auto ref = VkAttachmentReference{
            .attachment = attachmentRef.index,
            .layout = attachmentRef.layout
        };
        switch (attachmentRef.type) {
            case AttachmentType::PRESENT:
                if (containsMsaa)
                    resolveAttachments.push_back(ref);
                else
                    colorAttachments.push_back(ref);
                break;
            case AttachmentType::MSAA:
                colorAttachments.push_back(ref);
                break;
            case AttachmentType::DEPTH_STENCIL:
                depthStencilAttachments.push_back(ref);
                break;
        }
    }

    // TODO: dynamic subpass creation
    VkSubpassDescription mrt{};
    mrt.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    mrt.colorAttachmentCount = 1;
    mrt.pColorAttachments = colorAttachments.data();
    mrt.pDepthStencilAttachment = depthStencilAttachments.data();
    mrt.pResolveAttachments = resolveAttachments.data();

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDesc.size());
    renderPassInfo.pAttachments = attachmentDesc.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &mrt;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(ctx.getLogicalDevice(),
                           &renderPassInfo, nullptr,
                           &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}
