//
// Created by Jeremy S on 2018-01-14.
//

#pragma once

#include <tuple>
#include <ostream>
#include "../foundation/__base.hpp"
#include "../misc.hpp"
#include "../iterator.hpp"
#include "../memory/allocator.hpp"

#define __LANG_FORWARD_BASIC_SEQ_TYPES(B) \
typedef typename B::value_type      value_type;      \
typedef typename B::pointer         pointer;         \
typedef typename B::const_pointer   const_pointer;   \
typedef typename B::reference       reference;       \
typedef typename B::const_reference const_reference; \
typedef typename B::size_type       size_type;       \

__LANG_NAMESPACE

    template<class _Tp, class _Allocator = lang::allocator<_Tp>>
    class basic_sequence {
    public:

        typedef _Tp                 value_type;
        typedef _Tp *               pointer;
        typedef const _Tp *         const_pointer;
        typedef _Tp &               reference;
        typedef const _Tp &         const_reference;
        typedef _Allocator          allocator_type;
        typedef unsigned long       size_type;

    private:

        typedef basic_sequence<_Tp, _Allocator> self;

    protected:

        pointer __b_;
        pointer __e_;
        std::tuple<pointer, pointer, allocator_type> __alloc_data_;

        inline pointer &alloc_begin() { return std::get<0>(__alloc_data_); }
        inline pointer &alloc_end()   { return std::get<1>(__alloc_data_); }
        inline pointer alloc_begin() const { return std::get<0>(__alloc_data_); }
        inline pointer alloc_end()   const { return std::get<1>(__alloc_data_); }

        allocator_type &alloc();

    public:

        template<class T, class A>
        friend std::ostream &operator<<(std::ostream &os, basic_sequence<T, A> const &seq);

        basic_sequence();

        basic_sequence(self const &other);

        basic_sequence(self &&other) noexcept;

        template<class A>
        basic_sequence(basic_sequence<_Tp, A> const &other);

        template<class A>
        basic_sequence(basic_sequence<_Tp, A> &&seq) noexcept;

        virtual ~basic_sequence();

        /**
         * Wrap Constructor
         *
         * Same as void wrap(pointer, pointer)
         *
         * @param b : pointer to the start of the sequence
         * @param e : pointer to one after the end of the sequence
         */
        basic_sequence(pointer b, pointer e);

        template<class A>
        bool operator==(basic_sequence<_Tp, A> const &other);
        template<class A>
        bool operator!=(basic_sequence<_Tp, A> const &other);

        inline pointer data() const   { return __b_; }

        inline size_type size() const { return __e_ - __b_; }

        size_type capacity() const;

        inline forward_iterator<value_type> begin()
        { return forward_iterator<value_type>{__b_}; }

        inline forward_iterator<value_type> end()
        { return forward_iterator<value_type>{__e_}; }

        inline lang::forward_iterator<value_type, CONST> begin() const
        { return forward_iterator<value_type, CONST>{__b_}; }

        inline lang::forward_iterator<value_type, CONST> end() const
        { return forward_iterator<value_type, CONST>{__e_}; };

        lang::forward_strideable_set<value_type, CONST> forward() const;
        lang::forward_strideable_set<value_type> forward();

        lang::reverse_strideable_set<value_type, CONST> reverse() const;
        lang::reverse_strideable_set<value_type> reverse();

        /**
         * Wraps this sequence around a series of elements.
         *
         * Copies each element to the free store so that the
         * sequence may have full control of de-allocation.
         *
         * @param b : pointer to start of the sequence
         * @param e : pointer to one past the end of the sequence
         */
        void wrap(pointer b, pointer e);

        /**
         * Wraps this sequence around a series of elements.
         *
         * Does not copy the elements to the free store hence
         * the 'unsafe' prefix.
         * If the input pointers are pointers to elements on
         * the stack then the sequence will only valid whilst
         * those elements are still on the stack.
         *
         * @param b : pointer to the start of the sequence
         * @param e : pointer to one past the end of the sequence
         */
        void direct_wrap(pointer b, pointer e);

    };


    template<class _Tp, class _A>
    inline
    typename basic_sequence<_Tp, _A>::allocator_type &
    basic_sequence<_Tp, _A>::alloc()
    { return std::get<2>(__alloc_data_); }

    template<class _Tp, class _A>
    inline
    basic_sequence<_Tp, _A>::basic_sequence()
            : __b_(nullptr),
              __e_(nullptr),
              __alloc_data_(nullptr, nullptr, allocator_type{})
    {}

    template<class _Tp, class _A>
    inline
    basic_sequence<_Tp, _A>::basic_sequence(self const &seq)
            : __b_(nullptr),
              __e_(nullptr),
              __alloc_data_(nullptr, nullptr, allocator_type{})
    {
        alloc_begin() = alloc().allocate(seq.capacity());
        alloc_end() = alloc_end() + seq.capacity();
        __b_ = alloc_begin();
        __e_ = __b_ + seq.size();
        loop until(seq.size()) {
            alloc().construct(__b_ + i, seq.data()[i]);
        }
    }

    template<class _Tp, class _A>
    inline
    basic_sequence<_Tp, _A>::basic_sequence(self &&seq) noexcept
            : __b_(seq.__b_),
              __e_(seq.__e_),
              __alloc_data_(seq.alloc_begin(), seq.alloc_end(), allocator_type{})
    {
        seq.__b_ = nullptr;
        seq.__e_ = nullptr;
        seq.alloc_begin() = nullptr;
        seq.alloc_end() = nullptr;
    }

    template<class _Tp, class _A>
    template<class A>
    inline
    basic_sequence<_Tp, _A>::basic_sequence(basic_sequence<_Tp, A> const &seq)
            : __b_(nullptr),
              __e_(nullptr),
              __alloc_data_(nullptr, nullptr, allocator_type{})
    {
        fflush(stdout);
        alloc_begin() = alloc().allocate(seq.capacity());
        alloc_end() = alloc_end() + seq.capacity();
        __b_ = alloc_begin();
        __e_ = __b_ + seq.size();
        loop until(seq.size()) {
            alloc().construct(__b_ + i, seq.data()[i]);
        }
    }

    template<class _Tp, class _A>
    template<class A>
    inline
    basic_sequence<_Tp, _A>::basic_sequence(basic_sequence<_Tp, A> &&seq) noexcept
            : __b_(seq.__b_),
              __e_(seq.__e_),
              __alloc_data_(seq.alloc_begin(), seq.alloc_end(), allocator_type{})
    {
        fflush(stdout);
        seq.__b_ = nullptr;
        seq.__e_ = nullptr;
        seq.alloc_begin() = nullptr;
        seq.alloc_end() = nullptr;
    }

    template<class _Tp, class _A>
    inline
    basic_sequence<_Tp, _A>::basic_sequence(pointer b, pointer e)
            : __b_(nullptr),
              __e_(nullptr),
              __alloc_data_(nullptr, nullptr, allocator_type{})
    {
        wrap(b, e);
    }

    template<class _Tp, class _A>
    inline
    basic_sequence<_Tp, _A>::~basic_sequence()
    {
        if (alloc_begin()) {
            for (pointer p = __b_; p != __e_; ++p)
                alloc().destroy(p);
        }
        alloc().deallocate(alloc_begin(), alloc_end() - alloc_begin());
        alloc_begin() = nullptr;
        alloc_end() = nullptr;
        __b_ = nullptr;
        __e_ = nullptr;
    }

    template<class _Tp, class _A>
    template<class A>
    inline
    bool
    basic_sequence<_Tp, _A>::operator==(basic_sequence<_Tp, A> const &other)
    {
        if (this->size() != other.size())
            return false;
        loop until(size()) {
            if (__b_[i] != other.__b_[i])
                return false;
        }
        return true;
    }

    template<class _Tp, class _A>
    template<class A>
    inline
    bool
    basic_sequence<_Tp, _A>::operator!=(basic_sequence<_Tp, A> const &other)
    {
        return !((*this) == other);
    }

    template<class _Tp, class _A>
    inline
    typename basic_sequence<_Tp, _A>::size_type
    basic_sequence<_Tp, _A>::capacity() const
    { return alloc_end() - alloc_begin(); }

    template<class _Tp, class _A>
    inline
    lang::forward_strideable_set<typename basic_sequence<_Tp, _A>::value_type, CONST>
    basic_sequence<_Tp, _A>::forward() const
    {
        return lang::forward_strideable_set<value_type, CONST>{__b_, __e_};
    }

    template<class _Tp, class _A>
    inline
    lang::forward_strideable_set<typename basic_sequence<_Tp, _A>::value_type>
    basic_sequence<_Tp, _A>::forward()
    {
        return lang::forward_strideable_set<value_type>{__b_, __e_};
    }

    template<class _Tp, class _A>
    inline
    lang::reverse_strideable_set<typename basic_sequence<_Tp, _A>::value_type, CONST>
    basic_sequence<_Tp, _A>::reverse() const
    {
        return lang::reverse_strideable_set<value_type, CONST>{__b_, __e_};
    }

    template<class _Tp, class _A>
    inline
    lang::reverse_strideable_set<typename basic_sequence<_Tp, _A>::value_type>
    basic_sequence<_Tp, _A>::reverse()
    {
        return lang::reverse_strideable_set<value_type>{__b_, __e_};
    }

    template<class _Tp, class _A>
    inline
    void
    basic_sequence<_Tp, _A>::wrap(pointer b, pointer e)
    {
        __b_ = alloc().allocate(e - b);
        __e_ = __b_ + (e - b);
        alloc_begin() = __b_;
        alloc_end() = __e_;

        for (size_type i = 0; i < (e - b); ++i)
            alloc().construct(__b_ + i, b[i]);
    }

    template<class _Tp, class _A>
    inline
    void
    basic_sequence<_Tp, _A>::direct_wrap(pointer b, pointer e)
    {
        __b_ = b;
        __e_ = e;
    }

__LANG_NAMESPACE_END

// Non-member Functions //
__LANG_NAMESPACE

    template<class _Tp, class _A>
    inline
    std::ostream &
    operator<<(std::ostream &os, basic_sequence<_Tp, _A> const &seq)
    {
        os << '[';
        for (size_t i = 0; i < seq.size(); ++i) {
            os << seq.data()[i];
            if (i != seq.size() - 1)
                os << ", ";
        }
        os << ']';
        return os;
    }

__LANG_NAMESPACE_END
