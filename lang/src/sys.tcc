//
// Created by Jeremy S on 2018-01-08.
//

#pragma once

#include "../sys.hpp"
#include "../foundation/__conditional_keyword.hpp"
#include "../subprocess.hpp"
#include "../parsing.hpp"

__LANG_NAMESPACE

    con_inline std::string sys_wd() {
        auto wd = execute_inline("pwd");
        remove_trailing_ws(wd);
        return wd;
    }

__LANG_NAMESPACE_END
