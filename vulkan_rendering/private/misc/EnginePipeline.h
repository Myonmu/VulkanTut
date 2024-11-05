//
// Created by Tereza on 11/5/2024.
//

#pragma once
#include <functional>


class EnginePipeline {
    std::vector<std::function<bool()>> loops;
public:
    void addLoop(const std::function<bool()> &f) {
        loops.push_back(f);
    }
    bool mainLoop() const {
        for (auto& f : loops) {
            if (!f()) {
                return false;
            }
        }
        return true;
    }
};
