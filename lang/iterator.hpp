//
// Created by Jeremy S on 2017-12-15.
//

#pragma once

/*

        iterator.hpp synopsis

 namespace lang::__1
 {

 template<class T, bool Const>
 class __basic_iterator {
 public:

     typedef T                                                      value_type;
     typedef typename std::conditional<Const, const T*, T*>::type   pointer;
     typedef typename std::conditional<Const, const T&, T&>::type   reference;

 protected:

     typedef __basic_iterator<T, Const>                             basic_self;

     pointer __ptr_;

 public:

    __basic_iterator(basic_self const &) = default;
    explicit __basic_iterator(value_type *);

    template<bool _Const>
    bool operator==(__basic_iterator<T, _Const> const &other) const;
    template<bool _Const>
    bool operator!=(__basic_iterator<T, _Const> const &other) const;

    pointer operator->();
    reference operator*();
    pointer raw() const;
    template<bool _Const>
    size_t distance(__basic_iterator<T, _Const> const &other) const;

    template<class _T, class _C>
    friend std::ostream &operator<<(std::ostream &, __basic_iterator<_T, _C> const &);

 }; // class __basic_iterator<T, Const>


 template<class Iterator>
 class __basic_strideable_set {
 public:

     typedef Iterator                           iterator_type;
     typedef typename iterator_type::value_type value_type;

 private:

     iterator_type __begin_;
     iterator_type __end_;

 public:

     __basic_strideable_set(iterator_type const &, iterator_type const &);
     __basic_strideable_set(value_type *, value_type *);

     iterator_type begin();
     iterator_type end();

     size_t size() const;

     typename iterator_type::reference operator[](size_t);

 }; // class __basic_strideable_set<Iterator>


 template<class T, bool Const>
 class __forward_iterator : public __basic_iterator<T, Const> {
 private:

     typedef typename __basic_iterator<T, Const>::pointer   pointer
     typedef __forward_iterator<T, Const>                   self

 public:

     explicit __forward_iterator(T *);

     void advance();
     self &operator++();
     self operator++(int);
     self opeartor+(size_t) const;
     self operator-(size_t) const;

 };

 template<class T, bool Const>
 class __reverse_iterator : public __basic_iterator<T, Const> {
 private:

     typedef typename __basic_iterator<T, Const>::pointer   pointer
     typedef __reverse_iterator<T, Const>                   self

 public:

     explicit __reverse_iterator(T *);

     void advance();
     self &operator++();
     self operator++(int);
     self opeartor+(size_t) const;
     self operator-(size_t) const;

 };

 } // namespace lang::__1


 namespace lang
 {

 template<class T>
 using forward_iterator = __1::__forward_iterator<T, false>;

 template<class _Tp>
 using reverse_iterator = __1::__reverse_iterator<_Tp, false>;

 template<class _Tp>
 using const_forward_iterator = __1::__forward_iterator<_Tp, true>;

 template<class _Tp>
 using const_reverse_iterator = __1::__reverse_iterator<_Tp, true>;

 template<class _Tp>
 using forward_strideable_set = __1::__basic_strideable_set<forward_iterator<_Tp>>;

 template<class _Tp>
 using reverse_strideable_set = __1::__basic_strideable_set<reverse_iterator<_Tp>>;

 template<class _Tp>
 using const_forward_strideable_set = __1::__basic_strideable_set<const_forward_iterator<_Tp>>;

 template<class _Tp>
 using const_reverse_strideable_set = __1::__basic_strideable_set<const_reverse_iterator<_Tp>>;

 }

 */

#include "foundation/__base.hpp"
#include <ostream>

__LANG_SUBSPACE

    template<class _Tp, bool _Const>
    class __basic_iterator {
    public:

        typedef _Tp                                                         value_type;
        typedef typename std::conditional<_Const, const _Tp *, _Tp *>::type pointer;
        typedef typename std::conditional<_Const, const _Tp &, _Tp &>::type reference;

    protected:

        typedef __basic_iterator<_Tp, _Const>                               __basic_self;

        pointer __ptr_;

    public:

        __basic_iterator(__basic_self const &other) = default;

//        explicit __basic_iterator(value_type *__p);

        explicit __basic_iterator(pointer __p);

        template<bool ConstO>
        bool operator==(__basic_iterator<_Tp, ConstO> const &other) const
        { return __ptr_ == other.__ptr_; };

        template<bool ConstO>
        bool operator!=(__basic_iterator<_Tp, ConstO> const &other) const
        { return !((*this) == other); };

        inline pointer operator->() { return __ptr_; }

        inline reference operator*() { return *__ptr_; }

        inline pointer raw() const { return __ptr_; }

        template<bool ConstO>
        inline size_t distance(__basic_iterator<_Tp, ConstO> const &other) const
        { return static_cast<size_t>(lang::abs(other.raw() - raw())); }

        template<class _T, bool _C>
        friend std::ostream &operator<<(std::ostream &os, __basic_iterator<_T, _C> const &it);

    };

