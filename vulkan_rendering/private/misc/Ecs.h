//
// Created by Tereza on 11/5/2024.
//

#pragma once
#include <flecs.h>
#include <string>

class Component;
class Entity;

template<typename T>
concept EntityType = std::derived_from<T, Entity>;

template<typename T>
concept ComponentType = std::derived_from<T, Component>;

class World {
    flecs::world world{};
    friend class Entity;

public:
    [[nodiscard]] Entity createEntity(const char* name = nullptr) const;

    [[nodiscard]] Entity createEntityWithTransform(const char* name = nullptr) const;

    flecs::world &getRaw() { return world; }
    operator const flecs::world &() const { return world; }
};

class Entity {
    flecs::entity entity;

public:
    explicit Entity(const World &world, const char* name = nullptr):
        entity( name != nullptr ? world.world.entity(name) : world.world.entity()){};

    template<ComponentType T>
    const T *getComponent() {
        return entity.get<T>();
    }

    template<ComponentType T, typename... Args>
    const T *addComponent(Args &&... args) {
        auto &e = entity.emplace<T>(*this, std::forward<Args>(args)...);
        return entity.get<T>();
    }


    template<ComponentType T>
    void removeComponent() {
        entity.remove<T>();
    }

    operator const flecs::entity &() const { return entity; }
};

class Component {
protected:
    Entity &parent;

public:
    explicit Component(Entity &entity): parent(entity) {
    }
};
