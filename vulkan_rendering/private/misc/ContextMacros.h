//
// Created by Tereza on 10/15/2024.
//

#ifndef CONTEXTMACROS_H
#define CONTEXTMACROS_H

#define CTX_PROPERTY(type, name)\
    std::unique_ptr<type> name;\
    inline const type& get_##name() const {return *name;}

#define CTX_PROPERTY_RAW_PTR(type, name)\
    type* name;\
    inline const type& get_##name() const {return *name;}

#define CTX_PROPERTY_LIST(type, name)\
    std::vector<type> name;\
    inline

#define CTX_FORWARD_GET_DECL(type, name)\
    inline const type& get_##name() const;

#define CTX_FORWARD_GET_BODY(belongType, type, name)\
    inline const type& belongType::get_##name() const{ return context.get_##name(); }

#endif //CONTEXTMACROS_H
