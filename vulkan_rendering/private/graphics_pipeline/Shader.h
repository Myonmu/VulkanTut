//
// Created by Tereza on 10/22/2024.
//

#pragma once
#include <spirv_cross.hpp>
#include <vector>

class Shader {
    spirv_cross::ShaderResources resources;
public:
    explicit Shader(std::vector<uint32_t> code);
    ~Shader();
};
