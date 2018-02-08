//
// Created by Jeremy S on 2017-12-27.
//

#pragma once

#include "__base.hpp"

__LANG_SUBSPACE

    template<typename _Tp>
    class __ref_mutable_t {
    public:
        typedef _Tp                     value_type;
        typedef _Tp &                   reference;
        typedef _Tp *                   pointer;
        typedef __ref_mutable_t<_Tp>     __self;

    private:
        pointer __ptr_ = nullptr;

    public:
        __ref_mutable_t() = default;
        __ref_mutable_t(__self const &other);
        explicit __ref_mutable_t(reference __ref);
        explicit __ref_mutable_t(pointer __p);
        ~__ref_mutable_t() = default;
        inline pointer operator->()     { return  __ptr_; }
        inline reference operator*()    { return *__ptr_; }
        inline pointer get() const      { return  __ptr_; }
    };

    template<typename _Tp>
    inline
    __ref_mutable_t<_Tp>::__ref_mutable_t(__self const &other)
            : __ptr_(other.__ptr_)
    {}

    template<typename _Tp>
    inline
    __ref_mutable_t<_Tp>::__ref_mutable_t(reference __ref)
            : __ptr_(&__ref)
    {}

    template<typename _Tp>
    inline
    __ref_mutable_t<_Tp>::__ref_mutable_t(pointer __p)
            : __ptr_(__p)
    {}

    template<typename _Tp>
    class __ref_weak_t {
    public:
        typedef _Tp                     value_type;
        typedef const _Tp &             reference;
        typedef const _Tp *             pointer;
        typedef __ref_weak_t<_Tp>     __self;

    private:
        pointer __ptr_ = nullptr;

    public:
        __ref_weak_t() = default;
        __ref_weak_t(__self const &other);
        explicit __ref_weak_t(reference __ref);
        explicit __ref_weak_t(pointer __p);
        ~__ref_weak_t() = default;
        inline pointer operator->()   { return  __ptr_; }
        inline reference operator*()  { return *__ptr_; }
        inline pointer get() const    { return  __ptr_; }
    };

    template<typename _Tp>
    inline
    __ref_weak_t<_Tp>::__ref_weak_t(__self const &other)
            : __ptr_(other.__ptr_)
    {}

    template<typename _Tp>
    inline
    __ref_weak_t<_Tp>::__ref_weak_t(reference __ref)
            : __ptr_(&__ref)
    {}

    template<typename _Tp>
    inline
    __ref_weak_t<_Tp>::__ref_weak_t(pointer __p)
            : __ptr_(__p)
    {}

__LANG_SUBSPACE_END
