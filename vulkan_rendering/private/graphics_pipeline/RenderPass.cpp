//
// Created by miska on 2024/02/11.
//

#include "RenderPass.h"

#include "DeviceContext.h"

void Subpass::useAttachment(uint32_t id, VkImageLayout layout, ResourceUsageType usageType) {
    auto &attachment = renderPass.attachments[id];
    switch (usageType) {
        case ResourceUsageType::READ:
            if (attachment->getAttachmentType() == AttachmentType::DEPTH_STENCIL) {
                depthStencilAttachment.emplace(id, layout);
                return;
            }
            inputAttachments.emplace_back(id, layout);
            break;
        case ResourceUsageType::WRITE:
            switch (attachment->getAttachmentType()) {
                case AttachmentType::PRESENT:
                    if (renderPass.msaaEnabled) {
                        resolveAttachments.emplace_back(id, layout);
                    } else {
                        colorAttachments.emplace_back(id, layout);
                    }
                    break;
                case AttachmentType::DEPTH_STENCIL:
                    depthStencilAttachment.emplace(id, layout);
                    break;
                case AttachmentType::COLOR:
                    colorAttachments.emplace_back(id, layout);
                    break;
                default: ;
            }
            break;
        default: ;
    }
}

void Subpass::preserveAttachment(uint32_t id) {
    preserveAttachments.emplace_back(id);
}


VkSubpassDescription Subpass::getSubpassDescription() const {
    return {
        .flags = flags,
        .pipelineBindPoint = bindPoint,
        .inputAttachmentCount = static_cast<uint32_t>(inputAttachments.size()),
        .pInputAttachments = inputAttachments.data(),
        .colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size()),
        .pColorAttachments = colorAttachments.data(),
        .pResolveAttachments = resolveAttachments.data(),
        .pDepthStencilAttachment = depthStencilAttachment.has_value() ? &depthStencilAttachment.value() : nullptr,
        .preserveAttachmentCount = static_cast<uint32_t>(preserveAttachments.size()),
        .pPreserveAttachments = preserveAttachments.data()
    };
}

SubpassDependency &SubpassDependency::setSrc(uint32_t id, VkPipelineStageFlags stageFlags, VkAccessFlags accessFlags) {
    dependency.srcSubpass = id;
    dependency.srcAccessMask = accessFlags;
    dependency.srcStageMask = stageFlags;
    return *this;
}

SubpassDependency &SubpassDependency::setDest(uint32_t id, VkPipelineStageFlags stageFlags, VkAccessFlags accessFlags) {
    dependency.dstSubpass = id;
    dependency.dstAccessMask = accessFlags;
    dependency.dstStageMask = stageFlags;
    return *this;
}

SubpassDependency &SubpassDependency::setFlags(VkDependencyFlags flags) {
    dependency.dependencyFlags = flags;
    return *this;
}


VkSubpassDependency SubpassDependency::getSubpassDependency() const {
    return dependency;
}


RenderPass::~RenderPass() {
    vkDestroyRenderPass(ctx.getLogicalDevice(), resource, nullptr);
}

// TODO: make render pass configurable
RenderPass::RenderPass(DeviceContext &context)
    : VulkanResource(context) {
}

void RenderPass::build() {
    std::vector<VkAttachmentDescription> descriptions{};
    for (auto &attachment: attachments) {
        descriptions.push_back(attachment->getAttachmentDescription());
    }
    std::vector<VkSubpassDescription> subpassDescriptions;
    for (auto &subpass: subpasses) {
        subpassDescriptions.push_back(subpass->getSubpassDescription());
    }
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(descriptions.size());
    renderPassInfo.pAttachments = descriptions.data();
    renderPassInfo.subpassCount = subpassDescriptions.size();
    renderPassInfo.pSubpasses = subpassDescriptions.data();
    renderPassInfo.dependencyCount = subpassDependencies.size();
    renderPassInfo.pDependencies = subpassDependencies.data();

    if (vkCreateRenderPass(ctx.getLogicalDevice(),
                           &renderPassInfo, nullptr,
                           &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}


Subpass &RenderPass::createSubpass() {
    return *subpasses.emplace_back(std::make_unique<Subpass>(*this));
}

uint32_t RenderPass::addAttachment(RenderAttachment *attachment) {
    if (attachment->getAttachmentDescription().samples != VK_SAMPLE_COUNT_1_BIT) {
        msaaEnabled = true;
    }
    attachments.push_back(attachment);
    return attachments.size() - 1;
}

void RenderPass::addDependency(SubpassDependency &dependency) {
    subpassDependencies.push_back(dependency.getSubpassDependency());
}
