//
// Created by Jeremy S on 2017-12-29.
//

#pragma once

#include "basic_io.hpp"
#include "../list.hpp"
#include "../queue.hpp"
#include "../thread.hpp"
#include "../optional.hpp"

__LANG_NAMESPACE

    class keypad {
    private:
        queue<int> chr_queue;
        mutating_thread<queue<int>, int> thread;

    public:
        int interrupt;
        keypad();
        void start();
        void stop();
        void clean();
        optional<int> get();
        bool is_active() const;
        template<typename... Endings>
        _list<int> get_esc_seq(Endings... eds);
    };

    template<typename... Endings>
    inline
    _list<int>
    keypad::get_esc_seq(Endings... eds) {
        _list<int> seq{27};
        bool should_continue = true;
        while (should_continue) {
            auto oc = get();
            if (oc.has_value()) {
                int c = oc.value();
                seq.push_back(c);
                should_continue = !is_one_of(c, eds...);
            } else {
                should_continue = false;
            }
        }
        return seq;
    }

__LANG_NAMESPACE_END

#ifdef __LANG_FORCE_INLINE
#include "keypad.tcc"
#endif
