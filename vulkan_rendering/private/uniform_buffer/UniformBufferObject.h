//
// Created by miska on 2024/08/20.
//

#pragma once

#include "glm/mat4x4.hpp"

// Remember alignment requirements!
// https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#interfaces-alignment-requirements

struct UniformBufferObject {
    glm::mat4 model;
};


