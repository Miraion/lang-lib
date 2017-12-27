//
// Created by Jeremy S on 2017-12-06.
//

#pragma once

/*

        dym_array.hpp synopsis

        use with c++14 and lower will require library linkage

 namespace lang::__1
 {

 template<class T, template<class> class Allocator>
 class __basic_dym_array {
 protected:

     typedef Allocator<T>               allocator_type;
     typedef T                          value_type;
     typedef T *                        pointer;
     typedef T &                        reference;
     typedef const T &                  const_reference;
     typedef unsigned long              size_type;

     lang::compressed_pair<pointer, allocator_type> __alloc_pair;

     pointer &       __ptr();
     pointer const & __ptr() const;
     allocator_type &__alloc();

     __basic_dym_array();
     virtual ~__basic_dym_array();

     pointer __alloc_new(size_type);
     void __dealloc(pointer, size_type);
     void __alloc_move(pointer &, size_type &);

 }; // class basic_dym_array<T, Allocator>


 template<class T, template<class> class Allocator>
 class __dym_array : protected __basic_dym_array<T, Allocator> {
 protected:

     typedef basic_dym_array<_Tp, _Allocator>       __base;
     typedef attribute_t<64>                        __attr;
     typedef dym_array<_Tp, _Allocator>             __self;

 public:

     typedef typename __base::value_type            value_type;
     typedef typename __base::pointer               pointer;
     typedef typename __base::reference             reference;
     typedef typename __base::const_reference       const_reference;
     typedef typename __base::size_type             size_type;

 private:

     using __base::__ptr;
     using __base::__alloc;
     using __base::__alloc_new;
     using __base::__alloc_move;
     using __base::__dealloc;

     size_type __size_;
     size_type __cap_;

     bool __should_preform_realloc();
     void __move_mem(pointer __dst, pointer __src, size_type __c);

 public:

    // -- constructors and operators -- //
     __dym_array();
     __dym_array(__self const &);
     __dym_array(__self &&) noexcept;
     template<typename...Ts>
     explicit dym_array(Ts...);
     explicit dym_array(size_type);
     ~__dym_array();

     bool operator==(self const &) const;
     bool operator!=(self const &) const;

    // -- element getters -- //
     reference at(size_type);
     const_reference at(size_type) const;
     reference operator[](size_type);
     const_reference operator[](size_type) const;

    // -- element insertion and extraction -- //
     void push_back(const_reference);
     void push_front(const_reference);
     void insert_at(size_type, const_referecne);
     template<class... Args>
     void emplace_back(Args&&...);
     template<class... Args>
     void emplace_front(Args&&...);
     template<class... Args>
     void emplace_at(size_type, Args&&...);
     template<class Array>
     void push_back_contents(Array const &);
     void push_back_contents(__self &&);

     value_type pop_back();
     value_type pop_front();
     value_type pop_at(size_type);
     pointer extract_back();
     pointer extract_front();
     pointer extract_at(size_type);

    // -- memory imaging -- //
     lang::memory_image<value_type> snapshot() const;
     lang::memory_image<value_type> capture_at(size_type) const;

    // -- iterators and stridable sets -- //
     lang::forward_iterator<value_type> begin();
     lang::forward_iterator<value_type> end();
     lang::const_forward_iterator<value_type> begin() const;
     lang::const_forward_iterator<value_type> end() const;
     lang::reverse_iterator<value_type> rbegin();
     lang::reverse_iterator<value_type> rend();
     lang::const_reverse_iterator<value_type> rbegin() const;
     lang::const_reverse_iterator<value_type> rend() const;

     lang::const_forward_strideable_set<value_type> forward() const;
     lang::const_reverse_strideable_set<value_type> reverse() const;
     lang::const_forward_strideable_set<value_type> range(size_type, size_type) const;
     lang::const_reverse_strideable_set<value_type> reverse_range(size_type, size_type) const;

    // -- misc -- //
     void shrink_to_fit();
     void swap(size_type, size_type);
     void sort();
     void sort(bool(*)(const_reference, const_reference));
     void erase();
     reference front();
     reference back();
     const_reference front() const;
     const_reference back() const;
     size_type size() const;
     size_type capacity() const;
     size_type physical_size() const;
     size_type alloc_size() const;
     bool empty() const;

    // -- thread safe, noexcept getters -- //
     lang::optional<value_type> first() const noexcept; // until c++17: requires library linkage
     lang::optional<value_type> last() const noexcept;  // until c++17: requires library linkage
     lang::optional<value_type> get() const noexcept;   // until c++17: requires library linkage

 }; // class dym_array<T, Allocator>

 template<class T, template<class> class Allocator>
 std::ostream &operator<<(std::ostream &, dym_array<T, Allocator> const &);

 } // namespace lang::__1

 */

