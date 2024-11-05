cmake_minimum_required(VERSION 3.27)
project(Vk)
set(CMAKE_CXX_STANDARD 23)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/private/descriptor
        ${CMAKE_CURRENT_SOURCE_DIR}/public
        ${CMAKE_CURRENT_SOURCE_DIR}/public/command_buffer_cmds
)

# Source files
set(SOURCES
        private/setup/GlfwWindow.cpp
        private/graphics_pipeline/PipelineLayout.cpp
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
        private/vertex_buffer/Vertex.cpp
        private/vertex_buffer/Buffer.cpp
        private/misc/vk_mem_alloc.h
        private/misc/VmaInstance.cpp
        private/descriptor/DescriptorSetLayout.cpp
        private/uniform_buffer/UniformBufferGroup.h
        private/uniform_buffer/UniformBufferGroup.cpp
        private/descriptor/DescriptorPool.cpp
        private/descriptor/DescriptorSets.cpp
        private/Texture2D.cpp
        private/texture_mapping/TextureImage.cpp
        public/command_buffer_cmds/CBC_Drawing.h
        public/FrameInfo.h
        private/drawing/RenderPassRecorder.cpp
        private/drawing/RenderPassRecorder.h
        public/command_buffer_cmds/CBC_Misc.h
        private/FrameInfo.cpp
        private/texture_mapping/ImageView.cpp
        private/texture_mapping/ImageView.h
        private/texture_mapping/TextureSampler.cpp
        private/texture_mapping/TextureSampler.h
        private/descriptor/DescriptorSetLayoutBinding.h
        private/presentation/WindowContext.cpp
        private/presentation/WindowContext.h
        private/misc/SubContext.h
        public/DeviceContext.h
        private/DeviceContext.cpp
        private/misc/ContextMacros.h
        public/VulkanSetupProcedure.h
        public/VulkanSetupProcedure.h
        private/misc/ObjectPool.h
        private/setup/VulkanQueue.cpp
        private/setup/VulkanQueue.h
        private/drawing/QueueContext.cpp
        private/drawing/QueueContext.h
        private/misc/SubContext.cpp
        private/graphics_pipeline/Material.cpp
        private/graphics_pipeline/Material.h
        private/graphics_pipeline/Shader.cpp
        private/graphics_pipeline/Shader.h
        private/descriptor/DescriptorPoolAllocator.cpp
        private/descriptor/DescriptorPoolAllocator.h
        private/descriptor/DescriptorWriter.cpp
        private/descriptor/DescriptorWriter.h
        private/misc/ObjectHierarchy.h
        private/texture_mapping/UnifiedTexture2D.h
        private/texture_mapping/UnifiedTexture2D.cpp
        private/misc/Mesh.cpp
        private/misc/Mesh.h
        private/vertex_buffer/MeshBuffer.cpp
        private/vertex_buffer/MeshBuffer.h
        private/misc/Camera.cpp
        private/misc/Camera.h
        private/misc/ObjectBehaviour.h
        private/misc/RenderingContext.cpp
        private/misc/RenderingContext.h
        private/misc/Transform.h
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
include_directories(${CMAKE_SOURCE_DIR}/libs/SPIRV-Cross)
include_directories(${CMAKE_SOURCE_DIR}/libs/rapidyaml)
include_directories(${CMAKE_SOURCE_DIR}/libs/shaderc)
include_directories(${CMAKE_SOURCE_DIR}/libs/flecs)

# TODO: move to lower-level module
include_directories(${CMAKE_SOURCE_DIR}/libs/fmt)


# Link GLFW, Vulkan, and GLM with the library
target_link_libraries(Vk glfw Vulkan::Vulkan glm)

# Add include directories to target
target_include_directories(Vk
        PUBLIC
        ${INCLUDE_DIRS}
)
