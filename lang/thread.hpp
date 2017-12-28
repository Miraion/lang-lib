//
// Created by Jeremy S on 2017-12-27.
//

#pragma once

#include "memory.hpp"
#include "foundation/__thread_t.hpp"

__LANG_NAMESPACE

    template<class... CaptureObjects>
    using safe_thread = __1::__thread_t<weak_capture, CaptureObjects...>;

    template<class... CaptureObjects>
    using mutating_thread = __1::__thread_t<mutable_capture, CaptureObjects...>;

    template<class... CaptureObjects>
    inline
    safe_thread<CaptureObjects...>
    create_thread(typename safe_thread<CaptureObjects...>::lambda func, CaptureObjects *... params)
    {
        safe_thread<CaptureObjects...> t{func};
        t.capture(params...);
        return t;
    }

    template<class... CaptureObjects>
    inline
    mutating_thread<CaptureObjects...>
    create_mutating_thread(typename mutating_thread<CaptureObjects...>::lambda func, CaptureObjects *... params)
    {
        mutating_thread<CaptureObjects...> t{func};
        t.capture(params...);
        return t;
    }

__LANG_NAMESPACE_END
