//
// Created by miska on 2024/08/14.
//

#include "GlfwWindow.h"
#include <iostream>
#include <SDL3/SDL_log.h>

#include "WindowContext.h"

SdlWindow::SdlWindow(WindowContext &context, int w, int h, const char *appName)
    : ctx(context) {
    std::cout << "INIT SDL WINDOW" << std::endl;
    SDL_PropertiesID props = SDL_CreateProperties();
    if(props == 0) {
        SDL_Log("Unable to create properties: %s", SDL_GetError());
        throw;
    }

    // Assume the following calls succeed
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, appName);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, w);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, h);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN, true);
    // TODO: Funny experiment (and doesn't work yet), pass as construction flags in the future
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN, true);
    //SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);

    window = SDL_CreateWindowWithProperties(props);
    if(window == nullptr) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        throw;
    }
}

SdlWindow::~SdlWindow() {
    SDL_DestroyWindow(window);
}

SDL_WindowID SdlWindow::get_SdlWindowId() const {
    return SDL_GetWindowID(window);
}
