//
// Created by miska on 2024/08/14.
//

#include "GlfwWindow.h"
#include <iostream>
#include <VulkanAppContext.h>

GlfwWindow::GlfwWindow(VulkanAppContext& context, int w, int h, const char *appName, const GLFWframebuffersizefun resizeFun)
    :ctx(context){
    std::cout << "INIT GLFW WINDOW" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(w, h, appName, nullptr, nullptr);
    glfwSetWindowUserPointer(window, &ctx);
    glfwSetFramebufferSizeCallback(window, resizeFun);
}

GlfwWindow::~GlfwWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

