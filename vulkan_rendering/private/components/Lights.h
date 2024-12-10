//
// Created by miska on 2024/11/17.
//

#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct MainLightUboData {
    glm::vec4 direction;
    glm::vec4 color;
};

struct MainLight {
    glm::vec3 direction;
    /**
     * rgb = color, a = intensity
     */
    glm::vec4 color;
};