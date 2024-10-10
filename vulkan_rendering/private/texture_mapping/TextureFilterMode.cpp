//
// Created by miska on 10/10/2024.
//

#include "TextureFilterMode.h"

const TextureFilterMode TextureFilterMode::LINEAR = {VK_FILTER_LINEAR, VK_FILTER_LINEAR};

const TextureFilterMode TextureFilterMode::NEAREST = {VK_FILTER_NEAREST, VK_FILTER_NEAREST};