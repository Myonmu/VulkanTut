//
// Created by Tereza on 10/31/2024.
//
#pragma once
#include <unordered_set>
#include <memory>

class ObjectNode;

class ObjectNode {
    ObjectNode &parent;
    std::unordered_set<ObjectNode> children;
public:
    ObjectNode(ObjectNode &pool);
    virtual ~ObjectNode();
};

#define OBJECT_HIERARCHY_NODE\
  private: DeviceObjectPool objectPool{};\
  public: inline DeviceObjectPool& get_objectPool(){return objectPool;};
