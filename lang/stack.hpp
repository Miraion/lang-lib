//
// Created by Jeremy S on 2017-12-10.
//

#pragma once

#include "foundation/__dym_array.hpp"

__LANG_NAMESPACE

    template<class _Tp, template<class> class _Allocator = std::allocator>
    class stack : protected __1::__dym_array<_Tp, _Allocator> {
    private:

        typedef __1::__dym_array<_Tp, _Allocator> __base;

    public:

        using __base::__dym_array;
        using __base::push_back;
        using __base::emplace_back;
        using __base::pop_back;
        using __base::extract_back;
        using __base::begin;
        using __base::end;
        using __base::snapshot;
        using __base::shrink_to_fit;
        using __base::back;
        using __base::size;
        using __base::capacity;
        using __base::physical_size;
        using __base::alloc_size;
        using __base::empty;
        using __base::erase;

        template<class T, template<class> class A>
        friend std::ostream &operator<<(std::ostream &os, stack<T, A> const &q) {
            os << dynamic_cast<__1::__dym_array<T, A> const &>(q);
            return os;
        }

#ifdef __LANG_STD_17
        using __base::last;
#endif

    };

__LANG_NAMESPACE_END
