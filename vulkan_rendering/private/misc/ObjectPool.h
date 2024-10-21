//
// Created by miska on 2024/10/19.
//

#pragma once

#include <functional>
#include <queue>

template<typename T>
class ObjectPool<T> {
    std::function<T()> onObjectCreate;
    std::queue<T> queue{};

public:
    explicit ObjectPool(std::function<T()> onObjectCreate): onObjectCreate(onObjectCreate) {
    }

    T get() {
        if (queue.empty()) { return onObjectCreate(); }
        return queue.front();
    }

    void release(T object) {
        queue.push(object);
    }
};

