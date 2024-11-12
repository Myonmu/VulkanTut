//
// Created by Tereza on 11/12/2024.
//

#pragma once


namespace flecs {
    struct world;
}

class EcsSystem {
public:
    virtual ~EcsSystem() = default;
    virtual void createSystem(flecs::world& ecs) = 0;
};
