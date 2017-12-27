//
// Created by Jeremy S on 2017-12-19.
//

#pragma once

#include "../foundation/__base.hpp"
#include <iostream>

__LANG_NAMESPACE

    int getc();

    template<class First>
    inline void print(First const &first)
    { std::cout << first; }

    template<class First, class... Rest>
    inline void print(First first, Rest ... rest)
    { std::cout << first; lang::print(rest...); }

    template<class... Args>
    inline void println(Args... args)
    { lang::print(args...); lang::print('\n'); }

__LANG_NAMESPACE_END

#ifdef __LANG_FORCE_INLINE
#include "basic_io.tcc"
#endif
