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
        attachmentDesc[attachmentRef.index] = attachmentRef.attachment.getAttachmentDescription();
        if (attachmentRef.attachment.getAttachmentType() == AttachmentType::MSAA) {
            containsMsaa = true;
        }
    }

    for (auto &attachmentRef: attachments) {
        auto ref = VkAttachmentReference{
            .attachment = attachmentRef.index,
            .layout = attachmentRef.layout
        };
        switch (attachmentRef.attachment.getAttachmentType()) {
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


    /*
    VkAttachmentDescription colorAttachment{};
    // we *may* use swap chain image format but not necessarily.
    // VkRenderPass can be shared between swap chains
    colorAttachment.format = colorAttachmentFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = depthAttachmentFormat;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    */

    // TODO: dynamic subpass creation
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = colorAttachments.data();
    subpass.pDepthStencilAttachment = depthStencilAttachments.data();
    subpass.pResolveAttachments = resolveAttachments.data();

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
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(ctx.getLogicalDevice(),
                           &renderPassInfo, nullptr,
                           &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}
