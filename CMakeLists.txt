cmake_minimum_required(VERSION 3.27)
project(VulkanTut)

set(CMAKE_CXX_STANDARD 23)

# Vulkan and GLFW setup
find_package(Vulkan REQUIRED)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add subdirectories
add_subdirectory(libs/SDL)
add_subdirectory(libs/glm)
add_subdirectory(libs/SPIRV-Cross)
add_subdirectory(vulkan_rendering)
add_subdirectory(libs/flecs)
add_subdirectory(libs/fmt)
add_subdirectory(libs/fastgltf)

# Application executable
add_executable(VulkanTut
        main.cpp
        TriangleApp.h
        AppSetup.h
        scripts/FlycamSystem.h
        EcsSystemsHeader.h
        scripts/SimplePhysics.h
        scripts/MeshRotate.h
        scripts/EventSystem.h
)

# Link dependencies
target_link_libraries(VulkanTut
        Vk # This is your core module
        SDL3::SDL3-static
        Vulkan::Vulkan
        glm
        spirv-cross-core
        spirv-cross-util
        spirv-cross-glsl
        fmt::fmt
)

# Include directories for the app
target_include_directories(VulkanTut PUBLIC
        ${CMAKE_SOURCE_DIR}/core/public
)

add_custom_command(TARGET VulkanTut POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders )
add_custom_command(TARGET VulkanTut POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/textures ${CMAKE_CURRENT_BINARY_DIR}/textures )
add_custom_command(TARGET VulkanTut POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets )