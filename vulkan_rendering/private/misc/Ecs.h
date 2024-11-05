//
// Created by Tereza on 11/5/2024.
//

#pragma once
#include <flecs.h>

class Component;
class Entity;

template<typename T>
concept EntityType = std::derived_from<T, Entity>;

template<typename T>
concept ComponentType = std::derived_from<T, Component>;

class World {
    flecs::world world{};
    friend class Entity;
};

class Entity {
    flecs::entity entity;
public:
    explicit Entity(const World& world);

    template<ComponentType T>
    const T* getComponent();

    template<ComponentType T>
    const T* addComponent();
};

class Component {

};
