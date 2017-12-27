//
// Created by Jeremy S on 2017-12-18.
//

#pragma once

#include "../tracer.hpp"
#include "../foundation/__conditional_keyword.hpp"

__LANG_NAMESPACE

    con_inline tracer::tracer()
            : id(tracer::next_id++)
    {
        printf("INIT[%d] DEFAULT\n", id);
    }

    con_inline tracer::tracer(tracer const &other)
            : id(tracer::next_id++)
    {
        printf("INIT[%d] COPY\n", id);
    }

    con_inline tracer::tracer(tracer &&other) noexcept
            : id(tracer::next_id++)
    {
        printf("INIT[%d] MOVE\n", id);
    }

    con_inline tracer::~tracer() {
        printf("DEINIT[%d]\n", id);
    }

__LANG_NAMESPACE_END
