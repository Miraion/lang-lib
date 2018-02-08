//
// Created by Jeremy S on 2018-01-11.
//

#pragma once

#include "foundation/__base.hpp"
#include "operand.hpp"
#include "misc.hpp"

__LANG_SUBSPACE

    template<class _Tp, bool _Const>
    class basic_iterator
            : public comparable<typename std::conditional<_Const, const _Tp *, _Tp *>::type>
    {
    public:

        typedef _Tp         value_type;
        typedef const _Tp * const_pointer;
        typedef typename std::conditional<_Const, const _Tp *, _Tp *>::type pointer;
        typedef typename std::conditional<_Const, const _Tp &, _Tp &>::type reference;

    protected:

        typedef basic_iterator<_Tp, _Const> __basic_self;

        pointer __ptr_;

    public:

        basic_iterator();
        basic_iterator(__basic_self const &__a);
        explicit basic_iterator(pointer __p);

        __basic_self &operator=(__basic_self const &__a);
        __basic_self &operator=(pointer __p);

        // Implicit cast to pointer type (either const T* or T*)
        inline operator pointer() { return __ptr_; }

        bool operator==(pointer const &__a) const override;
        bool operator<(pointer const &__a) const override;

        inline pointer operator->() { return __ptr_; }
        inline reference operator*() { return *__ptr_; }

        __basic_self &operator++();

        virtual void advance() = 0;
        inline size_t distance(const_pointer __p) const { return lang::abs(__p - __ptr_); }

    };


    template<class _Tp, bool _Const>
    inline
    basic_iterator<_Tp, _Const>::basic_iterator()
            : __ptr_(nullptr)
    {}

    template<class _Tp, bool _Const>
    inline
    basic_iterator<_Tp, _Const>::basic_iterator(__basic_self const &__a)
            : __ptr_(__a.__ptr_)
    {}

    template<class _Tp, bool _Const>
    inline
    basic_iterator<_Tp, _Const>::basic_iterator(pointer __p)
            : __ptr_(__p)
    {}

    template<class _Tp, bool _Const>
    inline
    typename basic_iterator<_Tp, _Const>::__basic_self &
    basic_iterator<_Tp, _Const>::operator=(__basic_self const &__a)
    {
        this->__ptr_ = __a.__ptr_;
        return *this;
    }

    template<class _Tp, bool _Const>
    inline
    typename basic_iterator<_Tp, _Const>::__basic_self &
    basic_iterator<_Tp, _Const>::operator=(pointer __p)
    {
        this->__ptr_ = __p;
        return *this;
    }

    template<class _Tp, bool _Const>
    inline
    bool
    basic_iterator<_Tp, _Const>::operator==(pointer const &__a) const
    {
        return __ptr_ == __a;
    }

    template<class _Tp, bool _Const>
    inline
    bool
    basic_iterator<_Tp, _Const>::operator<(pointer const &__a) const
    {
        return __ptr_ < __a;
    }

    template<class _Tp, bool _Const>
    inline
    typename basic_iterator<_Tp, _Const>::__basic_self &
    basic_iterator<_Tp, _Const>::operator++()
    {
        advance();
        return *this;
    }


    template<class _Tp, bool _Const, template<class, bool> class _I>
    class strideable_set {
    public:

        typedef _I<_Tp, _Const> iterator_type;
        typedef typename iterator_type::pointer pointer;

    private:

        iterator_type __begin_;
        iterator_type __end_;

    public:

        strideable_set();
        strideable_set(strideable_set<_Tp, _Const, _I> const &__a) = default;
        strideable_set(pointer __b, pointer __e);

        inline iterator_type begin() const { return __begin_; }
        inline iterator_type end() const { return __end_; }

        inline size_t size() const { return __begin_.distance(__end_); }

    };

    template<class _Tp, bool _Const, template<class, bool> class _I>
    inline
    strideable_set<_Tp, _Const, _I>::strideable_set()
            : __begin_(nullptr), __end_(nullptr)
    {}

    template<class _Tp, bool _Const, template<class, bool> class _I>
    inline
    strideable_set<_Tp, _Const, _I>::strideable_set(pointer __b, pointer __e)
            : __begin_(__b), __end_(__e)
    {}

__LANG_SUBSPACE_END

#define CONST true

__LANG_NAMESPACE

    template<class _Tp, bool _Const = false>
    class forward_iterator : public __1::basic_iterator<_Tp, _Const> {
    private:

        typedef __1::basic_iterator<_Tp, _Const> base;

    public:
        typedef typename base::pointer           pointer;

        forward_iterator() = default;
        explicit forward_iterator(pointer __p) : base(__p) {}

        using base::operator=;
        using base::operator++;

        inline void advance() override
        { ++this->__ptr_; }

        forward_iterator<_Tp, _Const> operator++(int);

    };

    template<class _Tp, bool _Const>
    inline
    forward_iterator<_Tp, _Const>
    forward_iterator<_Tp, _Const>::operator++(int)
    {
        forward_iterator<_Tp, _Const> result{*this};
        ++(*this);
        return result;
    }


    template<class _Tp, bool _Const = false>
    class reverse_iterator : public __1::basic_iterator<_Tp, _Const> {
    private:

        typedef __1::basic_iterator<_Tp, _Const> base;

    public:
        typedef typename base::pointer          pointer;

        reverse_iterator() = default;
        explicit reverse_iterator(pointer __p) : base(__p) {}

        using base::operator=;
        using base::operator++;

        inline void advance() override
        { --this->__ptr_; }

        reverse_iterator<_Tp, _Const> operator++(int);

    };

    template<class _Tp, bool _Const>
    inline
    reverse_iterator<_Tp, _Const>
    reverse_iterator<_Tp, _Const>::operator++(int)
    {
        reverse_iterator<_Tp, _Const> result{*this};
        ++(*this);
        return result;
    }


    template<class _Tp, bool _Const = false>
    using forward_strideable_set = __1::strideable_set<_Tp, _Const, forward_iterator>;

    template<class _Tp, bool _Const = false>
    using reverse_strideable_set = __1::strideable_set<_Tp, _Const, reverse_iterator>;

__LANG_NAMESPACE_END
