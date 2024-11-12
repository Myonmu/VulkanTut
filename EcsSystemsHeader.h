//
// Created by Tereza on 11/12/2024.
//

#pragma once
#include "flecs.h"
#include "scripts/FlycamSystem.h"
#include "scripts/MeshRotate.h"
#include "scripts/SimplePhysics.h"


struct EcsSystemsHeader {
    void createSystems(flecs::world &ecs) {
#define CREATE_ECS_SYSTEM(SysType)\
    {SysType s{}; s.createSystem(ecs); }

        CREATE_ECS_SYSTEM(FlycamSystem)
        CREATE_ECS_SYSTEM(LinearPositionUpdate)
        CREATE_ECS_SYSTEM(MeshRotate)
    }
};
