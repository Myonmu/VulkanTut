//
// Created by miska on 2024/08/08.
//

#include "Vertex.h"


const std::vector<uint32_t> Vertex::testIndices = {
    0, 1, 2, 2, 3, 0
};

const std::vector<Vertex> Vertex::testVerts = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};
