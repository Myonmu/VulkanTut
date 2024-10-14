//
// Created by Tereza on 10/12/2024.
//

#ifndef DESCRIPTORCONTEXT_H
#define DESCRIPTORCONTEXT_H
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "SubContext.h"

class LogicalDevice;
struct VulkanAppContext;

struct DescriptorContext: public SubContext<VulkanAppContext>{
    DescriptorSetLayout descriptorSetLayout;
    DescriptorPool descriptorPool;

    DescriptorContext(VulkanAppContext &ctx, const std::vector<DescriptorSetLayoutBinding> &bindings);
};


#endif //DESCRIPTORCONTEXT_H
