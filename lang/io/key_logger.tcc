//
// Created by Jeremy S on 2017-12-19.
//

#include "key_logger.hpp"
#include "ioctl.hpp"
#include "../foundation/__conditional_keyword.hpp"

__LANG_NAMESPACE

    con_inline void io_start_key_logger() {
        key_logger::instance->start();
    }

    con_inline void io_stop_key_logger() {
        key_logger::instance->stop();
    }

    con_inline optional<int> io_key_logger_next() {
        return key_logger::instance->next();
    }

    con_inline void key_logger::start() {
        io_setattr(IO_BUFFER | IO_ECHO, false);
        __c_in_.erase();
        auto *args = new __t_args{&__interrupt_, &__c_in_};
        __interrupt_ = false;

        pthread_create(&__thread_, nullptr, [](void *__arg) -> void * {
            auto arg = (__t_args *)__arg;
            auto interrupt = arg->flag;
            auto q = arg->q;
            delete arg;
            while(!(*interrupt)) {
                q->push_back(lang::getc());
            }
            pthread_exit(nullptr);
        }, args);
    }

    con_inline void key_logger::stop() {
        __interrupt_ = true;
        io_setattr(IO_BUFFER | IO_ECHO, true);
        pthread_cancel(__thread_); // if possible, try to remove this
    }

    con_inline optional<int> key_logger::next() {
        return __c_in_.next();
    }

    con_inline void key_logger::clean() {
        __c_in_.erase();
        __c_in_.reset_queue_pos();
    }

    con_inline queue<int> key_logger::dump() const {
        memory_image<int> img = __c_in_.snapshot();
        queue<int> rt{};
        for (int x : img) {
            rt.push_back(x);
        }
        return rt;
    }

__LANG_NAMESPACE_END
