//
// Created by Tereza on 11/12/2024.
//

#pragma once
#include "EcsSystem.h"
#include "EventSystem.h"
#include "flecs.h"

struct Flycam {
    float pitch = 0, yaw = 0;
    float lookSensitivity = 1 / 250.f;
    float panSensitivity = 0.01f;
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
                auto query = ecs.query_builder<Rotation, Position, Flycam>();
                query.build();
                query.each([&evt](flecs::entity entity, Rotation &rot, Position &p, Flycam &f) {
                    if (auto e = evt.currentEvent; e.type == SDL_EVENT_MOUSE_MOTION) {
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            f.yaw += e.motion.xrel * f.lookSensitivity;
                            f.pitch -= e.motion.yrel * f.lookSensitivity;
                            glm::quat pitchRotation = glm::angleAxis(f.pitch, glm::vec3{1.f, 0.f, 0.f});
                            glm::quat yawRotation = glm::angleAxis(f.yaw, glm::vec3{0.f, -1.f, 0.f});
                            // ok we need to pass via matrix to avoid gimbal lock (heh?)
                            rot.rotation = glm::toQuat(glm::toMat4(yawRotation) * glm::toMat4(pitchRotation));
                        } else if (e.button.button == SDL_BUTTON_MIDDLE) {
                            auto rotMat = rot.getRotationMatrix();
                            auto x = e.motion.xrel * f.panSensitivity;
                            auto y = e.motion.yrel * f.panSensitivity;
                            p.translation += glm::vec3(rotMat * glm::vec4(x, y, 0, 0.f));
                        }
                    }
                });
            });
    }
};
