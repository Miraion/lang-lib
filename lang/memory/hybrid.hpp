//
// Created by Jeremy S on 2018-02-02.
//

#pragma once

#include "../foundation/__base.hpp"
#include "heap.hpp"
#include "pool.hpp"

__LANG_MEM_NAMESPACE

    template<typename T, class HeapType = heap<T>, class PoolType = pool<T>>
    class hybrid {
    public:

        ALLOCATOR_TRAITS(T)

        typedef HeapType heap_type;
        typedef PoolType pool_type;

    private:

        typedef singleton<heap_type> _heap;
        typedef singleton<pool_type> _pool;

    public:

        template<typename U>
        struct rebind { typedef hybrid<U> type; };

        hybrid() = default;

        template<typename U>
        hybrid(hybrid<U> const &) {}

        pointer allocate(size_type n, const_pointer /* hint */ = 0) {
            if (n == 1)
                return _pool::instance().allocate(n);
            else
                return _heap::instance().allocate(n);
        }

        void deallocate(pointer p, size_type n) {
            if (n == 1)
                _pool::instance().deallocate(p, n);
            else
                _heap::instance().deallocate(p, n);
        }

        size_type max_size() const { return max_allocations<T>::value; }

    };

__LANG_SUBSPACE_END
