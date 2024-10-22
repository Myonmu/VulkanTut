//
// Created by miska on 2024/02/11.
//

#pragma once


#include <vector>
#include <string>

//TODO: File operations are more low-level/universal than rendering, consider moving it to another cpp module
class FileUtility{
public:
    template<typename T>
    static std::vector<T> ReadFile(const std::string& filename);

    static std::vector<char> ReadFile(const std::string& filename);
    static std::vector<uint32_t> ReadSpv(const std::string& filename);
};


