//
// Created by miska on 2024/10/19.
//

#ifndef QUEUECONTEXT_H
#define QUEUECONTEXT_H
#include "ContextMacros.h"
#include "SubContext.h"
#include "VulkanQueue.h"
#include "CommandPool.h"

struct DeviceContext;

class QueueContext : public SubContext<DeviceContext>{
    uint32_t queueFamilyIndex;
    uint32_t queueIndex;
public:
    CTX_PROPERTY(VulkanQueue, queue)
    CTX_PROPERTY(CommandPool, commandPool)

    QueueContext(DeviceContext& ctx, uint32_t queueFamilyIndex, uint32_t queueIndex);
    ~QueueContext() override;
};



#endif //QUEUECONTEXT_H
