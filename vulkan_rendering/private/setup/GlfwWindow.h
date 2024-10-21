//
// Created by miska on 2024/08/14.
//

#pragma once


#include "GLFW/glfw3.h"


struct WindowContext;

class GlfwWindow {
public:
    GlfwWindow(WindowContext &context, int w, int h, const char *appName, GLFWframebuffersizefun resizeFun);

    ~GlfwWindow();
    operator GLFWwindow*() const {return window;}

    void notifyResize();
private:
    const WindowContext& ctx;
    GLFWwindow* window;
};


