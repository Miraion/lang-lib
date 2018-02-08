//
// Created by Jeremy S on 2017-12-10.
//

#pragma once

#include "foundation/__dym_array.hpp"

__LANG_NAMESPACE

    template<class _Tp, template<class> class _Allocator = std::allocator>
    class queue : protected __1::__dym_array<_Tp, _Allocator> {
    private:

        typedef __1::__dym_array<_Tp, _Allocator> __base;
        typedef typename __base::size_type      size_type;
        size_type __next_idx_ = 0;

    public:

        typedef typename __base::value_type     value_type;

        using __base::__dym_array;
        using __base::push_back;
        using __base::emplace_back;
        using __base::pop_front;
        using __base::extract_front;
        using __base::begin;
        using __base::end;
        using __base::snapshot;
        using __base::shrink_to_fit;
        using __base::front;
        using __base::size;
        using __base::capacity;
        using __base::physical_size;
        using __base::alloc_size;
        using __base::empty;
        using __base::erase;
        using __base::first;

        optional<value_type> next() noexcept;

        bool operator==(queue<_Tp, _Allocator> const &other) const;

        inline bool operator!=(queue<_Tp, _Allocator> const &other) const
        { return !(*this == other); }

        inline void reset_queue_pos() noexcept
        { __next_idx_ = 0; }

        template<class T, template<class> class A>
        friend std::ostream &operator<<(std::ostream &os, queue<T, A> const &q) {
            os << dynamic_cast<__1::__dym_array<T, A> const &>(q);
            return os;
        }

    };

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    inline
    bool
    queue<_Tp, _Allocator>::operator==(queue<_Tp, _Allocator> const &other) const
    {
        if (other.size() != size())
            return false;
        loop until(size()) {
            if (this->at(i) != other.at(i))
                return false;
        }
        return true;
    }

    __LANG_ALLOC_TEMPLATE(_Tp, _Allocator)
    optional<typename queue<_Tp, _Allocator>::value_type>
    queue<_Tp, _Allocator>::next() noexcept
    {
        lang::optional<value_type> v = this->get(__next_idx_);
        if (v.has_value()) {
            __next_idx_++;
            return v;
        } else {
            return v;
        }
    }

__LANG_NAMESPACE_END
