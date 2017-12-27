//
// Created by Jeremy S on 2017-12-19.
//

#pragma once

#include "../foundation/__base.hpp"
#include <string>

#define IO_BUFFER ICANON
#define IO_ECHO   ECHO

__LANG_NAMESPACE

    void io_setattr(int attr, bool state);
    std::string io_make_esc(std::string const &cmd);

    void io_flush();
    void io_esc(std::string const &cmd);
    void io_hide_cursor();
    void io_show_cursor();
    void io_csr_up(size_t n);
    void io_csr_right(size_t n);
    void io_csr_down(size_t n);
    void io_csr_left(size_t n);
    void io_csr_full_left();

__LANG_NAMESPACE_END

#ifdef __LANG_FORCE_INLINE
#include "ioctl.hpp"
#endif
