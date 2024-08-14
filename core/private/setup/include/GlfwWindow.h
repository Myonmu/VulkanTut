//
// Created by miska on 2024/08/14.
//

#ifndef GLFWWINDOW_H
#define GLFWWINDOW_H

#include "GLFW/glfw3.h"


struct VulkanAppContext;

class GlfwWindow {
public:
    GlfwWindow(VulkanAppContext &context, int w, int h, const char *appName, GLFWframebuffersizefun resizeFun);

    ~GlfwWindow();
    operator GLFWwindow*() const {return window;} // NOLINT(*-explicit-constructor)
private:
    VulkanAppContext& ctx;
    GLFWwindow* window;
};



#endif //GLFWWINDOW_H
