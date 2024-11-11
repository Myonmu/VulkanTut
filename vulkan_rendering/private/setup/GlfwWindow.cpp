//
// Created by miska on 2024/08/14.
//

#include "GlfwWindow.h"
#include <iostream>

#include "WindowContext.h"

SdlWindow::SdlWindow(WindowContext &context, int w, int h, const char *appName)
    : ctx(context) {
    std::cout << "INIT GLFW WINDOW" << std::endl;
    window = SDL_CreateWindow(
        appName,
        w, h,
        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
    );
}

SdlWindow::~SdlWindow() {
    SDL_DestroyWindow(window);
}

SDL_WindowID SdlWindow::get_SdlWindowId() const {
    return SDL_GetWindowID(window);
}
