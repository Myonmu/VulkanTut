//
// Created by miska on 2024/02/08.
//

#ifndef VULKANTUT_QUEUEFAMILYINDICES_H
#define VULKANTUT_QUEUEFAMILYINDICES_H

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
    QueueFamily requiredBits;
    std::unordered_map<QueueFamily, uint32_t> indices;

    [[nodiscard]] bool isComplete() const {
        // For each required bit in `requiredBits`, check if the corresponding index is valid
        for (uint32_t i = 0; i < QUEUE_FAMILY_SUPPORTED_QUERY_COUNT; ++i) {
            // Assuming there are 4 possible bits
            if (auto bit = static_cast<QueueFamily>(1 << i); (requiredBits & bit) == bit) {
                // If the bit is set in `requiredBits`
                // Check if the corresponding queue family index exists and is valid
                if (auto it = indices.find(bit); it == indices.end() || it->second == static_cast<uint32_t>(-1)) {
                    return false;
                }
            }
        }
        return true; // All required bits are satisfied
    }

    std::optional<uint32_t> operator[](const QueueFamily index) {
        auto pos = indices.find(index);
        if (pos != indices.end()) {
            auto val = indices[index];
            return {val};
        }
        return {};
    }

    QueueFamilyIndices(const PhysicalDevice &device, const VulkanSurface &surface,
                       const QueueFamily queueFamilyFlags): requiredBits(queueFamilyFlags) {
        queryQueueFamilies(device, surface);
    }

    explicit QueueFamilyIndices(const QueueFamily queueFamilyFlags):requiredBits(queueFamilyFlags) {

    };

    [[nodiscard]] std::set<uint32_t> getUniqueQueueFamilyIndices() const {
        std::set<uint32_t> uniqueIndices;
        for (const auto &entry: indices) {
            uniqueIndices.insert(entry.second);
        }
        return uniqueIndices;
    }

    [[nodiscard]] size_t getQueueFamilyIndicesCount() const {
        return indices.size();
    }

    void fillQueueFamilyIndicesArray(uint32_t arr[]) const {
        int i = 0;
        for (const auto &entry: indices) {
            arr[i] = entry.second;
            i++;
        }
    }

    void queryQueueFamilies(const VkPhysicalDevice device, const VkSurfaceKHR surface) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto &queueFamily: queueFamilies) {
            checkFlags(queueFamily, QueueFamily::QUEUE_FAMILY_GRAPHICS, VK_QUEUE_GRAPHICS_BIT, i);
            checkFlags(queueFamily, QueueFamily::QUEUE_FAMILY_COMPUTE, VK_QUEUE_COMPUTE_BIT, i);
            checkFlags(queueFamily, QueueFamily::QUEUE_FAMILY_TRANSFER, VK_QUEUE_TRANSFER_BIT, i);

            // Check if present queue is required (requires surface support)
            if (static_cast<bool>(requiredBits & QueueFamily::QUEUE_FAMILY_PRESENT)) {
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
                if (presentSupport) {
                    indices[QueueFamily::QUEUE_FAMILY_PRESENT] = i;
                }
            }
            if (isComplete()) {
                break;
            }
            i++;
        }
    }
private:



    void checkFlags(const VkQueueFamilyProperties &prop, const QueueFamily &compareFlag, const VkQueueFlagBits vkFlag,
                    const uint32_t i) {
        if (static_cast<bool>(requiredBits & compareFlag) && prop.queueFlags & vkFlag) {
            indices[QueueFamily::QUEUE_FAMILY_GRAPHICS] = i;
        }
    }
};


#endif //VULKANTUT_QUEUEFAMILYINDICES_H
