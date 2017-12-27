//
// Created by Jeremy S on 2017-11-22.
//

#pragma once

/*
        optional.hpp synopsis

        note: no external library linkage required for c++17,
              it is required for c++14 and lower.

 namespace lang
 {

 struct __nil_t{};

 inline const __nil_t nil{}; // since c++17
 extern const __nil_t nil;   // until c++17

 template<class T, class Allocator>
 class optional {
 private:

     typedef T                          value_type;
     typedef Allocator                  allocator_type;
     typedef __nil_t                    _nil;
     typdef optional<T, Allocator>      self;

     allocator_type __alloc{};
     value_type *__ptr_;
     bool __is_nil_;

 public:

     optional();
     optional(self const &);
     optional(self &&) noexcept;
     template<class A>
     explicit optional(optional<T, A> const &);
     template<class A>
     explicit optional(optional<T, A> &&) noexcept;
     optional(value_type const &);
     optional(_nil const &);
     virtual ~optional();

     self &operator=(value_type const &);
     self &operator=(_nil const &);
     self &operator=(self const &);
     template<class A>
     self &operator=(optional<T, A> const &);
     template<class A>
     self &operator=(optional<T, A> &&) noecept;

     bool operator==(_nil const &);
     bool operator!=(_nil const &);
     template<class A>
     bool operator==(optional<T, A> const &);
     template<class A>
     bool operator!=(optional<T, A> const &);

     explicit operator bool() const;

     bool has_value() const;
     value_type value() const;
     value_type *unsafe_raw();

 }; // class optional<T, Allocator>

 template<T, Allocator>
 std::ostream &operator<<(std::ostream &, optional<T, Allocator> const &);

 } // namespace lang

 */

#include <cstdlib>
#include <stdexcept>
#include <ostream>
#include <memory>
#include "foundation/__base.hpp"

__LANG_NAMESPACE

    struct __nil_t {};

#if _LIBCPP_STD_VER > 14
    inline const __nil_t nil{};
#else
    extern const __nil_t nil;
