//
// Created by Jeremy S on 2017-12-31.
//

#pragma once

#include "foundation/__base.hpp"
#include "int_seq.hpp"
#include <exception>
#include <tuple>

#include "io.hpp"

__LANG_NAMESPACE

    template<typename _Fp> class function;  // undefined

    template<typename _Rt, typename... _ArgTypes>
    class function<_Rt(_ArgTypes...)> {
    public:
        typedef _Rt                         return_type;
        typedef std::tuple<_ArgTypes...>    arg_tuple;
        typedef _Rt (*function_type)(_ArgTypes...);

    private:
        function_type __f_;
        arg_tuple *__args_;

    public:
        function();
        function(function<_Rt(_ArgTypes...)> const &__f);
        function(function<_Rt(_ArgTypes...)> &&__f) noexcept;
//        function(function_type __fp);
        template<typename _F>
        function(_F __f);
        ~function();

        function<_Rt(_ArgTypes...)> &operator=(function<_Rt(_ArgTypes...)> const &__f);
        function<_Rt(_ArgTypes...)> &operator=(function<_Rt(_ArgTypes...)> &&__f) noexcept;
        function<_Rt(_ArgTypes...)> &operator=(function_type __f);

        return_type operator()(_ArgTypes... __args) const;
        return_type operator()(arg_tuple const &__args) const;

        void set_args(_ArgTypes... __args);
        void set_args(arg_tuple const &__args);
        void set_func(function_type __f);
        return_type call() const;

        inline function_type as_pointer() const
        { return __f_; }
    };

    template<typename _Rt, typename... _ArgTypes>
    inline
    function<_Rt(_ArgTypes...)>:: function()
            : __f_(nullptr), __args_(nullptr)
    {}

    template<typename _Rt, typename... _ArgTypes>
    inline
    function<_Rt(_ArgTypes...)>::function(function<_Rt(_ArgTypes...)> const &__f)
            : __f_(__f.__f_), __args_(nullptr)
    {
        if (__f.__args_ != nullptr)
            __args_ = new arg_tuple{*__f.__args_};
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    function<_Rt(_ArgTypes...)>::function(function<_Rt(_ArgTypes...)> &&__f) noexcept
            : __f_(__f.__f_), __args_(__f.__args_)
    {
        __f.__args_ = nullptr;
    }

//    template<typename _Rt, typename... _ArgTypes>
//    inline
//    function<_Rt(_ArgTypes...)>::function(function_type __fp)
//            : __f_(__fp), __args_(nullptr)
//    {}

    template<typename _Rt, typename... _ArgTypes>
    template<typename _F>
    inline
    function<_Rt(_ArgTypes...)>::function(_F __f)
            : __f_(__f), __args_(nullptr)
    {}

    template<typename _Rt, typename... _ArgTypes>
    inline
    function<_Rt(_ArgTypes...)>::~function()
    {
        delete __args_;
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    function<_Rt(_ArgTypes...)> &
    function<_Rt(_ArgTypes...)>::operator=(const function<_Rt(_ArgTypes...)> &__f)
    {
        this->__f_ = __f.__f_;
        if (__f.__args_)
            this->__args_ = new arg_tuple{*__f.__args_};
        else
            this->__args_ = nullptr;
        return *this;
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    function<_Rt(_ArgTypes...)> &
    function<_Rt(_ArgTypes...)>::operator=(function<_Rt(_ArgTypes...)> &&__f) noexcept
    {
        this->__f_ = __f.__f_;
        this->__args_ = __f.__args_;
        __f.__args_ = nullptr;
        return *this;
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    function<_Rt(_ArgTypes...)> &
    function<_Rt(_ArgTypes...)>::operator=(function_type __f)
    {
        this->__f_ = __f;
        return *this;
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    typename function<_Rt(_ArgTypes...)>::return_type
    function<_Rt(_ArgTypes...)>::operator()(_ArgTypes... __args) const
    {
        return __f_(__args...);
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    typename function<_Rt(_ArgTypes...)>::return_type
    function<_Rt(_ArgTypes...)>::operator()(arg_tuple const &__args) const
    {
        return expand_into<_Rt, _ArgTypes...>(__f_, __args);
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    void
    function<_Rt(_ArgTypes...)>::set_args(_ArgTypes... __args)
    {
        if (__args_)
            delete __args_;
        __args_ = new std::tuple<_ArgTypes...>{__args...};
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    void
    function<_Rt(_ArgTypes...)>::set_args(arg_tuple const &__args)
    {
        if (__args_)
            delete __args_;
        __args_ = new std::tuple<_ArgTypes...>{__args};
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    void
    function<_Rt(_ArgTypes...)>::set_func(function_type __f)
    {
        __f_ = __f;
    }

    template<typename _Rt, typename... _ArgTypes>
    inline
    typename function<_Rt(_ArgTypes...)>::return_type
    function<_Rt(_ArgTypes...)>::call() const
    {
        if (!__f_)
            throw std::runtime_error("no target function has been supplied");
        if (!__args_)
            throw std::runtime_error("no arguments have been supplied");
        return expand_into<_Rt, _ArgTypes...>(__f_, *__args_);
    }

__LANG_NAMESPACE_END
