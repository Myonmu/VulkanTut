//
// Created by Tereza on 10/31/2024.
//
#pragma once
#include <unordered_map>
#include <memory>
#include <ranges>

#include "Transform.h"
#include "ContextMacros.h"

class ObjectNode;
class SceneObject;

template<class T>
concept ObjectNodeType = std::derived_from<T, ObjectNode>;

template<class T>
concept SceneObjectType = std::derived_from<T, SceneObject>;

class ObjectNode {

    ObjectNode *parent;
    std::vector<ObjectNode *> order;
    std::unordered_map<ObjectNode *, std::unique_ptr<ObjectNode> > children_map; // For ownership

protected:
    int getChildIndex(const ObjectNode *ptr) const {
        int i = 0;
        for (auto &child: order) {
            if (child == ptr) {
                return i;
            }
            i++;
        }
        return -1;
    }

public:
    explicit ObjectNode(ObjectNode *parent = nullptr) : parent(parent) {
    }

    virtual ~ObjectNode() {
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            (*it)->parent = nullptr;
            children_map.erase(*it);
        }
        children_map.clear();
        if (parent != nullptr) {
            parent->children_map.erase(this);
        }
    }

    // Template function to create and add a child of derived type T
    template<ObjectNodeType T, typename... Args>
    T &createChildObject(Args &&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        child->parent = this;
        T *rawPtr = child.get();
        children_map[rawPtr] = std::move(child); // Manage ownership with map
        order.push_back(rawPtr);
        return *rawPtr;
    }

    // Function to attach an existing child object
    template<ObjectNodeType T>
    T &attachChild(std::unique_ptr<T> child) {
        child->parent = this;
        T *rawPtr = child.get();
        children_map[rawPtr] = std::move(child); // Transfer ownership
        order.push_back(rawPtr);
        return *rawPtr;
    }

    [[nodiscard]] int getSelfIndex() const {
        if (parent == nullptr) { return -1; }
        return parent->getChildIndex(this);
    }

    // Detach a specific child and return it as a unique_ptr
    template<ObjectNodeType T>
    std::unique_ptr<T> detachChild(T *child) {
        // Check if the child exists in the set for O(1) lookup
        auto p = dynamic_cast<ObjectNode *>(child);
        if (children_map.contains(p)) {
            // If erased from set, it exists
            std::unique_ptr<T> detachedChild = std::unique_ptr<T>(
                dynamic_cast<T *>(std::move(children_map[p]).release()));
            children_map.erase(p); // Remove ownership
            order.erase(order.begin() + getChildIndex(p));
            detachedChild->parent = nullptr; // Clear parent pointer
            return detachedChild; // Return ownership
        }
        return nullptr; // Child not found
    }
};

class SceneObject : public ObjectNode {
    CTX_PROPERTY(Transform, transform)
};
