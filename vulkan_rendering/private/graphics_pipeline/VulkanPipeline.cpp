//
// Created by miska on 2024/02/11.
//

#include "VulkanPipeline.h"
#include "Shader.h"
#include "DeviceContext.h"
#include "Vertex.h"
#include "PipelineLayout.h"

VkPipelineRasterizationStateCreateInfo getRasterizerStateCreateInfo() {
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
    return rasterizer;
}

VkPipelineInputAssemblyStateCreateInfo getInputAssemblyCreateInfo() {
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    return inputAssembly;
}

VkViewport getFullWindowViewport() {
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = 0;
    viewport.height = 0;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    return viewport;
}

VkRect2D getFullWindowScissorRect() {
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {0,0};
    return scissor;
}

VkPipelineColorBlendAttachmentState getColorBlendAttachmentState() {
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT
                                          | VK_COLOR_COMPONENT_A_BIT;

    // Temporary remove blending since we might write alpha = 0 in gbuffer2
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    return colorBlendAttachment;
}

VkPipelineMultisampleStateCreateInfo getPipelineMultisampleStateCreateInfo(DeviceContext& ctx) {
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;//ctx.getLogicalDevice().deviceFeatures.sampleRateShading;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = .2f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional
    return multisampling;
}

// TODO: reflect from shader (similar to ShaderLab controls)
VkPipelineDepthStencilStateCreateInfo getDepthStencilStateCreateInfo() {
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;

    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;

    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional

    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional

    return  depthStencil;
}

VulkanPipeline::VulkanPipeline(DeviceContext &context,
                               const std::vector<Shader>& shaders,
                               const PipelineLayout &layout,
                               const RenderPass &renderPass,
                               const uint32_t subpassId)
    : VulkanResource(context), layout(layout) ,renderPass(renderPass), subpassId(subpassId){
    for (auto& shader: shaders) {
        auto& s = shaderModules.emplace_back(std::make_unique<ShaderModule>(shader, context));
        shaderStages.push_back(s->getShaderStageCreateInfo());
    }

    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    inputAssembly = getInputAssemblyCreateInfo();
    viewport = getFullWindowViewport();
    auto scissor = getFullWindowScissorRect();

    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    rasterizer = getRasterizerStateCreateInfo();
    multisampling = getPipelineMultisampleStateCreateInfo(ctx);

    depthStencil = getDepthStencilStateCreateInfo();
}

VulkanPipeline::~VulkanPipeline() {
    vkDestroyPipeline(ctx.getLogicalDevice(),
                      resource, nullptr);
}

void VulkanPipeline::build() {

    auto bindingDesc = Vertex::getBindingDescription();
    auto vertexAttrDesc = Vertex::getAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInput.vertexBindingDescriptionCount = 1;
    vertexInput.pVertexBindingDescriptions = &bindingDesc; // Optional
    vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttrDesc.size());
    vertexInput.pVertexAttributeDescriptions = vertexAttrDesc.data(); // Optional

    auto colorAttachmentCnt = renderPass.getSubpass(subpassId).colorAttachments.size();
    std::vector<VkPipelineColorBlendAttachmentState> colorAttachments{};
    for (uint32_t i = 0; i < colorAttachmentCnt; i++) {
        colorAttachments.push_back(getColorBlendAttachmentState());
    }


    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = colorAttachmentCnt;
    colorBlending.pAttachments = colorAttachments.data();
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional


    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInput;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = layout;

    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = subpassId;

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(ctx.getLogicalDevice()
                                  , VK_NULL_HANDLE, 1,
                                  &pipelineInfo, nullptr,
                                  &resource) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}
