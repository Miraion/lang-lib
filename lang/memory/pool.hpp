//
// Created by Jeremy S on 2018-01-29.
//

#pragma once

#include "allocator_traits.hpp"
#include "../stack.hpp"
#include "../singleton.hpp"

__LANG_MEM_NAMESPACE

    template<typename T>
    class pool {
    public:

        ALLOCATOR_TRAITS(T)

        template<typename U>
        struct rebind {
            typedef pool<U> type;
        };

        pool();

        template<typename U>
        pool(pool<U> const &other) {};

        pointer allocate(size_type n, const_pointer /* hint */ = nullptr);

        void deallocate(pointer p, size_type);

        size_type max_size() const { return max_allocations<type>::value; }

    private:

        struct _chunk { lang::stack<pointer> _free; };

        pointer allocate_chunk();

        static const size_type chunk_size = 2;

        typedef lang::singleton<_chunk> _pool;

    };


    template<typename T>
    inline
    pool<T>::pool()
    {
        _pool::init();
        pointer p = allocate_chunk();
        loop until(chunk_size) {
            _pool::instance()._free.push_back(p);
            ++p;
        }
    }

    template<typename T>
    inline
    typename pool<T>::pointer
    pool<T>::allocate(size_type n, const_pointer)
    {
        if (n > 1)
            throw std::runtime_error("pool may only preform singular allocations");
        if (_pool::instance()._free.empty()) {
            pointer p = allocate_chunk();
            loop until(chunk_size) {
                _pool::instance()._free.push_back(p);
                ++p;
            }
        }
        return _pool::instance()._free.pop_back();
    }

    template<typename T>
    inline
    void
    pool<T>::deallocate(pointer p, size_type)
    {
        _pool::instance()._free.push_back(p);
    }

    template<typename T>
    inline
    typename pool<T>::pointer
    pool<T>::allocate_chunk()
    {
        return static_cast<pointer>(::operator new(chunk_size * sizeof(T), ::std::nothrow));
    }

__LANG_SUBSPACE_END
