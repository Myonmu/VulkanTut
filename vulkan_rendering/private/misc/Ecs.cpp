//
// Created by Tereza on 11/5/2024.
//

#include "Ecs.h"

#include "Transform.h"

Entity World::createEntity(const char* name) const {
    return Entity(this, name);
}

Entity World::createEntityWithTransform(const char *name) const {
    Entity entity(this, name);
    entity.addComponent<Transform>();
    return entity;
}








