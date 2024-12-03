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
}

void RenderResource::recordUsage(uint32_t passId, ResourceUsageType usage) {
    switch (usage) {
        case ResourceUsageType::NONE:
            return;
        case ResourceUsageType::READ:
            readInPasses.insert(passId);
            break;
        case ResourceUsageType::WRITE:
            writtenInPasses.insert(passId);
            break;
        case ResourceUsageType::READ_WRITE:
            readInPasses.insert(passId);
            writtenInPasses.insert(passId);
            break;
        default:
            throw std::invalid_argument("Unknown Render Resource usage");
    }
}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Render Resource

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv Render Graph Node

RenderTextureResource &RenderGraphNode::addTextureResource(const std::string &name, const AttachmentInfo &decl,
                                                           ResourceType resourceType, ResourceUsageType usageType) {
    auto &res = renderGraph.getOrCreateResource<RenderTextureResource>(name);
    res.info = decl;
    res.recordUsage(id, usageType);
    res.queueFlags |= queueFlags;
    textureResources[resourceType][usageType].push_back(&res);
    return res;
}

RenderBufferResource &RenderGraphNode::addBufferResource(const std::string &name, const BufferInfo &decl,
                                                          ResourceType resourceType, ResourceUsageType usageType) {
    auto &res = renderGraph.getOrCreateResource<RenderBufferResource>(name);
    res.info = decl;
    res.recordUsage(id, usageType);
    bufferResources[resourceType][usageType].push_back(&res);
    return res;
}


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Render Graph Node

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv Render Graph

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


void RenderGraph::validate() {
    throw std::runtime_error("RenderGraph::validate(): not implemented");
}

bool RenderGraph::surfaceTransformSwapsXy(VkSurfaceTransformFlagBitsKHR transform) {
    return (transform & (
                VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR |
                VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR |
                VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR |
                VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR)) != 0;
}
