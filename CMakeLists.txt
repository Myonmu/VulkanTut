cmake_minimum_required(VERSION 3.27)
project(VulkanTut)

set(CMAKE_CXX_STANDARD 23)
# Vulkan and GLFW setup
find_package(Vulkan REQUIRED)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add subdirectories
add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
add_subdirectory(vulkan_rendering)

# Application executable
add_executable(VulkanTut
        main.cpp
        TriangleApp.h
        AppSetup.h
)

# Link dependencies
target_link_libraries(VulkanTut
        Vk # This is your core module
        glfw
        Vulkan::Vulkan
        glm
)

# Include directories for the app
target_include_directories(VulkanTut PUBLIC
        ${CMAKE_SOURCE_DIR}/core/public
)
