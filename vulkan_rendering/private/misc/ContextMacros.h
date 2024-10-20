//
// Created by Tereza on 10/15/2024.
//

#ifndef CONTEXTMACROS_H
#define CONTEXTMACROS_H

#include <memory.h>

#define CTX_PROPERTY(type, name)\
    std::unique_ptr<type> name;\
    inline const type& get_##name() const {return *(name);}

#define CTX_PROPERTY_RAW_PTR(type, name)\
    type* name;\
    inline const type& get_##name() const {return *(name);}

#define CTX_PROPERTY_LIST(type, name)\
    std::vector<std::unique_ptr<type>> (name);\
    template<typename... Args>\
    inline void create_##name##(Args&&... VAR_ARGS){(name).emplace_back(std::make_unique<type>(*this, std::forward<Args>(VAR_ARGS)...));};\
    inline type & get_##name##_at(uint32_t i) const {return * name[i];}

#define CTX_FORWARD_GET_DECL(type, name)\
    inline const type& get_##name() const;

#define CTX_FORWARD_GET_BODY(belongType, type, name)\
    inline const type& belongType::get_##name() const{ return context.get_##name(); }

#endif //CONTEXTMACROS_H
