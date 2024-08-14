//
// Created by miska on 2024/02/10.
//

#ifndef VULKANTUT_VULKANRESOURCE_H
#define VULKANTUT_VULKANRESOURCE_H

#include <iostream>

struct VulkanAppContext;
template <typename T> class VulkanResource {
protected:
    T resource;
public:
    VulkanResource(){
        std::cout << "Initializing " << typeid(this).name() << std::endl;
    }
    virtual ~VulkanResource() = default;
    operator T() const {return resource;}
    operator T*() const {return &resource;}
    virtual T getRaw() const{
        return resource;
    }
    virtual T* getRawPtr() {
        return &resource;
    }
};


#endif //VULKANTUT_VULKANRESOURCE_H
