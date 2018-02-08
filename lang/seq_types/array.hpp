//
// Created by Jeremy S on 2018-01-17.
//

#pragma once

#include "sequence.hpp"

__LANG_NAMESPACE

    template<class T, size_t N, class A = lang::allocator<T>>
    class array : public sequence<T, A> {
    private:

        typedef basic_sequence<T, A> root;
        typedef sequence<T, A>       base;
        typedef array<T, N, A>       self;

    protected:

        using base::__b_;
        using base::__e_;
        using base::alloc_begin;
        using base::alloc_end;
        using base::alloc;

    public:

        array() : base(N) {};

        array(self const &arr) : base(arr) {};

        array(self &&arr) noexcept : base(arr) {};

        template<typename... Ts>
        explicit array(Ts... elems);

        self &operator=(self const &other);
        self &operator=(self &&other) noexcept;

    };

    template<class T, size_t N, class A>
    template<typename... Ts>
    inline
    array<T, N, A>::array(Ts... elems)
            : base(N)
    {
        if (sizeof...(elems) > N)
            throw std::out_of_range("too many input parameters");
        T arr[] = {std::forward<Ts>(elems)...};
        loop until(sizeof...(elems)) {
            (*this)[i] = arr[i];
        }
    }

    template<class T, size_t N, class A>
    inline
    typename array<T, N, A>::self &
    array<T, N, A>::operator=(self const &other)
    {
        loop until(N) {
            alloc().destroy(__b_ + i);
            alloc().construct(__b_ + i, other[i]);
        }
        return *this;
    }

    template<class T, size_t N, class A>
    inline
    typename array<T, N, A>::self &
    array<T, N, A>::operator=(self &&other) noexcept
    {
        this->alloc_begin() = other.alloc_begin();
        this->alloc_end() = other.alloc_end();
        this->__b_ = other.__b_;
        this->__e_ = other.__e_;
        other.alloc_begin() = nullptr;
        other.alloc_end() = nullptr;
        other.__b_ = nullptr;
        other.__e_ = nullptr;
        return *this;
    }

__LANG_NAMESPACE_END
