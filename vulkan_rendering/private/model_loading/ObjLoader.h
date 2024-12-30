//
// Created by miska on 2024/11/12.
//

#pragma once
#include <complex.h>
#include <string>
#include <tiny_obj_loader.h>
#include <vector>
#include <future>

#include "Vertex.h"


class ObjLoader {
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    void computeTangent();

    std::future<void> loadGeometryAsync(const std::string& path);
    void loadGeometry(const std::string& path);
};
