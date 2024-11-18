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
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = colorAttachments.size();
    subpass.pColorAttachments = colorAttachments.data();
    subpass.pDepthStencilAttachment = depthStencilAttachments.data();
    subpass.pResolveAttachments = resolveAttachments.data();

    // This is just the GBuffer pass, NOT the lighting pass. The example splits them into 2 separate render passes.
    // But it is more optimal to combine them into the same pass but with 2 sub passes.
    VkSubpassDependency dependency1{};
    dependency1.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency1.dstSubpass = 0;
    dependency1.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ;
    dependency1.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency1.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependency1.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency1.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkSubpassDependency dependency2{};
    dependency2.srcSubpass = 0;
    dependency2.dstSubpass = VK_SUBPASS_EXTERNAL;
    dependency2.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency2.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependency2.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency2.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependency2.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    std::array subPasses = {subpass};
    std::array dependencies = {dependency1, dependency2};

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDesc.size());
    renderPassInfo.pAttachments = attachmentDesc.data();
    renderPassInfo.subpassCount = subPasses.size();
    renderPassInfo.pSubpasses = subPasses.data();
    renderPassInfo.dependencyCount = dependencies.size();
    renderPassInfo.pDependencies = dependencies.data();

    if (vkCreateRenderPass(ctx.getLogicalDevice(),
                           &renderPassInfo, nullptr,
                           &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}
