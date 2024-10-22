//
// Created by Tereza on 10/22/2024.
//

#include "Shader.h"
#include "spirv_cross.hpp"

Shader::Shader(std::vector<uint32_t> code) {
    const spirv_cross::Compiler compiler(std::move(code));
    resources = compiler.get_shader_resources();
    
}

Shader::~Shader() {

}
