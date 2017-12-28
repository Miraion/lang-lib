//
// Created by Jeremy S on 2017-12-27.
//

#pragma once

#include <pthread.h>
#include <tuple>
#include "__base.hpp"
#include "__meta.hpp"

__LANG_SUBSPACE

    template<template<class> class CaptureType, class... CaptureObjects>
    class __thread_t {
    public:
        typedef __thread_t<CaptureType, CaptureObjects...>  __self;
        typedef void(*lambda)(CaptureType<CaptureObjects>...);
        typedef std::tuple<CaptureType<CaptureObjects>...>  param_t;

    private:
        pthread_t __thread_{};
        lambda __exec_func_;
        param_t __params_;

        struct __t_args {
            lambda   l;
            param_t *p;
        };

    public:
        __thread_t() = default;
        __thread_t(__self const &other) = default;
        explicit __thread_t(lambda func);

        inline void set_func(lambda func) { __exec_func_ = func; }
        void capture(CaptureObjects *... cpt);
        void start();
        void join();
        void kill();
    };


//    template<template<class> class CaptureType, class... CaptureObjects>
//    inline
//    __thread_t<CaptureType, CaptureObjects...>::__thread_t(__self const &other)
//            : __exec_func_(other.__exec_func_), __params_(other.__params_)
//    {}

    template<template<class> class CaptureType, class... CaptureObjects>
    inline
    __thread_t<CaptureType, CaptureObjects...>::__thread_t(lambda func)
            : __exec_func_(func), __params_()
    {}

    template<template<class> class CaptureType, class... CaptureObjects>
    inline
    void
    __thread_t<CaptureType, CaptureObjects...>::capture(CaptureObjects *... cpt) {
        __params_ = std::make_tuple(CaptureType<CaptureObjects>{cpt}...);
    }

    template<template<class> class CaptureType, class... CaptureObjects>
    inline
    void
    __thread_t<CaptureType, CaptureObjects...>::start(){
        auto arg = new __t_args{__exec_func_, &__params_};
        pthread_create(&__thread_, nullptr, [](void *__a) -> void * {
            auto __arg = (__t_args *)__a;
            auto f = __arg->l;
            auto p = __arg->p;
            delete __arg;
            expand_into(f, *p); // run function
            pthread_exit(nullptr);
        }, (void *)arg);
    }

    template<template<class> class CaptureType, class... CaptureObjects>
    inline
    void
    __thread_t<CaptureType, CaptureObjects...>::join()
    {
        pthread_join(__thread_, nullptr);
    }

    template<template<class> class CaptureType, class... CaptureObjects>
    inline
    void
    __thread_t<CaptureType, CaptureObjects...>::kill()
    {
        pthread_kill(__thread_, 0);
    }

__LANG_SUBSPACE_END
