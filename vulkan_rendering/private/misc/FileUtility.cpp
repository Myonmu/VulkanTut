//
// Created by miska on 2024/02/11.
//

#include <fstream>
#include "FileUtility.h"

template<typename T>
std::vector<T> FileUtility::ReadFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSize = file.tellg();
    std::vector<T> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}


std::vector<char> FileUtility::ReadFile(const std::string &filename) {
    return ReadFile<char>(filename);
}

std::vector<uint32_t> FileUtility::ReadSpv(const std::string &filename) {
    return ReadFile<uint32_t>(filename);
}