#endif


    template<class _Tp, class _Allocator = std::allocator<_Tp>>
    class optional {
    private:

        typedef _Tp                         value_type;
        typedef _Allocator                  allocator_type;
        typedef __nil_t                     _nil;
        typedef optional<_Tp, _Allocator>   __self;

        allocator_type __alloc{};
        value_type *__ptr_;
        bool __is_nil_;

    public:

        optional ();

        optional (__self const &other);

        optional (__self &&other) noexcept;

        template<class A>
        explicit optional (optional<value_type, A> const &other);

        template<class A>
        explicit optional (optional<value_type, A> &&other) noexcept;

        optional (value_type const &x);

        optional (_nil const &n);

        virtual ~optional ();

        __self &operator= (value_type const &x);

        __self &operator= (_nil const &n);

        __self &operator= (__self const &other);

        template<class A>
        __self &operator= (optional<value_type, A> const &other);

        template<class A>
        __self &operator= (optional<value_type, A> &&other) noexcept;

        bool operator== (_nil const &n);

        bool operator!= (_nil const &n);

        template<class A>
        bool operator== (optional<value_type, A> const &other);

        template<class A>
        bool operator!= (optional<value_type, A> const &other);

        explicit operator bool() const { return has_value(); }

        bool has_value () const;

        value_type value () const;

        value_type *unsafe_raw ();

    };


    template<class _Tp, class _Allocator>
    inline
    optional<_Tp, _Allocator>::optional()
            : __ptr_(__alloc.allocate(1)),
              __is_nil_(true)
    {}

    template<class _Tp, class _Allocator>
    template<class A>
    inline
    optional<_Tp, _Allocator>::optional(optional<_Tp, A> const &other)
            : __ptr_(__alloc.allocate(1)),
              __is_nil_(other.__is_nil_)
    {
        if (other.has_value())
            __alloc.construct(__ptr_, other.value());
    }

    template<class _Tp, class _Allocator>
    inline
    optional<_Tp, _Allocator>::optional(optional<_Tp, _Allocator> const &other)
            : __ptr_(__alloc.allocate(1)),
              __is_nil_(other.__is_nil_)
    {
        if (other.has_value())
            __alloc.construct(__ptr_, other.value());
    }

    template<class _Tp, class _Allocator>
    template<class A>
    inline
    optional<_Tp, _Allocator>::optional(optional<_Tp, A> &&other) noexcept
            : __ptr_(other.__ptr_),
              __is_nil_(other.__is_nil_)
    {
        other.__ptr_ = nullptr;
    }

    template<class _Tp, class _Allocator>
    inline
    optional<_Tp, _Allocator>::optional
            (optional<value_type, allocator_type> &&other) noexcept
            : __ptr_(other.__ptr_),
              __is_nil_(other.__is_nil_)
    {
        other.__ptr_ = nullptr;
    }

    template<class _Tp, class _Allocator>
    inline
    optional<_Tp, _Allocator>::optional(_Tp const &x)
            : __ptr_(__alloc.allocate(1)),
              __is_nil_(false)
    {
        __alloc.construct(__ptr_, x);
    }

    template<class _Tp, class _Allocator>
    inline
    optional<_Tp, _Allocator>::optional(optional::_nil const &n)
            : __ptr_(__alloc.allocate(1)),
              __is_nil_(true)
    {}

    template<class _Tp, class _Allocator>
    inline
    optional<_Tp, _Allocator>::~optional()
    {
        if (__ptr_ != nullptr) {
            __alloc.destroy(__ptr_);
            __alloc.deallocate(__ptr_, 1);
        }
    }

    template<class _Tp, class _Allocator>
    inline
    optional<_Tp, _Allocator> &
    optional<_Tp, _Allocator>::operator=(const _Tp &x)
    {
        __alloc.destroy(__ptr_);
        __alloc.construct(__ptr_, x);
        __is_nil_ = false;
        return *this;
    }

    template<class _Tp, class _Allocator>
    inline
    optional<_Tp, _Allocator> &
    optional<_Tp, _Allocator>::operator=(optional::_nil const &n)
    {
        __is_nil_ = true;
        return *this;
    }

    template<class _Tp, class _Allocator>
    template<class A>
    inline
    optional<_Tp, _Allocator> &
    optional<_Tp, _Allocator>::operator=(optional<_Tp, A> const &other)
    {
        if (this != &other) {
            if (other.has_value()) {
                __alloc.destroy(__ptr_);
                __alloc.construct(__ptr_, *other.__ptr_);
                __is_nil_ = false;
            } else {
                __is_nil_ = true;
            }
        }
        return *this;
    }

    template<class _Tp, class _Allocator>
    inline
    optional<_Tp, _Allocator> &
    optional<_Tp, _Allocator>::operator=(optional<_Tp, _Allocator> const &other)
    {
        if (this != &other) {
            if (other.has_value()) {
                __alloc.destroy(__ptr_);
                __alloc.construct(__ptr_, *other.__ptr_);
                __is_nil_ = false;
            } else {
                __is_nil_ = true;
            }
        }
        return *this;
    }

    template<class _Tp, class _Allocator>
    template<class A>
    inline
    optional<_Tp, _Allocator> &
    optional<_Tp, _Allocator>::operator=(optional<_Tp, A> &&other) noexcept
    {
        __alloc.destroy();
        __alloc.deallocate();
        __ptr_ = other.__ptr_;
        other.__ptr_ = nullptr;
        return *this;
    }

    template<class _Tp, class _Allocator>
    inline
    bool
    optional<_Tp, _Allocator>::operator==(optional::_nil const &n)
    {
        return !this->has_value();
    }

    template<class _Tp, class _Allocator>
    inline
    bool
    optional<_Tp, _Allocator>::operator!=(optional::_nil const &n)
    {
        return !(*this == n);
    }

    template<class _Tp, class _Allocator>
    template<class A>
    inline
    bool
    optional<_Tp, _Allocator>::operator==(optional<_Tp, A> const &other)
    {
        if (!this->has_value())
            return !other.has_value();
        if (!other.has_value())
            return !this->has_value();
        return this->value() == other.has_value();
    }

    template<class _Tp, class _Allocator>
    template<class A>
    inline
    bool
    optional<_Tp, _Allocator>::operator!=(optional<_Tp, A> const &other)
    {
        return !(*this == other);
    }

    template<class _Tp, class _Allocator>
    inline
    bool
    optional<_Tp, _Allocator>::has_value() const
    {
        return !__is_nil_;
    }

    template<class _Tp, class _Allocator>
    inline
    _Tp
    optional<_Tp, _Allocator>::value() const
    {
        if (!has_value())
            throw std::runtime_error{"found nil when unwrapping optional"};
        return *__ptr_;
    }

    template<class _Tp, class _Allocator>
    inline
    _Tp *
    optional<_Tp, _Allocator>::unsafe_raw()
    {
        return __ptr_;
    }

    template<class _Tp, class _Allocator>
    inline
    std::ostream &
    operator<< (std::ostream &os, lang::optional<_Tp, _Allocator> const &o)
    {
        if (o)
            os << o.value();
        else
            os << "nil";
        return os;
    }

__LANG_NAMESPACE_END

using lang::nil;
