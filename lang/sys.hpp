//
// Created by Jeremy S on 2018-01-08.
//

#pragma once

#include <string>
#include "foundation/__base.hpp"

__LANG_NAMESPACE

    std::string sys_wd();

__LANG_NAMESPACE_END

#ifdef __LANG_FORCE_INLINE
#include "src/sys.tcc"
#endif
