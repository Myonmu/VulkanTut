//
// Created by miska on 2024/10/08.
//

#ifndef FRAMEINFO_H
#define FRAMEINFO_H
#include <cstdint>

struct FrameInfo {
    uint32_t windowId = 0;
    uint32_t imageIndex = 0;
    uint32_t currentFrameIndex = 0;
    static const FrameInfo DONT_CARE;
};
#endif //FRAMEINFO_H
