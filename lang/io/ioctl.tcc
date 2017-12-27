//
// Created by Jeremy S on 2017-12-19.
//

#pragma once

#include "ioctl.hpp"
#include "../foundation/__conditional_keyword.hpp"

#include <unistd.h>
#include <termios.h>

__LANG_NAMESPACE

    con_inline void io_setattr(int attr, bool state) {
        struct termios t{0};
        tcgetattr(STDIN_FILENO, &t);
        if (state)
            t.c_lflag |= attr;
        else
            t.c_lflag &= ~attr;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
    }

    con_inline std::string io_make_esc(std::string const &cmd) {
        return std::string("\033[") + cmd;
    }

    con_inline void io_flush() {
        fflush(stdout);
    }

    con_inline void io_esc(std::string const &cmd) {
        printf("\033[%s", cmd.c_str());
    }

    con_inline void io_hide_cursor() {
        io_esc("?25l");
    }

    con_inline void io_show_cursor() {
        io_esc("?25h");
    }

    con_inline void io_csr_up(size_t n) {
        io_esc(std::to_string(n) + "A");
    }

    con_inline void io_csr_right(size_t n) {
        io_esc(std::to_string(n) + "C");
    }

    con_inline void io_csr_down(size_t n) {
        io_esc(std::to_string(n) + "B");
    }

    con_inline void io_csr_left(size_t n) {
        io_esc(std::to_string(n) + "D");
    }

    con_inline void io_csr_full_left() {
        io_csr_left(1000);
    }

__LANG_NAMESPACE_END