//    template<class _Tp, bool _Const>
//    inline
//    __basic_iterator<_Tp, _Const>::__basic_iterator(value_type *__p)
//            : __ptr_(__p)
//    {}

    template<class _Tp, bool _Const>
    inline
    __basic_iterator<_Tp, _Const>::__basic_iterator(pointer __p)
            : __ptr_(__p)
    {}

    template<class _T, bool _C>
    inline
    std::ostream &
    operator<< (std::ostream &os, __basic_iterator<_T, _C> const &it)
    {
        os << it.__ptr_;
        return os;
    }


    template<class _Iterator>
    class __basic_strideable_set {
    public:

        typedef _Iterator                           iterator_type;
        typedef typename iterator_type::value_type  value_type;

    private:

        iterator_type __begin_;
        iterator_type __end_;

    public:

        __basic_strideable_set(iterator_type __b, iterator_type __e);

        __basic_strideable_set(value_type *__b, value_type *__e);

        inline iterator_type begin() { return __begin_; }

        inline iterator_type end() { return __end_; }

        inline size_t size() const { return __begin_.distance(__end_); }

        inline typename iterator_type::reference operator[](size_t __i)
        { return *(begin() + __i); }

    };

    template<class _Iterator>
    inline
    __basic_strideable_set<_Iterator>::__basic_strideable_set(iterator_type __b, iterator_type __e)
            : __begin_(__b), __end_(__e)
    {}

    template<class _Iterator>
    inline
    __basic_strideable_set<_Iterator>::__basic_strideable_set (value_type *__b, value_type * __e)
            : __begin_(__b), __end_(__e)
    {}


    template<class _Tp, bool _Const>
    class __forward_iterator : public __basic_iterator<_Tp, _Const> {
    private:

        typedef typename __basic_iterator<_Tp, _Const>::pointer         pointer;
        typedef __forward_iterator<_Tp, _Const>                         __self;

    public:

        explicit __forward_iterator(_Tp *__p)
                : __1::__basic_iterator<_Tp, _Const>(__p)
        {}

        void advance();

        __self &operator++();

        __self operator++(int);

        inline __self operator+(size_t stride) const { return __self(this->__ptr_ + stride); }

        inline __self operator-(size_t stride) const { return __self(this->__ptr_ - stride); }

    };

    template<class _Tp, bool _Const>
    inline
    void
    __forward_iterator<_Tp, _Const>::advance()
    {
        this->__ptr_++;
    }

    template<class _Tp, bool _Const>
    typename __forward_iterator<_Tp, _Const>::__self &
    __forward_iterator<_Tp, _Const>::operator++()
    {
        advance();
        return *this;
    }

    template<class _Tp, bool _Const>
    typename __forward_iterator<_Tp, _Const>::__self
    __forward_iterator<_Tp, _Const>::operator++(int)
    {
        __self result{*this};
        ++(*this);
        return result;
    }

    template<class _Tp, bool _Const>
    class __reverse_iterator : public __basic_iterator<_Tp, _Const> {
    private:

        typedef typename __1::__basic_iterator<_Tp, _Const>::pointer    pointer;
        typedef __reverse_iterator<_Tp, _Const>                         __self;

    public:

        explicit __reverse_iterator(pointer __p)
                : __1::__basic_iterator<_Tp, _Const>(__p)
        {}

        void advance();

        __self &operator++();

        __self operator++(int);

        inline __self operator+(size_t stride) const { return __self(this->__ptr_ - stride); }

        inline __self operator-(size_t stride) const { return __self(this->__ptr_ + stride); }

    };

    template<class _Tp, bool _Const>
    inline
    void
    __reverse_iterator<_Tp, _Const>::advance()
    {
        this->__ptr_--;
    }

    template<class _Tp, bool _Const>
    typename __reverse_iterator<_Tp, _Const>::__self &
    __reverse_iterator<_Tp,  _Const>::operator++()
    {
        advance();
        return *this;
    }

    template<class _Tp, bool _Const>
    typename __reverse_iterator<_Tp, _Const>::__self
    __reverse_iterator<_Tp, _Const>::operator++(int)
    {
        __self result{*this};
        ++(*this);
        return result;
    }

__LANG_SUBSPACE_END

__LANG_NAMESPACE

    template<class _Tp>
    using forward_iterator = __1::__forward_iterator<_Tp, false>;

    template<class _Tp>
    using reverse_iterator = __1::__reverse_iterator<_Tp, false>;

    template<class _Tp>
    using const_forward_iterator = __1::__forward_iterator<_Tp, true>;

    template<class _Tp>
    using const_reverse_iterator = __1::__reverse_iterator<_Tp, true>;

    template<class _Tp>
    using forward_strideable_set = __1::__basic_strideable_set<forward_iterator<_Tp>>;

    template<class _Tp>
    using reverse_strideable_set = __1::__basic_strideable_set<reverse_iterator<_Tp>>;

    template<class _Tp>
    using const_forward_strideable_set = __1::__basic_strideable_set<const_forward_iterator<_Tp>>;

    template<class _Tp>
    using const_reverse_strideable_set = __1::__basic_strideable_set<const_reverse_iterator<_Tp>>;

__LANG_NAMESPACE_END
