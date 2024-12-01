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

    VkImageUsageFlags convertToVulkanImageUsageFlags(ResourceType type, ResourceUsageDeclType usage,
                                                     const AttachmentDecl &decl) {
        return 0; //TODO
#define BRANCH_BY_USAGE(inputFlag, outputFlag)\
    switch(usage) { case ResourceUsageDeclType::INPUT: return inputFlag; case ResourceUsageDeclType::OUTPUT: return outputFlag;}
        /*
                switch (type) {
                    case ResourceType::GENERIC_TEXTURE: BRANCH_BY_USAGE(VK_IMAGE_USAGE_SAMPLED_BIT, 0);
                    case ResourceType::COLOR_TEXTURE: {
                        switch (usage) {
                            case ResourceUsageDeclType::INPUT: return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
                            case ResourceUsageDeclType::OUTPUT: {
                                if (decl.levels != 1)
                                    return VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
                                return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
                            }
                        };
                    case ResourceType::RESOLVE_TEXTURE: BRANCH_BY_USAGE(0, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
                    case ResourceType::COLOR_SCALE_TEXTURE: BRANCH_BY_USAGE();
                    case ResourceType::STORAGE_TEXTURE: return VK_IMAGE_USAGE_STORAGE_BIT;
                    case ResourceType::BLIT_TEXTURE: BRANCH_BY_USAGE();
                    case ResourceType::ATTACHMENT_TEXTURE: BRANCH_BY_USAGE(VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,);
                    case ResourceType::HISTORY_TEXTURE: BRANCH_BY_USAGE(VK_IMAGE_USAGE_SAMPLED_BIT);
                    case ResourceType::DEPTH_STENCIL_TEXTURE: break;
                    case ResourceType::PROXY: break;
                    default: return 0;
                    }*/
    }
}

