//
// Created by Jeremy S on 2017-12-08.
//

#include "../foundation/__base.hpp"
#include "../io.hpp"
#include "../optional.hpp"
#include "../tracer.hpp"
#include "../temporal.hpp"

#include "../io/basic_io.tcc"
#include "../io/key_logger.tcc"
#include "../io/ioctl.tcc"
#include "tracer.tcc"

#ifndef __LANG_STD_17
__LANG_NAMESPACE
    const __nil_t nil{};
    key_logger *key_logger::instance = new key_logger{};
    int tracer::next_id = 0;
    const time_t milliseconds = 1000000;
    const time_t centiseconds = 10 * milliseconds;
    const time_t seconds      = 1000 * milliseconds;
    const time_t minutes      = 60 * seconds;
__LANG_NAMESPACE_END
#endif
