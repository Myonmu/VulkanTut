//
// Created by miska on 2024/10/13.
//

#ifndef SUBCONTEXT_H
#define SUBCONTEXT_H


class LogicalDevice;

template <typename T>
struct SubContext {
    virtual ~SubContext() = default;
    explicit SubContext(T& parent):context(parent) {

    }

    T &context;
    [[nodiscard]] virtual LogicalDevice& getLogicalDevice() const;
};



#endif //SUBCONTEXT_H
