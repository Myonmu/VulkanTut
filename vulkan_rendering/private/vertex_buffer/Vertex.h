//
// Created by miska on 2024/08/06.
//

#pragma once

#include <vector>
#include <array>
#include <vulkan/vulkan_core.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include "glm/glm.hpp"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        /* NOTE:
         *
         * Unlike HLSL that has POSITION, TEXCOORD0 semantics,
         * GLSL only has locations e.g. layout( location = 0 )
         * This suggests if the location mismatches c++ declarations,
         * e.g. swizzling position and color,
         * the SPIRV-Cross reflection will *not* know such swizzle happened,
         * and then we cannot counter it by swizzle the vertex input data.
         *
         *  Hence, the vertex input attribute description is hard-coded here.
         *
         *  Optionally, for further implementation:
         *  use embedded yaml to determine the intended use of the bindings.
         */
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        attributeDescriptions[1].location = 1;

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, normal);
        attributeDescriptions[2].location = 2;

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    static const std::vector<Vertex> testVerts;

    static const std::vector<uint32_t> testIndices;

    bool operator==(const Vertex &other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};


template <>
struct std::hash<Vertex> {
    size_t operator()(const Vertex& v) const noexcept {
        size_t h1 = hash<glm::vec3>()(v.pos);     // Hash position
        size_t h2 = hash<glm::vec3>()(v.color);   // Hash color
        size_t h3 = hash<glm::vec3>()(v.normal);  // Hash normal
        size_t h4 = hash<glm::vec2>()(v.texCoord); // Hash texture coordinate
        // Combine all hashes using XOR and bit shifting
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};
