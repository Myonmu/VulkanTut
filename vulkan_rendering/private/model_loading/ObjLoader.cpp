//
// Created by miska on 2024/11/12.
//

#include "ObjLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

//TODO: multithreading

// uniqueVertices maps vertex to its index in vertex array.
void ObjLoader::computeTangent() {
    // Compute tangents and bitangents
    for (size_t i = 0; i < indices.size(); i += 3) {
        // Get the indices of the 3 vertices in the triangle
        auto idx0 = indices[i + 0];
        auto idx1 = indices[i + 1];
        auto idx2 = indices[i + 2];

        // Get positions and texture coordinates of the 3 vertices
        glm::vec3 v0 = vertices[idx0].pos;
        glm::vec3 v1 = vertices[idx1].pos;
        glm::vec3 v2 = vertices[idx2].pos;

        glm::vec2 uv0 = vertices[idx0].texCoord;
        glm::vec2 uv1 = vertices[idx1].texCoord;
        glm::vec2 uv2 =vertices[idx2].texCoord;

        // Compute edges and delta UVs
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;

        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float det = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;

        if (fabs(det) < 1e-6f) {
            continue; // Skip degenerate triangle
        }

        // Compute the tangent and bitangent
        float f = 1.0f / det;
        glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
        glm::vec3 bitangent = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

        // Add the computed tangents and bitangents to the correct vertices
        vertices[idx0].tangent += tangent;
        vertices[idx1].tangent += tangent;
        vertices[idx2].tangent += tangent;

        //vertices[idx0].bitangent += bitangent;
        //vertices[idx1].bitangent += bitangent;
        //vertices[idx2].bitangent += bitangent;
    }

    // Normalize tangents and bitangents
    for (auto & vertex : vertices) {
        vertex.normal = glm::normalize(vertex.normal);
        //vertex.bitangent = glm::normalize(vertex.bitangent);

        // Ensure tangent space is orthogonal
        glm::vec3 normal = vertex.normal;
        vertex.tangent = glm::normalize(vertex.tangent - glm::dot(vertex.tangent, normal) * normal);
        //vertex.bitangent = glm::normalize(glm::cross(normal, vertex.bitangent));
    }
}

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

            if (!uniqueVertices.contains(vertex)) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }

    computeTangent();
}

void ObjLoader::LoadTexture(const std::string &path) {
}
