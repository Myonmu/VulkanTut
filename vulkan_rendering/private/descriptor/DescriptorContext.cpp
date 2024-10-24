//
// Created by Tereza on 10/12/2024.
//

#include "DescriptorContext.h"

#include <VulkanAppContext.h>

#include "DescriptorSetLayoutBinding.h"

DescriptorContext::DescriptorContext(DeviceContext &ctx, const std::vector<DescriptorSetLayoutBinding> &bindings)
    : SubContext(ctx) {
    descriptorSetLayout = std::make_unique<DescriptorSetLayout>(*this, bindings);
    descriptorPool = std::make_unique<DescriptorPool>(*this);
}

DescriptorContext::DescriptorContext(DeviceContext &ctx, const std::map<uint32_t, DescriptorSetLayoutBinding> &bindings)
    : SubContext(ctx) {
    descriptorSetLayout = std::make_unique<DescriptorSetLayout>(*this, bindings);
    descriptorPool = std::make_unique<DescriptorPool>(*this);
}
