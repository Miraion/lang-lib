//
// Created by Jeremy S on 2018-02-02.
//

#pragma once

#include <tuple>
#include "foundation/__base.hpp"
#include "seq_types/basic_sequence.hpp"
#include "seq_types/sequence.hpp"
#include "memory.hpp"
#include "comparison.hpp"
#include "operand.hpp"
#include "algorithm.hpp"

__LANG_NAMESPACE

    template<typename T,
            class Comparator = less,
            class Allocator = allocator<T>>
    class heap
            : public basic_sequence<T, Allocator>,
              public equatable<heap<T, Comparator, Allocator>>
    {
    public:

        typedef basic_sequence<T, Allocator>    base;
        typedef heap<T, Comparator, Allocator>  self;
        typedef Comparator comparison_type;
        typedef Allocator  allocator_type;

        __LANG_FORWARD_BASIC_SEQ_TYPES(base)

        heap();

        heap(self const &other);

        heap(self &&other) noexcept;

        template<class A>
        heap(heap<T, Comparator, A> const &other);

        template<class A>
        heap(heap<T, Comparator, A> &&other) noexcept;

        template<typename... Elems>
        heap(Elems const &... elems);

        template<class Seq>
        explicit heap(Seq const &seq);

        self &operator=(self const &other);

        self &operator=(self &&other);

        template<class A>
        self &operator=(basic_sequence<T, A> const &seq);

        bool operator==(self const &other) const override;

        template<typename... Args>
        void insert(Args... args);

        value_type extract();

        value_type replace(const_reference x);

        const_reference peak() const { return __b_[0]; }

        bool empty() const { return this->size() == 0; }

        void linearize() { quick_sort<Comparator>(*this); }

    private:

        using base::__b_;
        using base::__e_;
        using base::alloc_begin;
        using base::alloc_end;
        using base::alloc;

        void expand_back();

        void shuffle_up(size_type idx);
        void shuffle_down();

    public:
        size_type left_child(size_type i) const { return (2 * i) + 1; }
        size_type right_child(size_type i) const { return (2 * i) + 2; }
        size_type parent(size_type i) const { return ((i - 1 - ((i - 1) % 2)) / 2); }

        pointer left_child_ptr(pointer p) const { return __b_ + left_child((size_type) (p - __b_)); }
        pointer right_child_ptr(pointer p) const { return __b_ + right_child((size_type) (p - __b_)); }
        pointer parent_ptr(pointer p) const { return __b_ + parent((size_type) (p - __b_)); }

    };


    template<typename T, class C, class A>
    inline
    heap<T, C, A>::heap()
            : base()
    {
        alloc_begin() = alloc().allocate(16);
        alloc_end() = alloc_begin() + 16;
        __b_ = alloc_begin();
        __e_ = __b_;
    }

    template<typename T, class C, class A>
    inline
    heap<T, C, A>::heap(self const &other)
            : base(other)
    {}

    template<typename T, class C, class A>
    inline
    heap<T, C, A>::heap(self &&other) noexcept
            : base(other)
    {}

    template<typename T, class C, class A1>
    template<class A2>
    inline
    heap<T, C, A1>::heap(heap<T, C, A2> const &other)
            : base(other)
    {}

    template<typename T, class C, class A1>
    template<class A2>
    inline
    heap<T, C, A1>::heap(heap<T, C, A2> &&other) noexcept
            : base(other)
    {}

    template<typename T, class C, class A>
    template<class Seq>
    inline
    heap<T, C, A>::heap(Seq const &seq)
            : base(dynamic_cast<basic_sequence<T> const &>(seq))
    {
        linearize();
    }

    template<typename T, class C, class A>
    template<typename... Elems>
    inline
    heap<T, C, A>::heap(Elems const &... elems)
            : base(sequence<T>{elems...})
    {
        linearize();
    }

    template<typename T, class C, class A>
    inline
    typename heap<T, C, A>::self &
    heap<T, C, A>::operator=(self const &other)
    {
        for (pointer p = __b_; p != __e_; ++p)
            alloc().destroy(p);
        alloc().deallocate(alloc_begin(), this->capacity());
        alloc_begin() = alloc().allocate(other.capacity());
        alloc_end() = alloc_begin() + other.capacity();
        __b_ = alloc_begin();
        __e_ = __b_ + other.size();
        loop until(this->size())
            alloc().construct(__b_ + i, other.data()[i]);
        return *this;
    }

    template<typename T, class C, class A>
    inline
    typename heap<T, C, A>::self &
    heap<T, C, A>::operator=(self &&other)
    {
        for (pointer p = __b_; p != __e_; ++p)
            alloc().destroy(p);
        alloc().deallocate(alloc_begin(), this->capacity());
        alloc_begin() = other.alloc_begin();
        alloc_end() = other.alloc_end();
        __b_ = other.__b_;
        __e_ = other.__e_;
        other.alloc_begin() = nullptr;
        other.alloc_end() = nullptr;
        other.__b_ = nullptr;
        other.__e_ = nullptr;
        return *this;
    }

    template<typename T, class C, class A1>
    template<class A2>
    inline
    typename heap<T, C, A1>::self &
    heap<T, C, A1>::operator=(basic_sequence<T, A2> const &seq)
    {
        for (pointer p = __b_; p != __e_; ++p)
            alloc().destroy(p);
        alloc().deallocate(alloc_begin(), this->capacity());
        alloc_begin() = alloc().allocate(seq.capacity());
        alloc_end() = alloc_begin() + seq.capacity();
        __b_ = alloc_begin();
        __e_ = __b_ + seq.size();
        loop until(this->size())
            alloc().construct(__b_ + i, seq.data()[i]);
        linearize();
        return *this;
    }


    template<typename T, class C, class A>
    template<typename... Args>
    inline
    void
    heap<T, C, A>::insert(Args... args)
    {
        if (__e_ == alloc_end())
            expand_back();
        alloc().construct(__e_, std::forward<Args>(args)...);
        ++__e_;
        shuffle_up(this->size() - 1);
    }

    template<typename T, class C, class A>
    inline
    typename heap<T, C, A>::value_type
    heap<T, C, A>::extract()
    {
        value_type ret{std::move(__b_[0])};
        alloc().destroy(__b_);
        alloc().construct(__b_, std::move(__e_[-1]));
        alloc().destroy(__e_ - 1);
        --__e_;
        shuffle_down();
        return ret;
    }

    template<typename T, class C, class A>
    typename heap<T, C, A>::value_type
    heap<T, C, A>::replace(const_reference x)
    {
        value_type ret{std::move(__b_[0])};
        alloc().destroy(__b_);
        alloc().construct(__b_, x);
        shuffle_down();
        return ret;
    }

    template<typename T, class C, class A>
    inline
    void
    heap<T, C, A>::shuffle_up(size_type idx)
    {
        comparison_type comp{};
        while (idx != 0 && comp(__b_[idx], __b_[parent(idx)])) {
            std::swap(__b_[parent(idx)], __b_[idx]);
            idx = parent(idx);
        }
    }

    template<typename T, class C, class A>
    inline
    void
    heap<T, C, A>::shuffle_down() {
        comparison_type comp{};
        pointer p = __b_;
        pointer c;
        if (right_child_ptr(p) >= __e_) {
            c = left_child_ptr(p);
        } else {
            c = comp(*left_child_ptr(p), *right_child_ptr(p)) ? left_child_ptr(p) : right_child_ptr(p);
        }

        while (c < __e_ && comp(*c, *p)) {
            std::swap(*c, *p);
            p = c;

            if (right_child_ptr(p) >= __e_) {
                c = left_child_ptr(p);
            } else {
                c = comp(*left_child_ptr(p), *right_child_ptr(p)) ? left_child_ptr(p) : right_child_ptr(p);
            }
        }
    }

    template<typename T, class C, class A>
    inline
    void
    heap<T, C, A>::expand_back()
    {
        size_type new_cap = this->capacity() + 16;
        pointer p = alloc().allocate(new_cap);
        lang::copy_memory(p, __b_, this->size());
        alloc().deallocate(__b_, this->capacity());
        __e_ = p + this->size();
        __b_ = p;
        alloc_begin() = __b_;
        alloc_end() = __b_ + new_cap;
    }

    template<typename T, class C, class A>
    inline
    bool heap<T, C, A>::operator==(self const &other) const
    {
        heap<T, C> temp_this{*this};
        temp_this.linearize();

        heap<T, C> temp_other{other};
        temp_other.linearize();
        return dynamic_cast<basic_sequence<T> &>(temp_this) == temp_other;
    }

__LANG_NAMESPACE_END



__LANG_NAMESPACE

    template<typename T, class C, class A1, class A2>
    inline heap<T, C> merge(heap<T, C, A1> const &a, heap<T, C, A2> const &b) {
        throw std::runtime_error("not implemented");
    }

__LANG_NAMESPACE_END
