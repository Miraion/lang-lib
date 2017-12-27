//
// Created by Jeremy S on 2017-12-27.
//

#pragma once

#include "foundation/__base.hpp"
#include "foundation/__ref_types.hpp"
#include "foundation/__basic_capture_set.hpp"

__LANG_NAMESPACE

    template<typename T>
    using mutable_capture = __1::__ref_mutable_t<T>;

    template<typename T>
    using weak_capture = __1::__ref_weak_t<T>;

    template<typename T>
    using mutable_capture_set = __1::__basic_capture_set<mutable_capture<T>>;

    template<typename T>
    using weak_capture_set = __1::__basic_capture_set<weak_capture<T>>;

    template<typename T>
    inline mutable_capture<T> unsafe_capture(T &__a)
    { return mutable_capture<T>{__a}; }

    template<typename T>
    inline weak_capture<T> safe_capture(T const &__a)
    { return weak_capture<T>{__a}; }

__LANG_NAMESPACE_END
