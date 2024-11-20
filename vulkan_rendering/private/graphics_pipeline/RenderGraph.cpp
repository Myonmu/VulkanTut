//
// Created by Tereza on 11/19/2024.
//

#include "RenderGraph.h"
#include <cassert>

namespace ResourceTypeUtils {
    bool isOfArchType(ResourceType type, ResourceArchType archType) {
        switch (archType) {
            case ResourceArchType::TEXTURE: return type <= ResourceType::DEPTH_STENCIL_TEXTURE;
            case ResourceArchType::BUFFER: return
                        type >= ResourceType::GENERIC_BUFFER && type <= ResourceType::TRANSFER_BUFFER;
            default: return type == ResourceType::PROXY;
        }
    }

    VkImageUsageFlags convertToVulkanUsageFlags(ResourceType type) {
        switch(type) {
            case ResourceType::GENERIC_TEXTURE: break;
            case ResourceType::COLOR_TEXTURE: return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            case ResourceType::RESOLVE_TEXTURE: return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            case ResourceType::COLOR_SCALE_TEXTURE:break;
            case ResourceType::STORAGE_TEXTURE:break;
            case ResourceType::BLIT_TEXTURE:break;
            case ResourceType::ATTACHMENT_TEXTURE:break;
            case ResourceType::HISTORY_TEXTURE:break;
            case ResourceType::DEPTH_STENCIL_TEXTURE:break;
            case ResourceType::GENERIC_BUFFER:break;
            case ResourceType::STORAGE_BUFFER:break;
            case ResourceType::TRANSFER_BUFFER:break;
            case ResourceType::PROXY:break;
        }

    }

}

RenderTextureResource &RenderGraphNode::addTextureResource(const std::string &name, const AttachmentDecl &decl,
                                                           ResourceType resourceType, ResourceUsageType usageType) {
    auto &res = renderGraph.getOrCreateResource<RenderTextureResource>(name);
    res.decl = decl;
    res.writtenInPasses.insert(id);
    res.queueFlags |= queueFlags;
    res.usage |=
}


RenderGraphNode &RenderGraph::getOrCreateNode(RenderGraphQueueFlagBits queue, const std::string &name) {
    assert(name.empty(), "RenderGraph::getOrCreateNode(): empty name");
    if (nodes.containsWeakKey(name))return *nodes[name];
    const auto &result =
            nodes.emplace(nodesCounter, name, std::make_unique<RenderGraphNode>(*this, nodesCounter, queue));
    nodesCounter++;
    return *result;
}

template<RenderResourceType T>
T &RenderGraph::getOrCreateResource(const std::string &name) {
    assert(name.empty(), "RenderGraph::getOrCreateResource(): empty name");
    if (resources.containsWeakKey(name)) return UNIQUE_PTR_GET_REF(resources[name], T);
    auto result = std::make_unique<T>(resourcesCounter, name);
    auto rawPtr = result.get();
    resources.emplace(resourcesCounter, name, std::move(result));
    resourcesCounter++;
    return *rawPtr;
}