#include <stdexcept>
#include <ostream>
#include <algorithm>
#include "__base.hpp"
#include "../memory_image.hpp"
#include "../tsafe_attr.hpp"
#include "../iterator.hpp"
#include "../optional.hpp"

__LANG_SUBSPACE

    template<class _Tp, template<class> class _Allocator>
    class __basic_dym_array {
    protected:

        typedef _Allocator<_Tp>         allocator_type;
        typedef _Tp                     value_type;
        typedef _Tp *                   pointer;
        typedef _Tp &                   reference;
        typedef const _Tp &             const_reference;
        typedef unsigned long           size_type;

        lang::compressed_pair<pointer, allocator_type> __alloc_pair{};

        inline pointer &       __ptr()       { return  __alloc_pair.first(); }
        inline pointer const & __ptr() const { return  __alloc_pair.first(); }
        inline allocator_type &__alloc()     { return __alloc_pair.second(); }

        __basic_dym_array() = default;

        pointer __alloc_new(size_type _n);

        void __dealloc(pointer _p, size_type _n);

        void __alloc_move(pointer &_p, size_type &_n);

    };

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __basic_dym_array<_Tp, _Allocator>::pointer
    __basic_dym_array<_Tp, _Allocator>::__alloc_new(size_type _n)
    {
        return __alloc().allocate(_n);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __basic_dym_array<_Tp, _Allocator>::__dealloc(pointer _p, size_type _n)
    {
        __alloc().deallocate(_p, _n);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __basic_dym_array<_Tp, _Allocator>::__alloc_move(pointer &_p, size_type &_n)
    {
        pointer tmp = __alloc_new(2 * _n);
        std::memcpy((void *)tmp, (void *)_p, _n * sizeof(value_type));
        __dealloc(_p, _n);
        _p = tmp;
        _n *= 2;
    }


    template<class _Tp, template<class> class _Allocator>
    class __dym_array : protected __basic_dym_array<_Tp, _Allocator>
    {
    protected:

        typedef __basic_dym_array<_Tp, _Allocator>        __base;
        typedef __dym_array<_Tp, _Allocator>              __self;

    public:

        typedef typename __base::value_type             value_type;
        typedef typename __base::pointer                pointer;
        typedef typename __base::reference              reference;
        typedef typename __base::const_reference        const_reference;
        typedef typename __base::size_type              size_type;

    private:

        using __base::__ptr;
        using __base::__alloc;
        using __base::__alloc_new;
        using __base::__alloc_move;
        using __base::__dealloc;

        size_type __size_;
        size_type __cap_;

        bool __should_preform_realloc();

        void __move_mem(pointer __dst, pointer __src, size_type __c);

    public:

        __dym_array();

        __dym_array(__self const &__a);

        __dym_array(__self&&__a) noexcept;

        template<typename ... _Ts>
        explicit __dym_array(_Ts ... _ts);

        explicit __dym_array(size_type __n);

        ~__dym_array();

        bool operator==(__self const &other) const;

        bool operator!=(__self const &other) const;

        reference at(size_type __i);

        const_reference at(size_type __i) const;

        reference operator[](size_type __i);

        const_reference operator[](size_type __i) const;

        void push_back(const_reference __x);

        template<class... _Args>
        void emplace_back(_Args&&... __args);

        void push_front(const_reference __x);

        template<class... _Args>
        void emplace_front(_Args&&... __args);

        void insert_at(size_type __i, const_reference __x);

        template<class... _Args>
        void emplace_at(size_type __i, _Args&&... __args);

        template<class _Array>
        void push_back_contents(_Array const &__arr);

        void push_back_contents(__self &&__arr);

        value_type pop_back();

        pointer extract_back();

        value_type pop_front();

        pointer extract_front();

        value_type pop_at(size_type __i);

        pointer extract_at(size_type __i);

        lang::memory_image<value_type> snapshot() const;

        lang::memory_image<value_type> capture_at(size_type __i) const;

        void shrink_to_fit();

        void swap(size_type __i, size_type __j);

        void sort();

        void sort(bool(*cmpr)(const_reference, const_reference));

        void erase();

        inline forward_iterator<value_type> begin()
        { return forward_iterator<value_type>{__ptr()}; }

        inline forward_iterator<value_type> end()
        { return forward_iterator<value_type>{__ptr() + __size_}; }

        inline const_forward_iterator<value_type> const_begin() const
        { return const_forward_iterator<value_type>{__ptr()}; }

        inline const_forward_iterator<value_type> const_end() const
        { return const_forward_iterator<value_type>{__ptr() + __size_}; }

        inline reverse_iterator<value_type> rbegin()
        { return reverse_iterator<value_type>{__ptr() + __size_ - 1}; }

        inline reverse_iterator<value_type> rend()
        { return reverse_iterator<value_type>{__ptr() - 1}; }

        inline const_reverse_iterator<value_type> const_rbegin() const
        { return const_reverse_iterator<value_type>{__ptr() + __size_ - 1}; }

        inline const_reverse_iterator<value_type> const_rend() const
        { return const_reverse_iterator<value_type>{__ptr() - 1}; }

        inline const_forward_strideable_set<value_type> forward() const
        { return const_forward_strideable_set<value_type>{const_begin(), const_end()}; }

        inline const_reverse_strideable_set<value_type> reverse() const
        { return const_reverse_strideable_set<value_type>{const_rbegin(), const_rend()}; }

        const_forward_strideable_set<value_type> range(size_type lb, size_type ub) const;

        const_reverse_strideable_set<value_type> reverse_range(size_type lb, size_type ub) const;

        reference front();

        const_reference front() const;

        reference back();

        const_reference back() const;

        lang::optional<value_type> first() const noexcept;

        lang::optional<value_type> last() const noexcept;

        lang::optional<value_type> get(size_type __i) const noexcept;

        inline size_type size() const { return __size_; }

        inline size_type capacity() const { return __cap_; }

        inline size_type physical_size() const { return __size_ * sizeof(value_type); }

        inline size_type alloc_size() const { return __cap_ * sizeof(value_type); }

        inline bool empty() const { return __size_ == 0; }

    };

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    bool
    __dym_array<_Tp, _Allocator>::__should_preform_realloc()
    {
        return __size_ == __cap_;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::__move_mem(pointer __dst, pointer __src, size_type __c)
    {
        std::memmove((void *)__dst, (void *)__src, __c * sizeof(value_type));
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    __dym_array<_Tp, _Allocator>::__dym_array()
            : __size_(0), __cap_(16)
    {
        __ptr() = __alloc_new(16);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    __dym_array<_Tp, _Allocator>::__dym_array(__self const &__a)
            : __size_(__a.__size_), __cap_(__a.__cap_)
    {
        __ptr() = __alloc_new(__cap_);

        loop until(__size_) {
            __ptr()[i] = __a[i];
        }
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    __dym_array<_Tp, _Allocator>::__dym_array(__self &&__a) noexcept
            : __size_(std::move(__a.__size_)),
              __cap_(std::move(__a.__cap_))
    {
        __ptr() = __a.__ptr();
        __a.__ptr() = nullptr;
        __a.__size_ = 0;
        __a.__cap_ = 0;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    template<class ... _Ts>
    inline
    __dym_array<_Tp, _Allocator>::__dym_array(_Ts... _ts)
            : __size_(0), __cap_(16)
    {
        __ptr() = __alloc_new(__cap_);
        value_type arr[] = {static_cast<value_type>(_ts) ...};
        loop until(sizeof(arr) / sizeof(value_type)) {
            this->emplace_back(std::move(arr[i]));
        }
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    __dym_array<_Tp, _Allocator>::__dym_array(size_type __n)
            : __size_(0), __cap_(__n)
    {
        __ptr() = __alloc_new(__cap_);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    __dym_array<_Tp, _Allocator>::~__dym_array()
    {
        loop until(__size_) {
            __alloc().destroy(__ptr() + i);
        }
        __dealloc(__ptr(), __cap_);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    bool
    __dym_array<_Tp, _Allocator>::operator==(__self const &other) const
    {
        if (__size_ != other.__size_)
            return false;
        loop until(__size_) {
            if ((*this)[i] != other[i])
                return false;
        }
        return true;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    bool
    __dym_array<_Tp, _Allocator>::operator!=(__self const &other) const
    {
        return !(*this == other);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::reference
    __dym_array<_Tp, _Allocator>::at(size_type __i)
    {
        if (__i >= __size_)
            throw std::out_of_range("dym_array::at: index out of range");
        return __ptr()[__i];
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::const_reference
    __dym_array<_Tp, _Allocator>::at(size_type __i) const
    {
        if (__i >= __size_)
            throw std::out_of_range("dym_array::at: index out of range");
        return __ptr()[__i];
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::reference
    __dym_array<_Tp, _Allocator>::operator[](size_type __i)
    {
        return __ptr()[__i];
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::const_reference
    __dym_array<_Tp, _Allocator>::operator[](size_type __i) const
    {
        return __ptr()[__i];
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::push_back(const_reference __x)
    {
        if (__should_preform_realloc())
            __alloc_move(__ptr(), __cap_);
        __alloc().construct(&__ptr()[__size_], __x);
        ++__size_;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    template<class... _Args>
    inline
    void
    __dym_array<_Tp, _Allocator>::emplace_back(_Args&&... __args)
    {
        if (__should_preform_realloc())
            __alloc_move(__ptr(), __cap_);
        __alloc().construct(&__ptr()[__size_], std::forward<_Args>(__args)...);
        ++__size_;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::push_front(const_reference __x)
    {
        if (__should_preform_realloc())
            __alloc_move(__ptr(), __cap_);
        __move_mem(__ptr() + 1, __ptr(), __size_);
        __alloc().construct(__ptr(), __x);
        ++__size_;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    template<class... _Args>
    inline
    void
    __dym_array<_Tp, _Allocator>::emplace_front(_Args&&... __args)
    {
        if (__should_preform_realloc())
            __alloc_move(__ptr(), __cap_);
        __move_mem(__ptr() + 1, __ptr(), __size_);
        __alloc().construct(__ptr(), std::forward<_Args>(__args)...);
        ++__size_;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::insert_at(size_type __i, const_reference __x)
    {
        if (__i >= __size_)
            throw std::out_of_range("dym_array::insert_at: index out of range");
        if (__should_preform_realloc())
            __alloc_move(__ptr(), __cap_);
        __move_mem(__ptr() + __i + 1, __ptr() + __i, __size_ - __i);
        __alloc().construct(__ptr() + __i, __x);
        ++__size_;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    template<class... _Args>
    inline
    void
    __dym_array<_Tp, _Allocator>::emplace_at(size_type __i, _Args&&... __args)
    {
        if (__i >= __size_)
            throw std::out_of_range("dym_array::insert_at: index out of range");
        if (__should_preform_realloc())
            __alloc_move(__ptr(), __cap_);
        __move_mem(__ptr() + __i + 1, __ptr() + __i, __size_ - __i);
        __alloc().construct(__ptr() + __i, std::forward<_Args>(__args)...);
        ++__size_;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    template<class _Array>
    inline
    void
    __dym_array<_Tp, _Allocator>::push_back_contents(_Array const &__arr)
    {
        for (value_type x : __arr) {
            emplace_back(std::move(x));
        }
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::push_back_contents(__self &&__arr)
    {
        while(__cap_ < __size_ + __arr.__size_)
            __alloc_move(__ptr(), __cap_);
        __move_mem(end().raw(), __arr.__ptr(), __arr.__size_);
        __size_ += __arr.__size_;
        __arr.__size_ = 0;
        __arr.__cap_ = 0;
        __arr.__ptr() = nullptr;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::value_type
    __dym_array<_Tp, _Allocator>::pop_back()
    {
        value_type x{back()};
        __size_--;
        __alloc().destroy(__ptr() + __size_);
        return x;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::pointer
    __dym_array<_Tp, _Allocator>::extract_back()
    {
        pointer x = __alloc().allocate(1);
        __size_--;
        __move_mem(x, __ptr() + __size_, 1);
        return x;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::value_type
    __dym_array<_Tp, _Allocator>::pop_front()
    {
        value_type x{front()};
        __alloc().destroy(__ptr());
        __size_--;
        __move_mem(__ptr(), __ptr() + 1, __size_);
        return x;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::pointer
    __dym_array<_Tp, _Allocator>::extract_front()
    {
        pointer x = __alloc().allocate(1);
        __size_--;
        __move_mem(x, __ptr(), 1);
        __move_mem(__ptr(), __ptr() + 1, __size_);
        return x;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::value_type
    __dym_array<_Tp, _Allocator>::pop_at(size_type __i)
    {
        if (__i >= __size_ - 1)
            return pop_back();
        value_type x{at(__i)};
        __alloc().destroy(__ptr() + __i);
        __size_--;
        __move_mem(__ptr() + __i, __ptr() + __i + 1, __size_ - __i);
        return x;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::pointer
    __dym_array<_Tp, _Allocator>::extract_at(size_type __i)
    {
        if (__i >= __size_ - 1)
            return extract_back();
        pointer x = __alloc().allocate(1);
        __move_mem(x, __ptr() + __i, 1);
        __size_--;
        __move_mem(__ptr() + __i, __ptr() + __i + 1, __size_ - __i);
        return x;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    lang::memory_image<typename __dym_array<_Tp, _Allocator>::value_type>
    __dym_array<_Tp, _Allocator>::snapshot() const
    {
        return lang::memory_image<value_type>{__ptr(), __ptr() + __size_};
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    lang::memory_image<typename __dym_array<_Tp, _Allocator>::value_type>
    __dym_array<_Tp, _Allocator>::capture_at(size_type __i) const
    {
        if (__i >= __size_)
            throw std::out_of_range("dym_array::capture_at: index out of range");
        return lang::memory_image<value_type>{__ptr() + __i};
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::shrink_to_fit()
    {
        pointer tmp = __alloc().allocate(__size_);
        __move_mem(tmp, __ptr(), __size_);
        __dealloc(__ptr(), __cap_);
        __ptr() = tmp;
        __cap_ = __size_;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::swap(size_type __i, size_type __j)
    {
        std::swap(at(__i), at(__j));
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::sort()
    {
        std::sort(begin(), end());
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::sort(bool(*cmpr)(const_reference, const_reference))
    {
        std::sort(begin(), end(), cmpr);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    void
    __dym_array<_Tp, _Allocator>::erase()
    {
        pointer old = __ptr();
        size_type old_size = __size_;
        size_type old_cap = __cap_;

        __ptr() = __alloc().allocate(16);
        __size_ = 0;
        __cap_ = 16;

        loop until(old_size) {
            __alloc().destroy(old + i);
        }
        __alloc().deallocate(old, old_cap);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    const_forward_strideable_set<typename __dym_array<_Tp, _Allocator>::value_type>
    __dym_array<_Tp, _Allocator>::range(size_type lb, size_type ub) const
    {
        if (ub > __size_)
            throw std::out_of_range("dym_array::range: upper bound out of range");
        if (ub <= lb)
            throw std::out_of_range("dym_array::range: upper bound must be > lower bound");
        return const_forward_strideable_set<value_type>{__ptr() + lb, __ptr() + ub};
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    const_reverse_strideable_set<typename __dym_array<_Tp, _Allocator>::value_type>
    __dym_array<_Tp, _Allocator>::reverse_range(size_type lb, size_type ub) const
    {
        if (ub > __size_)
            throw std::out_of_range("dym_array::range: upper bound out of range");
        if (ub <= lb)
            throw std::out_of_range("dym_array::range: upper bound must be > lower bound");
        return const_reverse_strideable_set<value_type>{__ptr() + ub - 1, __ptr() + lb - 1};
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::reference
    __dym_array<_Tp, _Allocator>::front()
    {
        if (__size_ == 0)
            throw std::out_of_range("dym_array::front: size is zero");
        return *__ptr();
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::const_reference
    __dym_array<_Tp, _Allocator>::front() const
    {
        if (__size_ == 0)
            throw std::out_of_range("dym_array::front: size is zero");
        return *__ptr();
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::reference
    __dym_array<_Tp, _Allocator>::back()
    {
        if (__size_ == 0)
            throw std::out_of_range("dym_array::front: size is zero");
        return *(__ptr() + __size_ - 1);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    typename __dym_array<_Tp, _Allocator>::const_reference
    __dym_array<_Tp, _Allocator>::back() const
    {
        if (__size_ == 0)
            throw std::out_of_range("dym_array::front: size is zero");
        return *(__ptr() + __size_ - 1);
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    lang::optional<typename __dym_array<_Tp, _Allocator>::value_type>
    __dym_array<_Tp, _Allocator>::first() const noexcept
    {
        try {
            return front();
        } catch(...) {
            return nil;
        }
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    lang::optional<typename __dym_array<_Tp, _Allocator>::value_type>
    __dym_array<_Tp, _Allocator>::last() const noexcept
    {
        try {
            return back();
        } catch(...) {
            return nil;
        }
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    lang::optional<typename __dym_array<_Tp, _Allocator>::value_type>
    __dym_array<_Tp, _Allocator>::get(size_type __i) const noexcept
    {
        try {
            return at(__i);
        } catch (...) {
            return nil;
        }
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    std::ostream &
    operator<<(std::ostream &os, __dym_array<_Tp, _Allocator> const &arr)
    {
        os << '[';
        loop until(arr.size()) {
            os << arr[i];
            if (i != arr.size() - 1)
                os << ", ";
        }
        os << ']';
        return os;
    }

__LANG_SUBSPACE_END
