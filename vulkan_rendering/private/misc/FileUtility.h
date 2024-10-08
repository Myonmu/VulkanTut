//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_FILEUTILITY_H
#define VULKANTUT_FILEUTILITY_H

#include <vector>
#include <string>

//TODO: File operations are more low-level/universal than rendering, consider moving it to another cpp module
class FileUtility{
public:
    static std::vector<char> ReadFile(const std::string& filename);
};


#endif //VULKANTUT_FILEUTILITY_H
