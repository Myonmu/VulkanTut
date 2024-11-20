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
        ${CMAKE_CURRENT_SOURCE_DIR}/private/vma
        ${CMAKE_CURRENT_SOURCE_DIR}/private/components
        ${CMAKE_CURRENT_SOURCE_DIR}/private/model_loading
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
        private/vma/VmaInstance.cpp
        private/descriptor/DescriptorSetLayout.cpp
        private/uniform_buffer/PerFrameBufferGroup.h
        private/uniform_buffer/PerFrameBufferGroup.cpp
        private/descriptor/DescriptorPool.cpp
        private/descriptor/DescriptorSet.cpp
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
        private/components/Mesh.cpp
        private/components/Mesh.h
        private/vertex_buffer/MeshBuffer.cpp
        private/vertex_buffer/MeshBuffer.h
        private/components/Camera.cpp
        private/components/Camera.h
        private/misc/RenderingContext.cpp
        private/misc/RenderingContext.h
        private/Components/Transform.h
        private/misc/EnginePipeline.h
        private/presentation/RenderAttachment.cpp
        private/presentation/RenderAttachment.h
        private/misc/EcsSystem.h
        private/model_loading/GltfLoader.cpp
        private/model_loading/GltfLoader.h
        private/model_loading/ObjLoader.cpp
        private/model_loading/ObjLoader.h
        private/components/Lights.cpp
        private/components/Lights.h
        private/descriptor/EngineDescriptorDef.h
        private/descriptor/EngineDescriptorDef.cpp
        private/graphics_pipeline/RenderGraph.cpp
        private/graphics_pipeline/RenderGraph.h
        private/misc/UtilityMacros.h
        private/misc/CompoundDataContainers.h
)

# Create static library
add_library(Vk STATIC ${SOURCES})

# Vulkan package
find_package(Vulkan REQUIRED)

find_package(fmt CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
set(SDL_VULKAN ON CACHE BOOL "")
find_package(glm CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
find_package(fastgltf CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(spirv_cross_core CONFIG REQUIRED)
find_package(spirv_cross_util CONFIG REQUIRED)
find_package(spirv_cross_glsl CONFIG REQUIRED)
find_package(boost_bimap CONFIG REQUIRED)

# Link GLFW, Vulkan, and GLM with the library
target_link_libraries(Vk
        Vulkan::Vulkan
        fmt::fmt
        SDL3::SDL3-shared
        glm::glm
        spirv-cross-core
        spirv-cross-util
        spirv-cross-glsl
        Boost::bimap
)
target_link_libraries(Vk flecs::flecs)

# Add include directories to target
target_include_directories(Vk
        PUBLIC
        ${INCLUDE_DIRS}
)
