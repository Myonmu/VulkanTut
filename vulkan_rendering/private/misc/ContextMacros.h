//
// Created by Tereza on 10/15/2024.
//

#pragma once


#include <memory>

#define CTX_PROPERTY(type, name)\
    private: std::unique_ptr<type> name;\
    public: inline type& get_##name() const {return *(name);}

#define CTX_PROPERTY_RAW_PTR(type, name)\
    type* name;\
    inline type& get_##name() const {return *(name);}

#define CTX_PROPERTY_LIST(type, name)\
    std::vector<std::unique_ptr<type>> (name);\
    template<typename... Args>\
    inline uint32_t create_##name##(Args&&... VAR_ARGS){\
        (name).emplace_back(std::make_unique<type>(*this, std::forward<Args>(VAR_ARGS)...));\
        return (name).size() - 1;\
    };\
    inline type & get_##name##_at(uint32_t i) const {return * name[i];}

#define CTX_FORWARD_GET_DECL(type, name)\
    const type& get_##name() const;

#define CTX_FORWARD_GET_BODY(belongType, type, name)\
    const type& belongType::get_##name() const{ return context.get_##name(); }

