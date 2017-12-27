//
// Created by Jeremy S on 2017-12-19.
//

#pragma once

#include "../foundation/__base.hpp"
#include <iostream>

__LANG_SUBSPACE

    template<class First>
    inline void __print(First const &first)
    { std::cout << first; }

    template<class First, class... Rest>
    inline void __print(First first, Rest ... rest)
    { std::cout << first << " "; lang::__1::__print(rest...); }

__LANG_SUBSPACE_END

__LANG_NAMESPACE

    int getc();

    // Space separated convenience printing to stdout using std::cout.
    template<class... Args>
    inline void print(Args... args)
    { lang::__1::__print(args...); }

    // Same as above but will print a '\n' after.
    template<class... Args>
    inline void println(Args... args)
    { lang::print(args...); lang::print('\n'); }

__LANG_NAMESPACE_END

#ifdef __LANG_FORCE_INLINE
#include "basic_io.tcc"
#endif
