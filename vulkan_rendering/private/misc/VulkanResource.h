//
// Created by miska on 2024/02/10.
//

#ifndef VULKANTUT_VULKANRESOURCE_H
#define VULKANTUT_VULKANRESOURCE_H

#include <iostream>

struct VulkanAppContext;

template<typename T>
class VulkanResource {
protected:
    T resource;
    VulkanAppContext& ctx;
public:
    explicit VulkanResource(VulkanAppContext& context) : ctx(context){
#ifdef NDEBUG
#else
        std::cout << "Initializing " << typeid(T).name() << std::endl;
#endif
    }

    virtual ~VulkanResource() {
#ifdef NDEBUG
#else
        std::cout << "Cleaning up " << typeid(T).name() << std::endl;
#endif
    }
    operator T() const { return resource; }
    operator const T *() const { return &resource; }

    // Gets the raw Vulkan object (type T)
    virtual T getRaw() const {
        return resource;
    }

    // Gets the pointer to the raw Vulkan object
    virtual T *getRawPtr() {
        return &resource;
    }
};


#endif //VULKANTUT_VULKANRESOURCE_H
