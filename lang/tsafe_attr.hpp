//
// Created by Jeremy S on 2017-12-06.
//

#pragma once

#include "foundation/__base.hpp"
#include "attribute.hpp"

__LANG_SUBSPACE

    struct tsafe_attr {
        static const auto lock_all      = attr_index<0>();
        static const auto lock_back     = attr_index<1>();
        static const auto lock_alloc    = attr_index<2>();
    };

__LANG_SUBSPACE_END
