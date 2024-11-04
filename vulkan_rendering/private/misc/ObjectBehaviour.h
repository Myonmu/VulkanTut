//
// Created by miska on 2024/11/04.
//

#pragma once
#include "ObjectHierarchy.h"


struct FrameInfo;

class ObjectBehaviour: public ObjectNode {
public:
    virtual void onFrameUpdate(const FrameInfo& frameInfo) = 0;
};
