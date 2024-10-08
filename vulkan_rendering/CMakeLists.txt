cmake_minimum_required(VERSION 3.27)
project(Vk)
set(CMAKE_CXX_STANDARD 17)

# Add subdirectories' include paths
set(INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/private/graphics_pipeline
        ${CMAKE_CURRENT_SOURCE_DIR}/private/misc
        ${CMAKE_CURRENT_SOURCE_DIR}/private/presentation
        ${CMAKE_CURRENT_SOURCE_DIR}/private/setup
        ${CMAKE_CURRENT_SOURCE_DIR}/private/drawing
        ${CMAKE_CURRENT_SOURCE_DIR}/private/vertex_buffer
        ${CMAKE_CURRENT_SOURCE_DIR}/private/uniform_buffer
        ${CMAKE_CURRENT_SOURCE_DIR}/private/texture_mapping
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        ${CMAKE_CURRENT_SOURCE_DIR}/public/command_buffer_cmds
)

# Source files
set(SOURCES
        private/setup/GlfwWindow.cpp
        private/graphics_pipeline/GraphicsPipeline.cpp
        private/graphics_pipeline/RenderPass.cpp
        private/graphics_pipeline/ShaderModule.cpp
        private/graphics_pipeline/VulkanPipeline.cpp
        private/misc/FileUtility.cpp
        private/presentation/SwapChain.cpp
        private/setup/LogicalDevice.cpp
        private/setup/PhysicalDevice.cpp
        private/setup/ValidationLayers.cpp
        private/setup/VulkanInstance.cpp
        private/presentation/VulkanSurface.cpp
        private/drawing/FrameBuffers.cpp
        private/drawing/CommandPool.cpp
        private/drawing/CommandBuffer.cpp
        private/drawing/CommandBufferRecorder.cpp
        private/drawing/VulkanFrame.cpp
        private/VulkanRenderer.cpp
        public/CommandBufferCmd.h
        private/VulkanAppContext.cpp
        private/vertex_buffer/Vertex.h
        private/vertex_buffer/VertexBuffer.cpp
        private/vertex_buffer/Vertex.cpp
        private/vertex_buffer/Buffer.cpp
        private/misc/vk_mem_alloc.h
        private/vertex_buffer/IndexBuffer.cpp
        private/misc/VmaInstance.cpp
        private/uniform_buffer/UniformBufferObject.cpp
        private/uniform_buffer/DescriptorSetLayout.cpp
        private/uniform_buffer/UniformBufferGroup.h
        private/uniform_buffer/UniformBufferGroup.cpp
        private/uniform_buffer/DescriptorPool.cpp
        private/uniform_buffer/DescriptorSets.cpp
        private/Texture2D.cpp
        private/texture_mapping/TextureImage.cpp
        public/command_buffer_cmds/BindDescriptorSet.h
        public/FrameInfo.h
        public/command_buffer_cmds/BindIndexBuffer.h
        public/command_buffer_cmds/BindVertexBuffer.h
        public/command_buffer_cmds/SetScissors.h
        public/command_buffer_cmds/SetViewport.h
        public/command_buffer_cmds/DrawIndexed.h
        public/command_buffer_cmds/BindPipeline.h
        private/drawing/RenderPassRecorder.cpp
        private/drawing/RenderPassRecorder.h
        public/command_buffer_cmds/EnqueueRenderPass.h
        public/command_buffer_cmds/CopyBuffer.h
        private/FrameInfo.cpp
        public/command_buffer_cmds/TransitionImageLayout.h
        public/command_buffer_cmds/CopyBufferToImage.h
        private/texture_mapping/ImageView.cpp
        private/texture_mapping/ImageView.h
        private/texture_mapping/TextureSampler.cpp
        private/texture_mapping/TextureSampler.h
        private/texture_mapping/TextureAddressMode.cpp
        private/texture_mapping/TextureAddressMode.h
        private/texture_mapping/TextureFilterMode.cpp
        private/texture_mapping/TextureFilterMode.h
        private/texture_mapping/TextureMipmapInfo.cpp
        private/texture_mapping/TextureMipmapInfo.h
        private/texture_mapping/TextureCompareInfo.cpp
        private/texture_mapping/TextureCompareInfo.h
        private/texture_mapping/TextureAnisotropyInfo.cpp
        private/texture_mapping/TextureAnisotropyInfo.h
)

# Create static library
add_library(Vk STATIC ${SOURCES})

# Vulkan package
find_package(Vulkan REQUIRED)

# GLFW setup (disable unnecessary parts)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Include GLFW and GLM
include_directories(${CMAKE_SOURCE_DIR}/libs/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glm/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/stb)

# Link GLFW, Vulkan, and GLM with the library
target_link_libraries(Vk glfw Vulkan::Vulkan glm)

# Add include directories to target
target_include_directories(Vk
        PUBLIC
        ${INCLUDE_DIRS}
)