RenderTextureResource &RenderGraphNode::addTextureResource(const std::string &name, const AttachmentDecl &decl,
                                                           ResourceType resourceType, ResourceUsageDeclType usageType) {
    auto &res = renderGraph.getOrCreateResource<RenderTextureResource>(name);
    res.decl = decl;
    res.writtenInPasses.insert(id);
    res.queueFlags |= queueFlags;
    throw std::runtime_error("Not implemented"); //TODO
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


void RenderGraph::validate() {
    nodes.forEachValue([this](const uint32_t &id, const std::unique_ptr<RenderGraphNode> &nodePtr) {
            auto &node = *nodePtr;

#define GET_TEX(type, usage) node.getTextureResources(ResourceType::type, ResourceUsageDeclType::usage)
#define TEX_IN_OUT_MISMATCH(type) GET_TEX(type, INPUT).size() != GET_TEX(type, OUTPUT).size()
#define GET_BUFFER(type, usage) node.getBufferResources(ResourceType::type, ResourceUsageDeclType::usage)
#define BUFFER_IN_OUT_MISMATCH(type) GET_BUFFER(type, INPUT).size() != GET_BUFFER(type, OUTPUT).size()

            if (TEX_IN_OUT_MISMATCH(COLOR_TEXTURE))
                throw std::logic_error("Size of color inputs must match color outputs.");

            if (BUFFER_IN_OUT_MISMATCH(STORAGE_BUFFER))
                throw std::logic_error("Size of storage inputs must match storage outputs.");

            if (TEX_IN_OUT_MISMATCH(BLIT_TEXTURE))
                throw std::logic_error("Size of blit inputs must match blit outputs.");

            if (TEX_IN_OUT_MISMATCH(STORAGE_TEXTURE))
                throw std::logic_error("Size of storage texture inputs must match storage texture outputs.");

            if (!GET_TEX(RESOLVE_TEXTURE, OUTPUT).empty() && GET_TEX(RESOLVE_TEXTURE, OUTPUT).size() != GET_TEX(
                    COLOR_TEXTURE, OUTPUT).size())
                throw std::logic_error("Must have one resolve output for each color output.");

            unsigned num_inputs = GET_TEX(COLOR_TEXTURE, INPUT).size();
            for (unsigned i = 0; i < num_inputs; i++) {
                auto &texInput = GET_TEX(COLOR_TEXTURE, INPUT)[i];
                if (!texInput) continue;
                auto &texOutput = GET_TEX(COLOR_TEXTURE, OUTPUT)[i];
                if (getResourceDimensions(*texInput) != getResourceDimensions(*texOutput))
                    return;
                    //node.make_color_input_scaled(i); //TODO
            }

            if (auto &output = GET_BUFFER(STORAGE_BUFFER, OUTPUT); !output.empty()) {
                auto &input = GET_BUFFER(STORAGE_BUFFER, INPUT);
                for (unsigned i = 0; i < output.size(); i++) {
                    if (!input[i]) continue;
                    if (output[i]->decl != input[i]->decl)
                        throw std::logic_error("Doing RMW on a storage buffer, but usage and sizes do not match.");
                }
            }

            if (auto &output = GET_TEX(BLIT_TEXTURE, OUTPUT); !output.empty()) {
                auto &input = GET_TEX(BLIT_TEXTURE, INPUT);
                for (unsigned i = 0; i < output.size(); i++) {
                    if (!input[i]) continue;
                    if (getResourceDimensions(*input[i]) != getResourceDimensions(*output[i]))
                        throw std::logic_error("Doing RMW on a blit image, but usage and sizes do not match.");
                }
            }

            if (auto &output = GET_TEX(STORAGE_TEXTURE, OUTPUT); !output.empty()) {
                auto &input = GET_TEX(STORAGE_TEXTURE, INPUT);
                for (unsigned i = 0; i < output.size(); i++) {
                    if (!input[i]) continue;
                    if (getResourceDimensions(*input[i]) != getResourceDimensions(*output[i]))
                        throw std::logic_error("Doing RMW on a storage image, but usage and sizes do not match.");
                }
            }

            if (auto &input = GET_TEX(DEPTH_STENCIL_TEXTURE, INPUT); !input.empty())
                if (auto &output = GET_TEX(DEPTH_STENCIL_TEXTURE, OUTPUT); !output.empty()) {
                    if (getResourceDimensions(*input[0]) != getResourceDimensions(*output[0]))
                        throw std::logic_error("Dimension mismatch.");
                }
        }
    );
}

ResourceDimensions RenderGraph::getResourceDimensions(const RenderBufferResource &resource) const {
    ResourceDimensions dim{};
    auto &info = resource.decl;
    dim.buffer_info = info;
    dim.buffer_info.usage |= resource.decl.usage;
    dim.flags |= info.flags;
    dim.name = resource.name;
    return dim;
}

bool RenderGraph::surfaceTransformSwapsXy(VkSurfaceTransformFlagBitsKHR transform)
{
    return (transform & (
            VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR |
            VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR |
            VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR |
            VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR)) != 0;
}

TexturePxDimensions RenderGraph::resolveTexturePxDimensions(const TextureRelativeDimensions &decl) {
    TexturePxDimensions dim{};
    switch (decl.sizeMode) {
#define RELATIVE_SIZE_TO_ABSOLUTE(relativeTo)\
        dim.width = std::max(static_cast<unsigned>(std::ceil(decl.width * relativeTo.width)), 1u);\
        dim.height = std::max(static_cast<unsigned>(std::ceil(decl.height * relativeTo.height)), 1u);\
        dim.depth = std::max(static_cast<unsigned>(std::ceil(decl.depth * relativeTo.depth)), 1u);

        case AttachmentSizeMode::SWAPCHAIN_RELATIVE:
            RELATIVE_SIZE_TO_ABSOLUTE(swapchainDimensions.dimensions)
            if (surfaceTransformSwapsXy(swapchainDimensions.transform))
                std::swap(dim.width, dim.height);
            break;

        case AttachmentSizeMode::ABSOLUTE:
            dim.width = std::max(static_cast<unsigned>(decl.width), 1u);
            dim.height = std::max(static_cast<unsigned>(decl.height), 1u);
            dim.depth = std::max(static_cast<unsigned>(decl.depth), 1u);
            break;

        case AttachmentSizeMode::INPUT_RELATIVE: {
            if (resources.containsWeakKey(decl.sizeReferenceTextureName))
                throw std::logic_error("Resource does not exist.");
            auto &input = dynamic_cast<RenderTextureResource &>(*resources[decl.sizeReferenceTextureName]);
            auto inputPxDim = resolveTexturePxDimensions(input.decl.dimensions);
            RELATIVE_SIZE_TO_ABSOLUTE(inputPxDim)
            break;
        }
    }
    return dim;
}


ResourceDimensions RenderGraph::getResourceDimensions(const RenderTextureResource &resource) {
    ResourceDimensions dim{};
    auto &decl = resource.decl;
    dim.layers = decl.layers;
    dim.samples = decl.samples;
    dim.format = decl.format;
    dim.queues = resource.queueFlags;
    dim.image_usage = decl.usage | resource.usage;
    dim.name = resource.name;
    dim.flags = decl.flags & ~(ATTACHMENT_INFO_SUPPORTS_PREROTATE_BIT | ATTACHMENT_INFO_INTERNAL_TRANSIENT_BIT);
    dim.dimensions = resolveTexturePxDimensions(resource.decl.dimensions);
    if (resource.isTransient)
        dim.flags |= ATTACHMENT_INFO_INTERNAL_TRANSIENT_BIT;

    // Mark the resource as potentially supporting pre-rotate.
    // If this resource ends up aliasing with the swapchain, it might go through.
    if ((decl.flags & ATTACHMENT_INFO_SUPPORTS_PREROTATE_BIT) != 0)
        dim.transform = swapchainDimensions.transform;

    if (dim.format == VK_FORMAT_UNDEFINED)
        dim.format = swapchainDimensions.format;

    dim.levels = std::min(dim.dimensions.getLevels(), decl.levels == 0 ? ~0u : decl.levels);
    return dim;
}
