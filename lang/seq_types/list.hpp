//
// Created by Jeremy S on 2018-01-19.
//

#pragma once

#include "sequence.hpp"
#include "../optional.hpp"
#include "../comparison.hpp"

__LANG_NAMESPACE

    template<class T, class A = lang::allocator<T>>
    class list : public sequence<T, A> {
    private:

        typedef sequence<T, A>  base;
        typedef list<T, A>      self;

        static const unsigned long __front_cap_size = 8;
        static const unsigned long __back_cap_size = 8;
        static const unsigned long __default_capacity = __front_cap_size + __back_cap_size;

        void __expand_back();
        void __expand_front();

    protected:

        using base::__b_;
        using base::__e_;
        using base::alloc_begin;
        using base::alloc_end;
        using base::alloc;

    public:

        using base::size;
        using base::capacity;
        using base::front;
        using base::back;

        __LANG_FORWARD_BASIC_SEQ_TYPES(base)

        /**
         * Default Constructor
         *
         * Capacity is set to 16, with the __b_ being set to alloc_begin() + 8.
         * This means that there is an 8 element pad before and after the initial
         * element.
         */
        list();
        list(self const &l) : base(l) {}
        list(self &&l) noexcept : base(l) {}

        /**
         * Parameter pack construction
         *
         * @tparam Ts : must be castable to template type T
         * @param elems : ordered set of elements to be added to list
         */
        template<typename... Ts>
        explicit list(Ts... elems);

        /**
         * Capacity size construction
         * Explicitly states the starting capacity for this list.
         * @param c : capacity as unsigned long
         */
        explicit list(size_type c, value_type v = value_type{});

        /**
         * Adds an element to the end of the list expanding the capacity
         * at the back if needed.
         *
         * @param x : element to push
         */
        void push_back(const_reference x);

        /**
         * Adds an element to the front of the list expanding the front
         * capacity if needed.
         * This action is equivalent in efficiency to push_back as list
         * is a bi-direcional dynamic data structure.
         *
         * @param x : element to push
         */
        void push_front(const_reference x);

        /**
         * Inserts an element at a given position. All elements after
         * said position are moved back one.
         *
         * @param i : index to place the element at
         * @param x : element to insert
         */
        void insert_at(size_type i, const_reference x);

        /**
         * Provides the same functionality as the above push/insert
         * methods, but constructs the element for given arguments
         * instead of copying an existing reference.
         */
        template<typename... Args>
        void emplace_back(Args... args);
        template<typename... Args>
        void emplace_front(Args... args);
        template<typename... Args>
        void emplace_at(size_type i, Args... args);

        value_type pop_back();
        value_type pop_front();
        value_type remove_at(size_type i);

        optional<value_type> remove_last();
        optional<value_type> remove_first();

        list<value_type> extract_sublist(size_type start_idx, size_type end_idx);
        list<value_type> sublist(size_type start_idx, size_type end_idx) const;

        template<class Comparator = lang::less>
        void sort();

        void shrink_to_fit();
        void swap(size_type i, size_type j);

        inline bool empty() const { return size() == 0; }

        optional<value_type> first() const noexcept;
        optional<value_type> last() const noexcept;
        optional<value_type> get(size_type i) const noexcept;

    };



    // ---------- Constructors ---------- //

    template<class T, class A>
    inline
    list<T, A>::list()
            : base()
    {
        alloc_begin() = alloc().allocate(__default_capacity);
        alloc_end() = alloc_begin() + __default_capacity;
        __b_ = alloc_begin() + __front_cap_size;
        __e_ = __b_;
    }

    template<class T, class A>
    template<typename... Ts>
    inline
    list<T, A>::list(Ts... elems)
            : base()
    {
        size_type _back_cap_size = sizeof...(elems) + __back_cap_size;
        size_type _cap_size = __front_cap_size + _back_cap_size;
        alloc_begin() = alloc().allocate(_cap_size);
        alloc_end() = alloc_begin() + _cap_size;
        __b_ = alloc_begin() + __front_cap_size;
        __e_ = __b_;

        T arr[] = {static_cast<T>(elems)...};
        loop until(sizeof...(elems)) {
            alloc().construct(__e_, std::move(arr[i]));
            ++__e_;
        }
    }

    template<class T, class A>
    inline
    list<T, A>::list(size_type c, value_type v)
            : base(c, v)
    {}



    // ---------- Push / Pop ---------- //

    template<class T, class A>
    inline
    void
    list<T, A>::__expand_back()
    {
        // Allocate new memory.
        size_type new_cap = capacity() + 16;
        pointer p = alloc().allocate(new_cap);

        // Since this is expand_back, the extra space will be added to the end.
        // This means that the begin and end pointers will keep their position
        // relative to the alloc_begin pointer.
        size_type begin_offset = __b_ - alloc_begin();
        size_type end_offset = __e_ - alloc_begin();

        // Copy memory that is in use to the new location.
        lang::copy_memory(p + begin_offset, __b_, size());

        // Deallocate old memory.
        alloc().deallocate(alloc_begin(), capacity());

        // Set the pointers to their new positions.
        alloc_begin() = p;
        alloc_end() = alloc_begin() + new_cap;
        __b_ = alloc_begin() + begin_offset;
        __e_ = alloc_begin() + end_offset;
    }

    template<class T, class A>
    inline
    void
    list<T, A>::__expand_front()
    {
        // Allocate new memory.
        size_type new_cap = capacity() + 16;
        pointer p = alloc().allocate(new_cap);

        // Since this is expand_front, the extra space will be added to the
        // front of the list. As such, the begin and end pointers will
        // retain their relative positions to the alloc_end pointer.
        size_type begin_offset = alloc_end() - __b_;
        size_type end_offset = alloc_end() - __e_;

        // Copy the in-use memory to the new location.
        lang::copy_memory(p + new_cap - begin_offset, __b_, size());

        // Deallocate old memory.
        alloc().deallocate(alloc_begin(), capacity());

        // Set the pointers to their new positions.
        alloc_begin() = p;
        alloc_end() = alloc_begin() + new_cap;
        __b_ = alloc_end() - begin_offset;
        __e_ = alloc_end() - end_offset;
    }

    template<class T, class A>
    inline
    void
    list<T, A>::push_back(const_reference x)
    {
        if (__e_ == alloc_end())
            __expand_back();

        alloc().construct(__e_, x);
        ++__e_;
    }

    template<class T, class A>
    inline
    void
    list<T, A>::push_front(const_reference x) {
        if (__b_ == alloc_begin())
            __expand_front();

        --__b_;
        alloc().construct(__b_, x);
    }

    template<class T, class A>
    inline
    void
    list<T, A>::insert_at(size_type i, const_reference x)
    {
        this->assert_bounds(i);

        if (__e_ == alloc_end())
            __expand_back();
        lang::move_memory(__b_ + i + 1, __b_ + i, size() - i);
        alloc().construct(__b_ + i, x);
        ++__e_;
    }

    template<class T, class A>
    template<typename... Args>
    inline
    void
    list<T, A>::emplace_back(Args... args)
    {
        if (__e_ == alloc_end())
            __expand_back();
        alloc().construct(__e_, std::forward<Args>(args)...);
    }

    template<class T, class A>
    template<typename... Args>
    inline
    void
    list<T, A>::emplace_front(Args... args)
    {
        if (__b_ == alloc_begin())
            __expand_front();
        --__b_;
        alloc().construct(__b_, std::forward<Args>(args)...);
    }

    template<class T, class A>
    template<typename... Args>
    inline
    void
    list<T, A>::emplace_at(size_type i, Args... args)
    {
        this->assert_bounds(i);

        if (__e_ == alloc_end())
            __expand_back();
        lang::move_memory(__b_ + i + 1, __b_ + i, size() - i);
        alloc().construct(__b_ + i, std::forward<Args>(args)...);
        ++__e_;
    }

    template<class T, class A>
    inline
    typename list<T, A>::value_type
    list<T, A>::pop_back()
    {
        value_type x = back();  // back will throw if size == 0
        --__e_;
        alloc().destroy(__e_);
        return x;
    }

    template<class T, class A>
    inline
    typename list<T, A>::value_type
    list<T, A>::pop_front()
    {
        value_type x = front();  // front will throw if size == 0
        alloc().destory(__b_);
        ++__b_;
        return x;
    }



    // ---------- Misc ---------- //

    template<class T, class A>
    inline
    void
    list<T, A>::shrink_to_fit()
    {
        pointer p = alloc().allocate(size());
        lang::copy_memory(p, __b_, size());
        __e_ = p + size();
        __b_ = p;
        alloc().deallocate(alloc_begin(), capacity());
        alloc_begin() = p;
        alloc_end() = __e_;
    }

    template<class T, class A>
    inline
    void
    list<T, A>::swap(size_type i, size_type j)
    {
        value_type temp = at(i);
        (*this)[i] = at(j);
        (*this)[j] = temp;
    }

    template<class T, class A>
    inline
    optional<typename list<T, A>::value_type>
    list<T, A>::first() const noexcept
    {
        try {
            return front();
        } catch(...) {
            return nil;
        }
    }

    template<class T, class A>
    inline
    optional<typename list<T, A>::value_type>
    list<T, A>::last() const noexcept
    {
        try {
            return back();
        } catch(...) {
            return nil;
        }
    }

    template<class T, class A>
    inline
    optional<typename list<T, A>::value_type>
    list<T, A>::get(size_type i) const noexcept
    {
        try {
            return at(i);
        } catch(...) {
            return nil;
        }
    }


__LANG_NAMESPACE_END
