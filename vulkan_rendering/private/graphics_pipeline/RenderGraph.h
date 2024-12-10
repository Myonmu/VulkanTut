//
// Created by Tereza on 11/19/2024.
//

/* Adapted from:
 *  https://github.com/Themaister/Granite/blob/master/renderer/render_graph.hpp
 *
 *  primary article: https://themaister.net/blog/2017/08/15/render-graphs-and-vulkan-a-deep-dive/
 *  supplementary article: https://poniesandlight.co.uk/reflect/island_rendergraph_1/
 */

#pragma once
#include <memory>
#include <string>
#include <TextureImage.h>
#include <unordered_set>
#include <utility>
#include <vulkan/vulkan_core.h>
#include "CompoundDataContainers.h"
#include "RenderAttachment.h"
#include "UtilityMacros.h"

enum RenderGraphQueueFlagBits {
    RENDER_GRAPH_QUEUE_GRAPHICS_BIT = 1 << 0,
    RENDER_GRAPH_QUEUE_COMPUTE_BIT = 1 << 1,
    RENDER_GRAPH_QUEUE_ASYNC_COMPUTE_BIT = 1 << 2
};

using RenderGraphQueueFlags = uint32_t;


enum class ResourceUsageType {
    /**
     * "NONE" is valid when a resource is PRESERVED
     */
    NONE,
    READ,
    WRITE,
    READ_WRITE,
};

enum class ResourceType {
    GENERIC_TEXTURE = 0,
    COLOR_TEXTURE = 1,
    RESOLVE_TEXTURE = 2,
    COLOR_SCALE_TEXTURE = 3,
    STORAGE_TEXTURE = 4,
    BLIT_TEXTURE = 5,
    ATTACHMENT_TEXTURE = 6,
    HISTORY_TEXTURE = 7,
    DEPTH_STENCIL_TEXTURE = 8,

    GENERIC_BUFFER = 9,
    STORAGE_BUFFER = 10,
    TRANSFER_BUFFER = 11,

    PROXY = 12
};

struct RenderResource {
    const uint32_t id;
    const std::string name;
    uint32_t physicalId = -1;
    std::unordered_set<uint32_t> writtenInPasses;
    std::unordered_set<uint32_t> readInPasses;
    RenderGraphQueueFlags queueFlags = 0;

    RenderResource(const uint32_t id, std::string name): id(id), name(std::move(name)) {
    }

    void recordUsage(uint32_t passId, ResourceUsageType usage, RenderGraphQueueFlags queueFlags);

    virtual ~RenderResource() = default;
};

template<ResourceDescriptorType T>
struct RenderResourceT: public RenderResource {
    T info{};
    RenderResourceT(const uint32_t id, std::string name): RenderResource(id, std::move(name)) {
    }
};

template<typename T>
concept RenderResourceType = std::is_base_of_v<RenderResource, T>;

struct RenderTextureResource : public RenderResourceT<AttachmentInfo> {
    VkImageUsageFlags usage = 0;
    bool isTransient = false;
    RenderTextureResource(const uint32_t id, std::string name): RenderResourceT(id, std::move(name)) {
    }
};

struct RenderBufferResource : public RenderResourceT<BufferInfo> {
    RenderBufferResource(const uint32_t id, std::string name): RenderResourceT(id, std::move(name)) {
    }
};


class RenderGraph;

class RenderGraphNode {
    uint32_t id;
    RenderGraph &renderGraph;
    RenderGraphQueueFlagBits queueFlags;
    //Resource Type -> usage type -> resources
    cascade_unordered_map<ResourceType, ResourceUsageType, std::vector<RenderTextureResource *> > textureResources{};
    cascade_unordered_map<ResourceType, ResourceUsageType, std::vector<RenderBufferResource *> > bufferResources{};

public:
    PROPERTY(std::string, name, public, public)

    RenderGraphNode(RenderGraph &graph, uint32_t id, RenderGraphQueueFlagBits queueFlags) : id(id), renderGraph(graph),
        queueFlags(queueFlags) {
    }

    [[nodiscard]] std::vector<RenderTextureResource *> &getTextureResources(
        const ResourceType type, const ResourceUsageType usageType) {
        return textureResources[type][usageType];
    }

    [[nodiscard]] std::vector<RenderBufferResource *> &getBufferResources(
        const ResourceType type, const ResourceUsageType usageType) {
        return bufferResources[type][usageType];
    }

    RenderTextureResource &addTextureResource(const std::string &name,
                                              const AttachmentInfo &decl,
                                              ResourceType resourceType,
                                              ResourceUsageType usageType);

    RenderBufferResource &addBufferResource(const std::string &name,
                                             const BufferInfo &decl,
                                             ResourceType resourceType,
                                             ResourceUsageType usageType);
};


class RenderGraph {
    uint32_t nodesCounter = 0;
    uint32_t resourcesCounter = 0;
    aliased_unordered_map<uint32_t, std::string, std::unique_ptr<RenderResource> > resources{};
    // since strong key is assigned by submission order, nodes are arranged in submission order as well
    aliased_map<uint32_t, std::string, std::unique_ptr<RenderGraphNode> > nodes{};

    void validate();

    static bool surfaceTransformSwapsXy(VkSurfaceTransformFlagBitsKHR transform);

public:

    RenderGraphNode &getOrCreateNode(RenderGraphQueueFlagBits queue, const std::string &name);

    template<RenderResourceType T>
    T &getOrCreateResource(const std::string &name);
};
