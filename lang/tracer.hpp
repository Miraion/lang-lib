//
// Created by Jeremy S on 2017-12-08.
//

#pragma once

#include "foundation/__base.hpp"

__LANG_NAMESPACE

    class tracer {
    private:
        static int next_id;

    public:

        int id;

        tracer();

        tracer(tracer const &other);

        tracer(tracer &&other) noexcept;

        virtual ~tracer();

    };

#ifdef __LANG_STD_17
    inline int tracer::next_id = 0;
#endif

__LANG_NAMESPACE_END

#ifdef __LANG_FORCE_INLINE
#include "src/tracer_impl.tcc"
#endif
