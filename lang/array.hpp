//
// Created by Jeremy S on 2017-12-10.
//

#pragma once

#include "foundation/__dym_array.hpp"

__LANG_NAMESPACE

    template<class _Tp, template<class> class _Allocator = std::allocator>
    using array = __1::__dym_array<_Tp, _Allocator>;

__LANG_NAMESPACE_END
