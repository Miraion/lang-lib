//
// Created by Jeremy S on 2018-01-16.
//

#pragma once

#include "../foundation/__base.hpp"
#include <lang/print.hpp>

__LANG_NAMESPACE

    template<typename T>
    class object_traits {
    public:

        typedef T type;

        template<typename U>
        struct rebind {
            typedef object_traits<U> type;
        };

        // Default Constructor
        object_traits() = default;

        // Copy Constructor
        template<typename U>
        object_traits(object_traits<U> const &other) {}

        type*       address(type       &obj) { return &obj; }
        type const* address(type const &obj) { return &obj; }

        template<typename... Args>
        void construct(type *p, Args... args)
        { new(p) type(std::forward<Args>(args)...); }

        void destroy(type *p) { p->~type(); }

    };

__LANG_NAMESPACE_END
