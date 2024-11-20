//
// Created by Tereza on 11/20/2024.
//

#pragma once

#define PROPERTY(Type, name, getAccess, setAccess)\
    private: Type name;\
    getAccess: const Type& get_##name##() const {return name;}\
    setAccess: void set_##name##(const Type& val) { name = val; }\
    setAccess: Type& get_##name##_mut() const {return name;}\

#define PROPERTY_UNIQUE_PTR(Type, name)\
    private: std::unique_ptr<Type> name;\
    public: const Type& get_##name##() const {return *name;}