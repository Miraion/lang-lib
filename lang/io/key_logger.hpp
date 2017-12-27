//
// Created by Jeremy S on 2017-12-19.
//

#pragma once

#include <thread>
#include <unistd.h>
#include <termios.h>

#include "../foundation/__base.hpp"
#include "../optional.hpp"
#include "../queue.hpp"

__LANG_NAMESPACE

    void io_start_key_logger();
    void io_stop_key_logger();
    optional<int> io_key_logger_next();

    enum class key_code : int {
        right_arrow = 4414235,
        down_arrow  = 4348699,
        left_arrow  = 4479771,
        up_arrow    = 4283163,
        eol         = 10,
        esc         = 27,
        tab         = 9,
        backspace   = 127
    };

    class key_logger {
    private:

        pthread_t __thread_{};
        bool __interrupt_ = false;
        queue<int> __c_in_{};

        struct __t_args {
            bool *flag;
            queue<int> *q;
        };

    public:

        static key_logger *instance;
        key_logger() = default;
        void start();
        void stop();
        optional<int> next();
        void clean();
        queue<int> dump() const;
        inline size_t queue_size() const { return __c_in_.size(); }

    };

#ifdef __LANG_STD_17
    inline key_logger *key_logger::instance = new key_logger{};
#endif

__LANG_NAMESPACE_END

#ifdef __LANG_FORCE_INLINE
#include "key_logger.tcc"
#endif
