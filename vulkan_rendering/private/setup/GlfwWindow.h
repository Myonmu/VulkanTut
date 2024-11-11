//
// Created by miska on 2024/08/14.
//

#pragma once


#include <SDL3/SDL_video.h>

struct WindowContext;

class SdlWindow {
public:
    SdlWindow(WindowContext &context, int w, int h, const char *appName);

    ~SdlWindow();
    operator SDL_Window*() const {return window;}

    SDL_WindowID get_SdlWindowId() const ;
private:
    const WindowContext& ctx;
    SDL_Window* window;
};


