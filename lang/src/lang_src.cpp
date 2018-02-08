//
// Created by Jeremy S on 2017-12-08.
//

#include "../foundation/__base.hpp"

// Global const and singleton definitions
#include "../io.hpp"
#include "../optional.hpp"
#include "../tracer.hpp"
#include "../temporal.hpp"
#include "../singleton.hpp"

// Non-template source code
#include "../io/basic_io.tcc"
#include "../io/keypad.tcc"
#include "../io/ioctl.tcc"
#include "tracer.tcc"
#include "subprocess.tcc"
#include "sys.tcc"
#include "parsing.tcc"

// These variables are defined inline in c++17
#ifndef __LANG_STD_17
__LANG_NAMESPACE
    const __nil_t nil{};
    int tracer::next_id = 0;
    const time_t milliseconds = 1000000;
    const time_t centiseconds = 10 * milliseconds;
    const time_t seconds      = 1000 * milliseconds;
    const time_t minutes      = 60 * seconds;

    template<class T>
    typename singleton<T>::type *singleton<T>::__instance_ = nullptr;
__LANG_NAMESPACE_END
#endif
