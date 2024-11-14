cmake_minimum_required(VERSION 3.27)
project(VulkanTut)

set(CMAKE_CXX_STANDARD 23)
# Vulkan and GLFW setup
find_package(Vulkan REQUIRED)

find_package(SDL3 CONFIG REQUIRED)
set(SDL_VULKAN ON CACHE BOOL "")

find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)

add_subdirectory(vulkan_rendering)

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
        Vk
        Vulkan::Vulkan
        glm::glm
        fmt::fmt
        flecs::flecs
        SDL3::SDL3-shared
)

# Include directories for the app
target_include_directories(VulkanTut PUBLIC
        ${CMAKE_SOURCE_DIR}/core/public
)

add_custom_command(TARGET VulkanTut POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders )
add_custom_command(TARGET VulkanTut POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures ${CMAKE_CURRENT_BINARY_DIR}/textures )
add_custom_command(TARGET VulkanTut POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets )