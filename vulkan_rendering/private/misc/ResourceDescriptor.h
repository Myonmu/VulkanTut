//
// Created by miska on 2024/12/02.
//

#pragma once

struct ResourceDescriptor;
template<typename T>
concept ResourceDescriptorType = std::is_base_of_v<ResourceDescriptor, T>;

enum class ResourceArchType {
    UNDEFINED,
    TEXTURE,
    BUFFER,
    PROXY
};


struct ResourceDescriptor {
    virtual ResourceArchType get_archType() {
        return ResourceArchType::UNDEFINED;
    };
    ResourceDescriptor() = default;
    virtual ~ResourceDescriptor() = default;
    ResourceDescriptor(const ResourceDescriptor&) = default;
};
