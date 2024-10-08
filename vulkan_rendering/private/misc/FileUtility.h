//
// Created by miska on 2024/02/11.
//

#ifndef VULKANTUT_FILEUTILITY_H
#define VULKANTUT_FILEUTILITY_H

#include <vector>
#include <string>

class FileUtility{
public:
    static std::vector<char> ReadFile(const std::string& filename);
};


#endif //VULKANTUT_FILEUTILITY_H
