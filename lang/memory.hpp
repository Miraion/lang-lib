//
// Created by Jeremy S on 2017-12-27.
//

#pragma once

#include "foundation/__base.hpp"
#include "foundation/__ref_types.hpp"

#include "memory/allocator_traits.hpp"
#include "memory/object_traits.hpp"
#include "memory/heap.hpp"
#include "memory/pool.hpp"
#include "memory/hybrid.hpp"
#include "memory/allocator.hpp"

__LANG_NAMESPACE

    template<typename T>
    using mutable_capture = __1::__ref_mutable_t<T>;

    template<typename T>
    using weak_capture = __1::__ref_weak_t<T>;

    template<typename T>
    inline mutable_capture<T> unsafe_capture(T &__a)
    { return mutable_capture<T>{__a}; }

    template<typename T>
    inline weak_capture<T> safe_capture(T const &__a)
    { return weak_capture<T>{__a}; }

    template<typename T>
    using heap_allocator = allocator<T, mem::heap<T>, object_traits<T>>;

    template<typename T>
    using pool_allocator = allocator<T, mem::pool<T>, object_traits<T>>;

__LANG_NAMESPACE_END
