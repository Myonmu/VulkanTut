//
// Created by Tereza on 11/27/2024.
//

#pragma once
#include <memory>
#include <vector>

/**
 * Polymorphic vector container, uses unique ptr
 * @tparam T base type
 */
template<typename T>
struct poly_vector {
    std::vector<std::unique_ptr<T> > list;

    template<typename ActualT, typename... Args>
    ActualT &emplace_back(Args &&... args) {
        auto o = std::make_unique<ActualT>(std::forward<Args>(args)...);
        ActualT *rawPtr = o.get();
        list.push_back(std::move(o));
        return *rawPtr;
    }

    template<typename ActualT>
    ActualT &get_as(int index) {
        return *dynamic_cast<ActualT *>(list[index].get());
    }
};
