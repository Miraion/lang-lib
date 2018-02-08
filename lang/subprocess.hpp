//
// Created by Jeremy S on 2018-01-07.
//

#pragma once

#include <cstdio>
#include <memory>
#include <string>
#include "foundation/__base.hpp"
#include "thread.hpp"

__LANG_NAMESPACE

    class subprocess {
    public:

        subprocess();
        subprocess(subprocess const &other);

        explicit subprocess(std::string const &cmd);

        void start();
        bool is_finished() const;
        std::string output() const;
        void join();
        void set_cmd(std::string const &cmd);

    private:

        std::string __cmd_;
        mutating_thread<subprocess> __capture_thread_{};
        std::string __output_ = "";
        bool __is_finished_ = false;
        std::shared_ptr<FILE> __pipe_ = nullptr;

        // executed concurrently by __capture_thread_
        static void cptr(mutable_capture<subprocess> self);

    };

    /*
     * Creates and starts a subprocess with a given command.
     *
     * Equivalent to:
     *      lang::subprocess p{cmd};
     *      p.start();
     *      return p
     */
    subprocess execute(std::string const &cmd);

    /*
     * Creates and starts a subprocess with a given command.
     * Halts the execution of the current program to wait
     * for said subprocess to finished.
     *
     * Returns the output of the subprocess as a string.
     *
     * Equivalent to:
     *      lang::subprocess p{cmd};
     *      p.start();
     *      p.joint();
     *      return p.output();
     */
    std::string execute_inline(std::string const &cmd);

__LANG_NAMESPACE_END

#ifdef __LANG_FORCE_INLINE
#include "src/subprocess.tcc"
#endif
