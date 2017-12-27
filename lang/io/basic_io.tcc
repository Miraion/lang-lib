//
// Created by Jeremy S on 2017-12-19.
//

#pragma once

#include "basic_io.hpp"
#include "../foundation/__conditional_keyword.hpp"

#include <unistd.h>
#include <thread>

__LANG_NAMESPACE

    con_inline int getc() {
        int x = 0;
        read(0, &x, 4);
        return x;
    }

__LANG_NAMESPACE_END
