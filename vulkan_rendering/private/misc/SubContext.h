//
// Created by miska on 2024/10/13.
//

#pragma once



class LogicalDevice;

template <typename T>
struct SubContext {
    virtual ~SubContext() = default;
    explicit SubContext(T& parent):context(parent) {

    }

    T &context;
    [[nodiscard]] virtual LogicalDevice& getLogicalDevice() const;
};


