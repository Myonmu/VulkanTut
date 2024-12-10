//
// Created by Tereza on 11/20/2024.
//

#pragma once

/**
 * Shorthand for writing C# like properties
 * @param Type type of the property
 * @param name name of the property
 * @param getAccess immutable get accessor specifier (public/private/protected)
 * @param setAccess set and mutable get accessor specifier (public/private/protected)
 */
#define PROPERTY(Type, name, getAccess, setAccess)\
    private: Type name;\
    getAccess: const Type& get_##name##() const {return name;}\
    setAccess: void set_##name##(const Type& val) { name = val; }\
    setAccess: Type& get_##name##_mut() {return name;}\

#define PROPERTY_UNIQUE_PTR(Type, name)\
    private: std::unique_ptr<Type> name;\
    public: const Type& get_##name##() const {return *name;}

/**
 * Obtain polymorphic reference from a unique pointer (unique_ptr<T> -> U&)
 * @param ptr a unique ptr of type T
 * @param type base or derived type of T
 */
#define UNIQUE_PTR_GET_REF(ptr, type) *dynamic_cast<type *>(ptr.get())

/**
 * cast a reference to another type
 * @param ref a reference of type T
 * @param type base or derived type of T
 */
#define REFERENCE_CAST(ref, type) *dynamic_cast<type *>(&ref)