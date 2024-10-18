//
// Created by Tereza on 10/18/2024.
//

#ifndef LIFETIMEBARRIER_H
#define LIFETIMEBARRIER_H

enum class LifetimeBarrierEventType {

};

// controls initialization
class LifetimeBarrier {
public:
    void registerAction(void* functionPtr);
    void notify();
};



#endif //LIFETIMEBARRIER_H
