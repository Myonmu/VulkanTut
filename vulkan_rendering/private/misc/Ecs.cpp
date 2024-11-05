//
// Created by Tereza on 11/5/2024.
//

#include "Ecs.h"

Entity::Entity(const World &world): entity(world.world.entity()) {
}


template<ComponentType T>
const T *Entity::getComponent() {
    return entity.get<T>();
}

template<ComponentType T>
const T *Entity::addComponent() {
    
}

