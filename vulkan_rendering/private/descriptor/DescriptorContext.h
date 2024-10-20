//
// Created by Tereza on 10/12/2024.
//

#ifndef DESCRIPTORCONTEXT_H
#define DESCRIPTORCONTEXT_H
#include "ContextMacros.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "SubContext.h"

struct DeviceContext;
class LogicalDevice;

struct DescriptorContext: public SubContext<DeviceContext>{
    CTX_PROPERTY(DescriptorSetLayout,descriptorSetLayout)
    CTX_PROPERTY(DescriptorPool, descriptorPool)

    DescriptorContext(const DeviceContext &ctx, const std::vector<DescriptorSetLayoutBinding> &bindings);
};


#endif //DESCRIPTORCONTEXT_H
