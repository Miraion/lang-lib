//
// Created by Jeremy S on 2018-02-02.
//

#pragma once

#include "foundation/__base.hpp"

__LANG_NAMESPACE

    struct less;
    struct greater;
    struct less_equal;
    struct greater_equal;


    struct less {
        typedef greater_equal inverse;

        template<typename T, typename U>
        bool operator()(T const &lhs, U const &rhs) const { return lhs < rhs; }

        template<typename T, typename U>
        static bool compare(T const &lhs, U const &rhs) { return lhs < rhs; };
    };

    struct greater {
        typedef less_equal inverse;

        template<typename T, typename U>
        bool operator()(T const &lhs, U const &rhs) const { return lhs > rhs; }

        template<typename T, typename U>
        static bool compare(T const &lhs, U const &rhs) { return lhs > rhs; };
    };

    struct less_equal {
        typedef greater inverse;

        template<typename T, typename U>
        bool operator()(T const &lhs, U const &rhs) const { return lhs <= rhs; }

        template<typename T, typename U>
        static bool compare(T const &lhs, U const &rhs) { return lhs <= rhs; };
    };

    struct greater_equal {
        typedef less inverse;

        template<typename T, typename U>
        bool operator()(T const &lhs, U const &rhs) const { return lhs >= rhs; }

        template<typename T, typename U>
        static bool compare(T const &lhs, U const &rhs) { return lhs >= rhs; };
    };

__LANG_NAMESPACE_END
