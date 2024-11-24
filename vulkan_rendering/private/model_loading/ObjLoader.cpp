//
// Created by miska on 2024/11/12.
//

#include "ObjLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void ObjLoader::LoadGeometry(const std::string &path) {
    tinyobj::ObjReader reader{};
    reader.ParseFromFile(path);
    if (!reader.Valid()) {
        throw std::runtime_error(reader.Error());
    }
    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    auto shapes = reader.GetShapes();
    auto materials = reader.GetMaterials();
    auto attrib = reader.GetAttrib();

    for (const auto &shape: shapes) {
        for (const auto &index: shape.mesh.indices) {
            Vertex vertex{};
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };



            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void ObjLoader::LoadTexture(const std::string &path) {
}
