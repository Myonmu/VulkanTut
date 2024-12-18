//
// Created by miska on 2024/10/14.
//
#pragma once

#include <LogicalDevice.h>
#include <PhysicalDevice.h>
#include <vector>
#include <VmaInstance.h>
#include <VulkanInstance.h>
#include "QueueContext.h"
#include "SubContext.h"
#include "VulkanSetupProcedure.h"
#include "WindowContext.h"
#include "ContextMacros.h"
#include "ObjectHierarchy.h"

struct VulkanAppContext;

struct DeviceContext : SubContext<VulkanAppContext> {
    const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    /*----------- PROPERTIES ----------*/

    // VkPhysicalDevice, requires surfaces to select device
    CTX_PROPERTY(PhysicalDevice, physicalDevice)
    // Queue family indices info of the selected physical device
    CTX_PROPERTY(QueueFamilyIndices, queueFamilyIndices)
    // VkDevice
    CTX_PROPERTY(LogicalDevice, logicalDevice)
    // VMA Allocator
    CTX_PROPERTY(VmaInstance, vma);

    CTX_PROPERTY_LIST(QueueContext, queueContext)

    CTX_PROPERTY_LIST(WindowContext, windowContext)

    CTX_PROPERTY_LIST(RenderPass, renderPass)
    //Queues (should always be ordered)

    CTX_PROPERTY(ObjectNode, sceneRoot)

    /*----------- FORWARD CALLS ----------*/

    CTX_FORWARD_GET_DECL(VulkanInstance, vulkanInstance)

    /*----------- MEMBER FUNCTIONS ----------*/
    explicit DeviceContext(VulkanAppContext &ctx, VulkanDeviceSetupProcedure &setupProcedure);

    ~DeviceContext() override;

    [[nodiscard]] LogicalDevice &getLogicalDevice() const override;

    // get combined queue family requirement flags from all windows
    [[nodiscard]] QueueFamily getCombinedQueueFamilyRequirements() const;

    void createWindow(const char *name, int width, int height, QueueFamily requiredQueueFamilies);
    void destroyWindow(uint32_t id);

    // manually called after all surfaces are created
    void init();

    [[nodiscard]] bool isLogicalDeviceCreated() const { return isDeviceCreated; }

    [[nodiscard]] QueueContext &getCommonQueueContext(QueueFamily queueFamily) const;

    [[nodiscard]] QueueContext &getPresentQueueContext(const VulkanSurface &surface) const;

    void bindRenderPassToWindow(uint32_t windowId, uint32_t renderPassId) const;

    template<ObjectNodeType T, typename... Args>
    T &createObject(Args &&... args) {
        return get_sceneRoot().createChildObject<T>(std::forward<Args>(args)...);
    }

    WindowContext& getWindowContextFromSdlId(const SDL_WindowID& window_id);

private:
    // fills a QueueFamilyIndices object with present queue info.
    void queryPresentQueues(QueueFamilyIndices &queueFamilyIndices, VkPhysicalDevice physicalDevice,
                            QueueFamilyIndices &queueFamilies) const;

    friend class PhysicalDevice;
    bool isDeviceCreated = false;
};
