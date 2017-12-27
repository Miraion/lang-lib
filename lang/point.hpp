//
// Created by Jeremy S on 2017-11-22.
//

#pragma once

#include <cstdlib>
#include <array>
#include <ostream>
#include "foundation/__base.hpp"

namespace lang {

    template<size_t __n, typename _Tp = int>
    class point : public std::array<_Tp, __n> {
    protected:

        typedef point<__n, _Tp>         __self;
        typedef std::array<_Tp, __n>    __super;
        typedef _Tp                     value_type;

    public:

        point () = default;

        point (__self const &other);

        explicit point (__super const &arr);

        template<typename ... _Ts>
        explicit point (_Ts ... v);

        virtual ~point () = default;

        __self &operator= (__super const &arr);

        template<typename ... _Ts>
        __self &operator= (_Ts ... v);

        bool operator== (__self const &other) const;

        bool operator!= (__self const &other) const;

        __self operator- () const;

        __self operator+ (__self const &other) const;

        __self operator- (__self const &other) const;

        __self operator* (value_type const &c) const;

        static __self zero ();

    };

    template<size_t __n, typename _Tp>
    inline point<__n, _Tp>::point(const point::__self &other)
            : point::__super(other)
    {}

    template<size_t __n, typename _Tp>
    inline point<__n, _Tp>::point(const point::__super &arr)
            : point::__super(arr)
    {}

    template<size_t __n, typename _Tp>
    template<typename... _Ts>
    inline point<__n, _Tp>::point(_Ts... v)
            : point::__super()
    {
        value_type as_arr[] = {(value_type)v ...};
        loop until(__n)
            (*this)[i] = as_arr[i];
    }

    template<size_t __n, typename _Tp>
    inline point<__n, _Tp> &point<__n, _Tp>::operator=(const point::__super &arr) {
        loop until(__n)
            (*this)[i] = arr[i];
        return *this;
    }

    template<size_t __n, typename _Tp>
    template<typename... _Ts>
    inline point<__n, _Tp> &point<__n, _Tp>::operator=(_Ts... v) {
        value_type as_arr[] = {(value_type)v ...};
        loop until(__n) {
            (*this)[i] = as_arr[i];
        }
        return *this;
    }

    template<size_t __n, typename _Tp>
    inline bool point<__n, _Tp>::operator==(const point::__self &other) const {
        for (size_t i = 0; i < __n; ++i) {
            if ((*this)[i] != other[i])
                return false;
        }
        return true;
    }

    template<size_t __n, typename _Tp>
    inline bool point<__n, _Tp>::operator!=(const point::__self &other) const {
        return !(*this == other);
    }

    template<size_t __n, typename _Tp>
    inline point<__n, _Tp> point<__n, _Tp>::operator-() const {
        return (*this) * -1;
    }

    template<size_t __n, typename _Tp>
    inline point<__n, _Tp> point<__n, _Tp>::operator+(const point::__self &other) const {
        __self r{};
        loop until(__n)
            r[i] = (*this)[i] + other[i];
        return r;
    }

    template<size_t __n, typename _Tp>
    inline point<__n, _Tp> point<__n, _Tp>::operator-(const point::__self &other) const {
        return (*this) + (-other);
    }

    template<size_t __n, typename _Tp>
    inline point<__n, _Tp> point<__n, _Tp>::operator*(const _Tp &c) const {
        __self r{};
        loop until(__n)
            r[i] = (*this)[i] * c;
        return r;
    }

    template<size_t __n, typename _Tp>
    inline point<__n, _Tp> point<__n, _Tp>::zero() {
        __self r{};
        loop until(__n)
            r[i] = 0;
        return r;
    }

    template<size_t __n, typename _Tp>
    inline std::ostream &operator<< (std::ostream &os, point<__n, _Tp> const &p) {
        os << '(';
        loop until(__n) {
            os << p[i];
            if (i != __n - 1)
                os << ", ";
        }
        os << ')';
        return os;
    }

}
