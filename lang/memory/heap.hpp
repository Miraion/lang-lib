//
// Created by Jeremy S on 2018-01-16.
//

#pragma once

#include "allocator_traits.hpp"

__LANG_MEM_NAMESPACE

    template<typename T>
    class heap {
    public:

        ALLOCATOR_TRAITS(T)

        template<typename U>
        struct rebind { typedef heap<U> type; };

        heap() = default;

        template<typename U>
        heap(heap<U> const &other) {}

        pointer allocate(size_type n, const_pointer /* hint */ = 0) {
            if (n > max_size())
                throw std::bad_alloc();
            return static_cast<pointer>(::operator new(n * sizeof(type), ::std::nothrow));
        }

        void deallocate(pointer p, size_type /* count */) {
            ::operator delete(p);
        }

        size_type max_size() const { return max_allocations<T>::value; }

    };

__LANG_SUBSPACE_END
