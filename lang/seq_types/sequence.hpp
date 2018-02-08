//
// Created by Jeremy S on 2018-01-17.
//

#pragma once

#include "basic_sequence.hpp"

__LANG_NAMESPACE

    template<class T, class A = lang::allocator<T>>
    class sequence : public basic_sequence<T, A> {
    private:

        typedef basic_sequence<T, A> base;
        typedef sequence<T, A>       self;

    protected:

        using base::alloc_begin;
        using base::alloc_end;
        using base::alloc;
        using base::__b_;
        using base::__e_;

    public:

        __LANG_FORWARD_BASIC_SEQ_TYPES(base)

        using base::size;
        using base::capacity;

        // Default Constructor
        sequence() = default;

        // Copy Constructor
        sequence(basic_sequence<T, A> const &s);

        // Move Constructor
        sequence(base &&s) noexcept;

        /**
         * Construct a sequence of a given size.
         *
         * This allocates n elements worth of data and constructs
         * n elements using a given reference object to copy.
         *
         * @param n : the number of elements for the sequence
         */
        template<typename I>
        explicit sequence(I n, value_type v = value_type{});

        template<typename... Ts>
        explicit sequence(Ts... elems);

        self &operator=(base const &s);
        self &operator=(base &&s) noexcept;

        /**
         * Sequence Subscript
         *
         * Does not preform bound checking.
         *
         * @tparam I : Must be castable to size_type (i.e. unsigned long)
         * @param i  : Index
         * @return   : Value at a given index
         */
        template<typename I>
        reference operator[](I i);
        template<typename I>
        const_reference operator[](I i) const;

        /**
         * Same as operator[] but preforms bound checking.
         *
         * Throws std::out_of_range if index is out of range.
         */
        template<typename I>
        reference at(I i);
        template<typename I>
        const_reference  at(I i) const;

        /**
         * First and last element access.
         *
         * Throws std::out_of_range if size() == 0.
         */
        inline reference        front()       { return at(0); }
        inline const_reference  front() const { return at(0); }
        inline reference        back()        { return at(size() - 1); }
        inline const_reference  back()  const { return at(size() - 1); }

        void assert_bounds(size_type i) const;

        template<typename U = T>
        typename std::enable_if<std::is_same<U, char>::value, std::string>::type
        as_string() const;

    };


    template<class T, class A>
    inline
    sequence<T, A>::sequence(base const &s)
            : base(s)
    {}

    template<class T, class A>
    inline
    sequence<T, A>::sequence(base &&s) noexcept
            : base(s)
    {}

    template<class T, class A>
    template<typename I>
    inline
    sequence<T, A>::sequence(I n, value_type v)
            : base()
    {
        alloc_begin() = alloc().allocate(n);
        alloc_end() = alloc_begin() + n;
        __b_ = alloc_begin();
        __e_ = alloc_end();
        loop until(size())
            alloc().construct(__b_ + i, v);
    }

    template<class T, class A>
    template<typename... Ts>
    inline
    sequence<T, A>::sequence(Ts... elems)
            : base()
    {
        alloc_begin() = alloc().allocate(sizeof...(elems));
        alloc_end() = alloc_begin() + sizeof...(elems);
        __b_ = alloc_begin();
        __e_ = alloc_end();
        T arr[] = {std::forward<Ts>(elems)...};
        loop until(size())
            alloc().construct(__b_ + i, std::move(arr[i]));
    }

    template<class T, class A>
    inline
    typename sequence<T, A>::self &
    sequence<T, A>::operator=(base const &s)
    {
        alloc_begin() = alloc().allocate(s.capacity());
        alloc_end() = alloc_begin() + s.capacity();
        __b_ = alloc_begin() + (s.__b_ - s.alloc_begin());
        __e_ = __b_ + s.size();
        loop until(size()) {
            alloc().construct(__b_ + i, s.__b_[i]);
        }
        return *this;
    }

    template<class T, class A>
    inline
    typename sequence<T, A>::self &
    sequence<T, A>::operator=(base &&s) noexcept
    {
        this->alloc_begin() = s.alloc_begin();
        this->alloc_end() = s.alloc_end();
        this->__b_ = s.__b_;
        this->__e_ = s.__e_;

        s.alloc_begin() = nullptr;
        s.alloc_end() = nullptr;
        s.__b_ = nullptr;
        s.__e_ = nullptr;
        return *this;
    }

    template<class T, class A>
    template<typename I>
    inline
    typename sequence<T, A>::reference
    sequence<T, A>::operator[](I i)
    {
        return __b_[static_cast<size_type>(i)];
    }

    template<class T, class A>
    template<typename I>
    inline
    typename sequence<T, A>::const_reference
    sequence<T, A>::operator[](I i) const
    {
        return __b_[static_cast<size_type>(i)];
    }

    template<class T, class A>
    template<typename I>
    inline
    typename sequence<T, A>::reference
    sequence<T, A>::at(I i)
    {
        if (i >= size())
            throw std::out_of_range("index out of range");
        return (*this)[i];
    }

    template<class T, class A>
    template<typename I>
    inline
    typename sequence<T, A>::const_reference
    sequence<T, A>::at(I i) const
    {
        if (i >= size())
            throw std::out_of_range("index out of range");
        return (*this)[i];
    }

    template<class T, class A>
    inline
    void
    sequence<T, A>::assert_bounds(size_type i) const
    {
        if (i >= size())
            throw std::out_of_range("index out of range");
    }

    template<typename T, class A>
    template<typename U>
    inline
    typename std::enable_if<std::is_same<U, char>::value, std::string>::type
    sequence<T, A>::as_string() const
    {
        char str[size() + 1];
        copy_memory(str, __b_, size());
        str[size()] = '\0';
        return std::string{(const char *) str};
    }

__LANG_NAMESPACE_END
