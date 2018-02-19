//
// Created by Jeremy S on 2018-01-07.
//

#pragma once

#include "../subprocess.hpp"
#include "../foundation/__conditional_keyword.hpp"

__LANG_NAMESPACE

    con_inline subprocess::subprocess()
            : __cmd_("")
    {}

    con_inline subprocess::subprocess(subprocess const &other) {
        this->__cmd_ = other.__cmd_;
        this->__output_ = other.__output_;
        this->__is_finished_ = other.__is_finished_;
        this->__pipe_ = other.__pipe_;
    }

    con_inline subprocess::subprocess(std::string const &cmd)
            : __cmd_(cmd)
    {}

    con_inline void subprocess::start() {
        __pipe_ = std::shared_ptr<FILE>{popen((__cmd_ + " 2>&1").c_str(), "r"), pclose};
        if (!__pipe_)
            throw std::runtime_error("error opening pipe");
        __capture_thread_.set_func(cptr);
        __capture_thread_.capture(this);
        __capture_thread_.start();
    }

    con_inline bool subprocess::is_finished() const {
        return __is_finished_;
    }

    con_inline std::string subprocess::output() const {
        return __output_;
    }

    con_inline void subprocess::join() {
        __capture_thread_.join();
    }

    con_inline void subprocess::set_cmd(std::string const &cmd) {
        __cmd_ = cmd;
    }

    con_inline void subprocess::cptr(mutable_capture<subprocess> self) {
        char buf[128];
        while (!feof(self->__pipe_.get())) {
            if (fgets(buf, 128, self->__pipe_.get()) != nullptr)
                self->__output_.append(buf);
            thread_sleep_milli(1); // to mitigate cpu usage
        }
        self->__is_finished_ = true;
    }

    con_inline subprocess execute(std::string const &cmd) {
        subprocess p{cmd};
        p.start();
        return p;
    }

    con_inline std::string execute_inline(std::string const &cmd) {
        subprocess p{cmd};
        p.start();
        p.join();
        return p.output();
    }

__LANG_NAMESPACE_END
