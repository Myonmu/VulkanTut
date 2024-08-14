cmake_minimum_required(VERSION 3.27)
project(Vk)
set(CMAKE_CXX_STANDARD 17)

# Add subdirectories' include paths
set(INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/private/graphics_pipeline/include
        ${CMAKE_CURRENT_SOURCE_DIR}/private/misc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/private/presentation/include
        ${CMAKE_CURRENT_SOURCE_DIR}/private/setup/include
        ${CMAKE_CURRENT_SOURCE_DIR}/private/drawing/include
        ${CMAKE_CURRENT_SOURCE_DIR}/private/vertex_buffer/include
        ${CMAKE_CURRENT_SOURCE_DIR}/public/include
)

# Source files
set(SOURCES
        private/graphics_pipeline/src/GraphicsPipeline.cpp
        private/graphics_pipeline/src/RenderPass.cpp
        private/graphics_pipeline/src/ShaderModule.cpp
        private/graphics_pipeline/src/VulkanPipeline.cpp
        private/misc/src/FileUtility.cpp
        private/presentation/src/SwapChain.cpp
        private/setup/src/LogicalDevice.cpp
        private/setup/src/PhysicalDevice.cpp
        private/setup/src/ValidationLayers.cpp
        private/setup/src/VulkanInstance.cpp
        private/presentation/src/VulkanSurface.cpp
        private/drawing/src/FrameBuffers.cpp
        private/drawing/src/CommandPool.cpp
        private/drawing/src/CommandBuffer.cpp
        private/drawing/src/CommandBufferRecorder.cpp
        private/drawing/src/VulkanFrame.cpp
        public/src/VulkanRenderer.cpp
        public/include/CommandBufferCommand.h
        public/src/VulkanAppContext.cpp
        private/vertex_buffer/include/Vertex.h
        private/vertex_buffer/src/VertexBuffer.cpp
        private/vertex_buffer/src/Vertex.cpp
        private/vertex_buffer/src/Buffer.cpp
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
include_directories(${CMAKE_SOURCE_DIR}/libs/glfw-3.3.9/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glm-1.0.0/include)

# Link GLFW, Vulkan, and GLM with the library
target_link_libraries(Vk glfw Vulkan::Vulkan glm)

# Add include directories to target
target_include_directories(Vk
        PUBLIC
        ${INCLUDE_DIRS}
)
