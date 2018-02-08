//
// Created by Jeremy S on 2018-01-01.
//

#pragma once

#include "foundation/__base.hpp"

__LANG_SUBSPACE

    /*
     * Sequence and Sequence Generation reference from:
     *     https://stackoverflow.com/questions/36612596/tuple-to-parameter-pack
     *
     * Slightly modified from the original.
     */

    template<int...> struct __basic_sequence {};

    template<unsigned int N, int... S>
    struct __seq_generator : __seq_generator<N - 1, N - 1, S...> {};

    template<int... S>
    struct __seq_generator<0, S...> {
        typedef __basic_sequence<S...> type;
    };

    template<typename _Rt, typename... _ArgTypes, int... S>
    _Rt __seq_call(_Rt(*__f)(_ArgTypes...),
                    std::tuple<_ArgTypes...> const &__args,
                    __basic_sequence<S...> seq)
    {
        return __f(std::get<S>(__args)...);
    }

__LANG_SUBSPACE_END

__LANG_NAMESPACE

    template<int... S>
    using int_seq = __1::__basic_sequence<S...>;

    template<unsigned int N>
    using seq_gen = __1::__seq_generator<N>;

    // Expands a tuple into the arguments of a given function
    template<typename _Rt, typename... _ArgTypes>
    inline _Rt expand_into(_Rt(*__f)(_ArgTypes...),
                           std::tuple<_ArgTypes...> const &__args)
    {
        return __1::__seq_call<_Rt, _ArgTypes...>
                (__f, __args, typename seq_gen<sizeof...(_ArgTypes)>::type());
    }

__LANG_NAMESPACE_END
