//
// Created by Jeremy S on 2018-01-22.
//

#pragma once

#include "allocator_traits.hpp"
#include "../stack.hpp"

__LANG_NAMESPACE

    template<typename T, size_t N = 128>
    class static_pool {
    public:

        ALLOCATOR_TRAITS(T)

        template<typename U>
        struct rebind {
            typedef static_pool<U> type;
        };

        static_pool();

        template<typename U>
        static_pool(static_pool<U, N> const &other) {}

        pointer allocate(size_type n, const_pointer /* hint */ = nullptr);

        void deallocate(pointer p, size_type);

        size_type max_size() const { return _free.size(); }

    private:

        lang::stack<pointer> _free;

    };


    template<typename T, size_t N>
    inline static_pool<T, N>::static_pool()
            : _free()
    {
        auto _p = static_cast<pointer>(::operator new(N * sizeof(T), ::std::nothrow));
        loop until(N) {
            _free.push_back(_p);
            ++_p;
        }
    }

    template<typename T, size_t N>
    inline
    typename static_pool<T, N>::pointer
    static_pool<T, N>::allocate(size_type n, const_pointer)
    {
        if (n > 1)
            throw std::runtime_error("multiple allocations are not allowed");
        if (max_size() == 0)
            throw std::runtime_error("unable to allocate, pool is empty");
        return _free.pop_back();
    }

    template<typename T, size_t N>
    inline
    void
    static_pool<T, N>::deallocate(pointer p, size_type)
    {
        _free.push_back(p);
    }

__LANG_NAMESPACE_END
