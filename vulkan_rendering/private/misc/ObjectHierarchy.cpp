//
// Created by Tereza on 10/31/2024.
//

#include "ObjectHierarchy.h"
#include "DeviceContext.h"

DeviceObjectPool::DeviceObjectPool() = default;

DeviceObjectPool::~DeviceObjectPool() {
    for (const auto object: objects) {
        delete object;
    }
}

void DeviceObjectPool::registerObject(ObjectHierarchy *object) {
    objects.insert(object);
}

void DeviceObjectPool::unregisterObject(ObjectHierarchy *object) {
    objects.erase(object);
}



ObjectHierarchy::ObjectHierarchy(DeviceObjectPool &pool) : pool(pool) {
    pool.registerObject(this);
}

ObjectHierarchy::~ObjectHierarchy() {
    pool.unregisterObject(this);
}
