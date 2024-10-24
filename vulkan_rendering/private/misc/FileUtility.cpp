//
// Created by miska on 2024/02/11.
//

#include <fstream>
#include "FileUtility.h"

#include <sstream>

std::vector<char> FileUtility::ReadFile(const std::string &filename) {
    std::vector<char> data;
    std::ifstream file(filename);

    // Check if the file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

std::vector<uint32_t> FileUtility::ReadSpv(const std::string &filename) {
    std::vector<uint32_t> data;
    std::ifstream file(filename, std::ios::binary);

    // Check if the file opened successfully
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    // Read the file size
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Calculate number of uint32_t elements
    size_t numElements = size / sizeof(uint32_t);
    data.resize(numElements);

    // Read the file into the vector
    if (!file.read(reinterpret_cast<char*>(data.data()), size)) {
        throw std::runtime_error("Error reading file: " + filename);
    }

    file.close();
    return data;
}
