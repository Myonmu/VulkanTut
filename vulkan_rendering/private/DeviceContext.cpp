//
// Created by miska on 2024/10/14.
//

#include "DeviceContext.h"
#include "LogicalDevice.h"
#include <VulkanAppContext.h>
#include "QueueFamilyIndices.h"

DeviceContext::DeviceContext(VulkanAppContext &ctx, VulkanDeviceSetupProcedure &setupProcedure) : SubContext(ctx) {
    sceneRoot = std::make_unique<ObjectNode>();
    setupProcedure.createWindows(*this);
    init();
    setupProcedure.createRenderPasses(*this);
}

void DeviceContext::createWindow(const char *name, int width, int height, QueueFamily requiredQueueFamilies) {
    // when the logical device is not yet created, adding a new window (surface) is safe
    if (!isDeviceCreated) {
        const auto id = create_windowContext(name, width, height, requiredQueueFamilies);
        get_windowContext_at(id).id = id;
    } else {
        // or when required queue families are already satisfied
        if (auto combinedRequirements = getCombinedQueueFamilyRequirements();
            (combinedRequirements | requiredQueueFamilies) == combinedRequirements) {
            const auto id = create_windowContext( name, width, height, requiredQueueFamilies);
            get_windowContext_at(id).id = id;
        } else {
            // or else, requiring a new queue family would cause a device recreation
            // we do not consider allocating a new physical device for now.
            // TODO: allow allocating new physical device for unsuitable surface
            // TODO: allow device recreation
            throw std::runtime_error(
                "New window requires extra queue families which will cause device recreation, this is not supported.");
        }
    }
}


void DeviceContext::init() {
    physicalDevice = std::make_unique<PhysicalDevice>(*this);
    // TODO: this query is not necessary, because the selected physical device has already done this
    queueFamilyIndices = std::make_unique<QueueFamilyIndices>(get_physicalDevice(),
                                                              getCombinedQueueFamilyRequirements());
    //cannot use getter (const)
    queueFamilyIndices->queryCommonQueueFamilies();
    queryPresentQueues(*queueFamilyIndices, get_physicalDevice(), *queueFamilyIndices);
    logicalDevice = std::make_unique<LogicalDevice>(*this);
    for (auto id: get_queueFamilyIndices().getUniqueQueueFamilyIndices()) {
        create_queueContext(id, 0);
    }

    isDeviceCreated = true;
    // now we finalize window contexts
    for (const auto &window: windowContext) {
        window->init();
    }

    vma = std::make_unique<VmaInstance>(*this);
}

DeviceContext::~DeviceContext() = default;

LogicalDevice &DeviceContext::getLogicalDevice() const {
    return *logicalDevice;
}

QueueFamily DeviceContext::getCombinedQueueFamilyRequirements() const {
    QueueFamily q{};
    for (const auto &window: windowContext) {
        q = q | window->requiredQueueFamilies;
    }
    return q;
}

void DeviceContext::queryPresentQueues(QueueFamilyIndices& queueFamilyIndices, VkPhysicalDevice physicalDevice, QueueFamilyIndices &queueFamilies) const {
    for (auto &window: windowContext) {
        queueFamilyIndices.queryPresentFamilyIndex(physicalDevice, window->get_surface());
    }
}

QueueContext &DeviceContext::getPresentQueueContext(const VulkanSurface &surface) const {
    const auto id = queueFamilyIndices->getPresentQueueFamilyIndex(surface).value();
    return get_queueContext_at(id);
}

QueueContext &DeviceContext::getCommonQueueContext(const QueueFamily queueFamily) const {
    const auto id = queueFamilyIndices->getCommonQueueFamilyIndex(queueFamily).value();
    return get_queueContext_at(id);
}

void DeviceContext::bindRenderPassToWindow(const uint32_t windowId, const uint32_t renderPassId) const {
    get_windowContext_at(windowId).createFrameBuffers(get_renderPass_at(renderPassId));
}

void DeviceContext::destroyWindow(uint32_t id) {
    uint32_t i = 0;
    for (auto const &window: windowContext) {
        if (window->id == id) {
            break;
        }
        i++;
    }
    /* TODO: could leave holes for future allocation (use array ?), because erase shifts elements forward
     * This can potentially be inefficient in games like WindowShot where windows are spawned/destroyed frequently
     */
    windowContext.erase(windowContext.begin() + i);
}

WindowContext& DeviceContext::getWindowContextFromSdlId(const SDL_WindowID& window_id) {
    for (auto &window: windowContext) {
        if(window->window->get_SdlWindowId() == window_id) return *window;
    }
    throw std::runtime_error("Window context not found");
}


CTX_FORWARD_GET_BODY(DeviceContext, VulkanInstance, vulkanInstance)
