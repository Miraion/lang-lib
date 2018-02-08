//
// Created by Jeremy S on 2017-12-29.
//

#pragma once

#include "keypad.hpp"
#include "../foundation/__conditional_keyword.hpp"

__LANG_NAMESPACE

//    con_inline
//    void
//    __keypad_t_exec(mutable_capture<lang::queue<int>> q, mutable_capture<int> i)
//    {
//        while (*i == 1) {
//            int c = lang::getc();
//            if (c == 3)  // trigger interrupt flag on ctr-c keyboard input
//                *i = 0;
//            q->push_back(c);
//        }
//    }

    con_inline
    keypad::keypad()
            : chr_queue(),
              interrupt(1),
              thread()
    {}

    con_inline
    void
    keypad::start()
    {
        thread.set_func([](auto q, auto i){
            while ((*i) == 1) {
                int c = lang::getc();
                if (c == 3)
                    *i = 0;
                q->push_back(c);
            }
        });
        thread.capture(&chr_queue, &interrupt);

        chr_queue.erase();
        chr_queue.reset_queue_pos();
        interrupt = 1;
        thread.start();
    }

    con_inline
    void
    keypad::stop()
    {
        interrupt = 0;
        thread.kill();
    }

    con_inline
    void
    keypad::clean()
    {
        chr_queue.erase();
        chr_queue.reset_queue_pos();
    }

    con_inline
    optional<int>
    keypad::get()
    {
        return chr_queue.next();
    }

    con_inline
    bool
    keypad::is_active() const
    {
        return interrupt == 1;
    }

__LANG_NAMESPACE_END
