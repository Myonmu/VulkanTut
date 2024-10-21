//
// Created by Tereza on 10/18/2024.
//

#pragma once

enum class LifetimeBarrierEventType {

};

// controls initialization
class LifetimeBarrier {
public:
    void registerAction(void* functionPtr);
    void notify();
};


