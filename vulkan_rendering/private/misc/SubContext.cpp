//
// Created by miska on 2024/10/20.
//

#include "SubContext.h"

template<typename T>
LogicalDevice &SubContext<T>::getLogicalDevice() const {
    return context->getLogicalDevice();
}
