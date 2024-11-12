//
// Created by miska on 2024/11/12.
//

#pragma once
#include <string>
#include <vector>

#include "Vertex.h"


class ObjLoader {
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    void LoadGeometry(const std::string& path);
    void LoadTexture(const std::string& path);
};
