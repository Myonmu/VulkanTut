//
// Created by miska on 2024/02/08.
//

#pragma once


#include <optional>
#include <PhysicalDevice.h>
#include <set>
#include <unordered_map>
#include <vector>
#include <VulkanSurface.h>
#include <vulkan/vulkan_core.h>

enum class QueueFamily : uint32_t {
    QUEUE_FAMILY_PRESENT = 1 << 0,
    QUEUE_FAMILY_GRAPHICS = 1 << 1,
    QUEUE_FAMILY_COMPUTE = 1 << 2,
    QUEUE_FAMILY_TRANSFER = 1 << 3,
};

#ifndef QUEUE_FAMILY_SUPPORTED_QUERY_COUNT
#define QUEUE_FAMILY_SUPPORTED_QUERY_COUNT 4
#endif

inline QueueFamily operator|(QueueFamily a, QueueFamily b) {
    return static_cast<QueueFamily>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline QueueFamily operator&(QueueFamily a, QueueFamily b) {
    return static_cast<QueueFamily>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

struct QueueFamilyIndices {
private:
    bool presentQueryFailed = false;

public:
    QueueFamily requiredBits;
    std::vector<VkQueueFamilyProperties> queueFamilies;
    std::unordered_map<QueueFamily, uint32_t> indices;
    std::unordered_map<const VulkanSurface *, uint32_t> presentIndices;

    [[nodiscard]] bool isComplete() const {
        // For each required bit in `requiredBits`, check if the corresponding index is valid
        for (uint32_t i = 0; i < QUEUE_FAMILY_SUPPORTED_QUERY_COUNT; ++i) {
            // Assuming there are 4 possible bits
            if (auto bit = static_cast<QueueFamily>(1 << i); (requiredBits & bit) == bit) {
                // If the bit is set in `requiredBits`
                // Check if the corresponding queue family index exists and is valid
                if (bit == QueueFamily::QUEUE_FAMILY_PRESENT && presentQueryFailed) return false;
                if (auto it = indices.find(bit); it == indices.end() || it->second == static_cast<uint32_t>(-1)) {
                    return false;
                }
            }
        }
        return true; // All required bits are satisfied
    }

    std::optional<uint32_t> getCommonQueueFamilyIndex(const QueueFamily queueFamily) {
        if (queueFamily == QueueFamily::QUEUE_FAMILY_PRESENT)
            throw std::invalid_argument("PRESENT queue is surface-dependant, use getPresentQueueFamilyIndex instead.");
        auto pos = indices.find(queueFamily);
        if (pos != indices.end()) {
            auto val = indices[queueFamily];
            return {val};
        }
        return {};
    }

    QueueFamilyIndices(const PhysicalDevice &device,
                       const QueueFamily queueFamilyFlags)
        : QueueFamilyIndices(static_cast<VkPhysicalDevice>(device), queueFamilyFlags) {
    }

    QueueFamilyIndices(VkPhysicalDevice device, const QueueFamily queueFamilyFlags): requiredBits(queueFamilyFlags) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    }

    [[nodiscard]] std::set<uint32_t> getUniqueQueueFamilyIndices() const {
        std::set<uint32_t> uniqueIndices;
        for (const auto &entry: indices) {
            uniqueIndices.insert(entry.second);
        }
        for (const auto &entry: presentIndices) {
            uniqueIndices.insert(entry.second);
        }
        return uniqueIndices;
    }

    [[nodiscard]] size_t getQueueFamilyIndicesCount(const VulkanSurface &surface) const {
        return indices.size() + (presentIndices.contains(&surface) ? 1 : 0);
    }

    void fillQueueFamilyIndicesArray(uint32_t arr[], const VulkanSurface &surface) const {
        int i = 0;
        for (const auto &entry: indices) {
            arr[i] = entry.second;
            i++;
        }
        if (const auto present = presentIndices.find(&surface); present != presentIndices.end()) {
            arr[i] = present->second;
        }
    }

    [[nodiscard]] std::optional<uint32_t> getPresentQueueFamilyIndex(const VulkanSurface &surface) const {
        return presentIndices.find(&surface)->second;
    }

    void queryCommonQueueFamilies() {
        int i = 0;
        for (const auto &queueFamily: queueFamilies) {
            checkFlags(queueFamily, QueueFamily::QUEUE_FAMILY_GRAPHICS, VK_QUEUE_GRAPHICS_BIT, i);
            checkFlags(queueFamily, QueueFamily::QUEUE_FAMILY_COMPUTE, VK_QUEUE_COMPUTE_BIT, i);
            checkFlags(queueFamily, QueueFamily::QUEUE_FAMILY_TRANSFER, VK_QUEUE_TRANSFER_BIT, i);
            if (isComplete()) {
                break;
            }
            i++;
        }
    }

    // query present queue support and cache result
    void queryPresentFamilyIndex(VkPhysicalDevice device, const VulkanSurface &surface) {
        for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport) {
                presentIndices[&surface] = i;
                return;
            }
        }
        presentQueryFailed = true;
    }

private:
    void checkFlags(const VkQueueFamilyProperties &prop, const QueueFamily &compareFlag, const VkQueueFlagBits vkFlag,
                    const uint32_t i) {
        if (static_cast<bool>(requiredBits & compareFlag) && prop.queueFlags & vkFlag) {
            indices[QueueFamily::QUEUE_FAMILY_GRAPHICS] = i;
        }
    }
};



