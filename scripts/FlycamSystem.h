//
// Created by Tereza on 11/12/2024.
//

#pragma once
#include "EcsSystem.h"
#include "EventSystem.h"
#include "flecs.h"

struct Flycam {
    float pitch = 0, yaw = 0;
    float mouseSensitivity = 1 / 200.f;
};

class FlycamSystem : public EcsSystem {
public:
    void createSystem(flecs::world &ecs) override {
        ecs.observer<EventData>("Flycam-WASD").event(flecs::OnSet)
                .each([&ecs](flecs::entity entity, EventData &evt) {
                    auto query = ecs.query_builder<Velocity, Rotation>().with<Flycam>();
                    query.build();
                    query.each([&evt](flecs::iter &it, size_t index, Velocity &v, Rotation &rot) {
                        if (const auto e = evt.currentEvent;
                            e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_KEY_UP) {
                            switch (e.key.key) {
                                case SDLK_W: v.velocity.z = e.key.down ? -1.0f : 0.0f;
                                    break;
                                case SDLK_S: v.velocity.z = e.key.down ? 1.0f : 0.0f;
                                    break;
                                case SDLK_A: v.velocity.x = e.key.down ? -1.0f : 0.0f;
                                    break;
                                case SDLK_D: v.velocity.x = e.key.down ? 1.0f : 0.0f;
                                    break;
                            }
                        }
                    });
                });

        ecs.observer<EventData>("Flycam-Mouse").event(flecs::OnSet).each(
            [&ecs](flecs::entity entity, EventData &evt) {
                auto query = ecs.query_builder<Rotation, Flycam>();
                query.build();
                query.each([&evt](flecs::entity entity, Rotation &rot, Flycam &f) {
                    if (auto e = evt.currentEvent; e.type == SDL_EVENT_MOUSE_MOTION) {
                        //TODO: gimbal lock
                        f.yaw += e.motion.xrel * f.mouseSensitivity;
                        f.pitch -= e.motion.yrel * f.mouseSensitivity;
                        glm::quat pitchRotation = glm::angleAxis(f.pitch, glm::vec3{1.f, 0.f, 0.f});
                        glm::quat yawRotation = glm::angleAxis(f.yaw, glm::vec3{0.f, -1.f, 0.f});
                        rot.rotation = pitchRotation * yawRotation;
                    }
                });
            });
    }
};
