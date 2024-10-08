//
// Created by miska on 2024/08/20.
//

#ifndef UNIFORMBUFFEROBJECT_H
#define UNIFORMBUFFEROBJECT_H
#include "glm/mat4x4.hpp"

// Remember alignment requirements!
// https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#interfaces-alignment-requirements

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};



#endif //UNIFORMBUFFEROBJECT_H
