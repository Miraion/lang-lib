//
// Created by Jeremy S on 2017-12-08.
//

#pragma once

/*

        memory_image.hpp synopsis

 namespace lang::__1
 {

 template<class T, template<class> class Allocator>
 class memory_image {
 protected:

     typedef T                              value_type;
     typedef T *                            pointer;
     typedef T &                            reference;
     typedef const T &                      const_reference;
     typedef Allocator<T>                   allocator_type;
     typedef unsigned long                  size_type;
     typedef memory_image<T, Allocator>     self;

 private:

     lang::compressed_pair<pointer, allocator_type> __alloc_pair_;
     size_type __count_;

     pointer &__ptr();
     const pointer &__ptr();
     allocator_type &__alloc();

 public:

     memory_image();
     memory_image(self const &);
     memory_image(self &&) noexcept;
     explicit memory_image(pointer);
     memory_image(pointer, size_type);
     memory_image(pointer, pointer);
     virtual ~memory_image();

     const value_type *operator->() const;
     pointer           operator->();
     const_reference   operator[](size_type) const;
     reference         operator[](size_type);
     const_reference   operator*() const;
     reference         operator*();

     void copy(pointer, size_type);
     void copy(pointer, pointer);
     const value_type *ptr() const;
     pointer mutable_ptr();
     size_type count() const;
     size_type physical_size() const;

 }; // class memory_image<T, Allocator>

 } // namespace lang::__1

 namspace lang
 {

 template<class T, template<class> class Allocator = std::allocator>
 using memory_image = __1::memory_image<T, Allocator>;

 }

 */
#include "foundation/__base.hpp"
#include "misc.hpp"

__LANG_SUBSPACE

    template<class _Tp, template<class> class _Allocator>
    class memory_image {
    protected:

        typedef _Tp                             value_type;
        typedef _Tp *                           pointer;
        typedef _Tp &                           reference;
        typedef const _Tp &                     const_reference;
        typedef _Allocator<_Tp>                 allocator_type;
        typedef unsigned long                   size_type;
        typedef memory_image<_Tp, _Allocator>   __self;

    private:

        compressed_pair<pointer, allocator_type> __alloc_pair_{};

        size_type __count_ = 0;

        inline pointer &__ptr()             { return __alloc_pair_.first();  }
        inline const pointer &__ptr() const { return __alloc_pair_.first();  }
        inline allocator_type &__alloc()    { return __alloc_pair_.second(); }

    public:

        memory_image();

        memory_image(__self const &__a);

        memory_image(__self &&__a) noexcept;

        explicit memory_image(pointer __src);

        // __n is number of elements, NOT number of bytes
        memory_image(pointer __src, size_type __n);

        memory_image(pointer __begin, pointer end);

        ~memory_image();

        const value_type *operator->() const;

        pointer operator->();

        const_reference operator[](size_type __i) const;

        reference operator[](size_type __i);

        inline const_reference operator*() const { return (*this)[0]; }

        inline reference operator*() { return (*this)[0]; }

        // __n is number of elements, NOT number of bytes
        void copy(pointer __src, size_type __n);

        void copy(pointer __begin, pointer __end);

        inline const value_type *ptr() const { return __ptr(); }

        inline pointer mutable_ptr() { return __ptr(); }

        inline size_type count() const { return __count_; }

        inline size_type physical_size() const { return sizeof(value_type) * __count_; }

        inline pointer begin() const { return __ptr(); }

        inline pointer end() const { return __ptr() + __count_; }

    };

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    memory_image<T, A>::memory_image()
    {
        __ptr() = nullptr;
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    memory_image<T, A>::memory_image(__self const &__a)
    {
        this->__ptr() = nullptr;
        this->__count_ = __a.__count_;
        copy(__a.__ptr(), __count_);
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    memory_image<T, A>::memory_image(__self &&__a) noexcept
    {
        this->__ptr() = __a.__ptr();
        this->__count_ = __a.__count_;
        __a.__ptr() = nullptr;
        __a.__count_ = 0;
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    memory_image<T, A>::memory_image(pointer __src)
    {
        this->__ptr() = nullptr;
        this->__count_ = 1;
        copy(__src, 1);
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    memory_image<T, A>::memory_image(pointer __src, size_type __n)
    {
        this->__ptr() = nullptr;
        copy(__src, __n);
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    memory_image<T, A>::memory_image(pointer __being, pointer __end)
    {
        this->__ptr() = nullptr;
        copy(__being, __end);
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    memory_image<T, A>::~memory_image()
    {
        __alloc().deallocate(__ptr(), __count_);
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    const typename memory_image<T, A>::value_type *
    memory_image<T, A>::operator->() const
    {
        return __ptr();
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    typename memory_image<T, A>::pointer
    memory_image<T, A>::operator->()
    {
        return __ptr();
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    typename memory_image<T, A>::const_reference
    memory_image<T, A>::operator[](size_type __i) const
    {
        return __ptr()[__i];
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    typename memory_image<T, A>::reference
    memory_image<T, A>::operator[](size_type __i)
    {
        return __ptr()[__i];
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    void
    memory_image<T, A>::copy(pointer __src, size_type __n)
    {
        if (__ptr() != nullptr)
            __alloc().deallocate(__ptr(), __count_);
        __count_ = __n;
        __ptr() = __alloc().allocate(__n);
        std::memcpy((void *)__ptr(), (void *)__src, physical_size());
    }

    __LANG_ALLOC_TEMPLATE(T, A)
    inline
    void
    memory_image<T, A>::copy(pointer __begin, pointer __end)
    {
        if (__ptr() != nullptr)
            __alloc().deallocate(__ptr(), __count_);
        __count_ = (__end - __begin);
        __ptr() = __alloc().allocate(__count_);
        std::memcpy((void *)__ptr(), (void *)__begin, physical_size());
    }

__LANG_SUBSPACE_END

__LANG_NAMESPACE

    template<class _Tp, template<class> class _Allocator = std::allocator>
    using memory_image = __1::memory_image<_Tp, _Allocator>;

__LANG_NAMESPACE_END
