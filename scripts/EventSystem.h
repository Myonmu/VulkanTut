//
// Created by Tereza on 11/12/2024.
//

#pragma once
#include <SDL3/SDL_events.h>

struct EventData {
    SDL_Event currentEvent;
};

struct EventSystem {
    flecs::entity entity;

    explicit EventSystem(const flecs::world& ecs) {
        entity = ecs.entity("EventSystem");
    }

    void pushSdlEvent(const SDL_Event &e) const {
        entity.set(EventData{e});
    }
};
