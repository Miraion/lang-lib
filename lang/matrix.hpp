//
// Created by Jeremy S on 2017-11-24.
//

#pragma once

#include "foundation/__base.hpp"
#include "point.hpp"

namespace lang {


    template<size_t __m, size_t __n, typename _Tp = double>
    class matrix {
    protected:

        typedef _Tp                     value_type;
        typedef matrix<__m, __n, _Tp>   __self;

        value_type *__arr_;

        const size_t __size_ = __m * __n;

    private:

        value_type *__alloc () {
            printf("alloc: %ld", __size_);
            fflush(stdout);
            return new value_type[__size_];
        }

    public:

        using column_t = point<__m, _Tp>;

        using row_t = point<__n, _Tp>;

        matrix ()
                : __arr_(new value_type[__m * __n])
        {
            loop until(__size_)
                  __arr_[i] = value_type{};
        }

        matrix (matrix<__m, __n, _Tp> const &other)
                : __arr_(new value_type[__m * __n])
        {
            loop until(__size_)
                  this->__arr_[i] = other.__arr_[i];
        }

        matrix (matrix<__m, __n, _Tp>&& other) noexcept
                : __arr_(other.__arr_)
        {
            other.__arr_ = nullptr;
        }

        template<typename ... _Ts>
        explicit matrix (_Ts ... ts)
                : __arr_(new value_type[__m * __n])
        {
            value_type vs[] = {(value_type)ts ...};
            loop until(__size_)
                  __arr_[i] = vs[i];
        }

        virtual ~matrix () {
            delete[] __arr_;
        }

        const value_type &at (size_t i, size_t j) const {
            return __arr_[(i * __n) + j];
        }

        value_type &at (size_t i, size_t j) {
            return __arr_[(i * __n) + j];
        }

        row_t row (size_t i) const {
            if (i >= __m)
                throw std::out_of_range{"matrix::row: index out of range"};

            row_t r{};
            loop_until(j, __n)
                r[j] = this->at(i, j);
            return r;
        }

        column_t column (size_t j) const {
            if (j >= __n)
                throw std::out_of_range{"matrix::column: index out of range"};

            column_t c{};
            loop_until(i, __m)
                c[i] = this->at(i, j);
            return c;
        }

        size_t size () const {
            return __size_;
        }

        const value_type *begin () const {
            return __arr_;
        }

        value_type  *begin () {
            return __arr_;
        }

        const value_type *end () const {
            return __arr_ + __size_;
        }

        value_type  *end () {
            return __arr_ + __size_;
        }

    };

}
