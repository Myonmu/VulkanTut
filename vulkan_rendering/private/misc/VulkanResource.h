//
// Created by miska on 2024/02/10.
//
#pragma once

#include <iostream>

template<typename T, typename CTX>
class VulkanResource {
protected:
    T resource;
    CTX& ctx;
public:
    explicit VulkanResource(CTX& context):ctx(context){
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
    virtual const T *getRawPtr() const{
        return &resource;
    }
};

