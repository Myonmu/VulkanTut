//
// Created by miska on 2024/10/13.
//

#ifndef SUBCONTEXT_H
#define SUBCONTEXT_H


class LogicalDevice;

template <typename T>
struct SubContext<T> {
    virtual ~SubContext() = default;
    explicit SubContext(const T& parent):context(parent) {

    }
    T& context;
    [[nodiscard]] virtual const LogicalDevice& getLogicalDevice() const {
        return context.getLogicalDevice();
    }
};



#endif //SUBCONTEXT_H
