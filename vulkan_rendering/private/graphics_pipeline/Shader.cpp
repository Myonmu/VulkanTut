//
// Created by Tereza on 10/22/2024.
//

#include "Shader.h"
#include "spirv_cross.hpp"

Shader::Shader(std::vector<uint32_t> code) {
    const spirv_cross::Compiler compiler(std::move(code));
    resources = compiler.get_shader_resources();
    for(auto &u: resources.uniform_buffers) {
        uint32_t set = compiler.get_decoration(u.id, spv::DecorationDescriptorSet);
        uint32_t binding = compiler.get_decoration(u.id, spv::DecorationBinding);
        printf("Found UBO %s at set = %u, binding = %u!\n",u.name.c_str(), set, binding);
    }

    for(auto &combinedImageSamplers : resources.sampled_images) {

    }

    for(auto &pcb : resources.push_constant_buffers) {

    }

    for(auto &sin : resources.stage_inputs) {
        uint32_t location = compiler.get_decoration(sin.id, spv::DecorationLocation);
        compiler.get_
    }

    for(auto &sout : resources.stage_outputs) {
        uint32_t location = compiler.get_decoration(sout.id, spv::DecorationLocation);
    }
}

Shader::~Shader() {

}
