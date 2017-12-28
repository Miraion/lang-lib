//
// Created by Jeremy S on 2017-12-27.
//

#pragma once

#include "__base.hpp"
#include <tuple>

__LANG_SUBSPACE

    template<int...> struct seq {};
    template<int N, int... S> struct gens : gens<N - 1, N - 1, S...> {};
    template<int... S> struct gens<0, S...> { typedef seq<S...> type; };

    template<class Rt, class... Args>
    struct __vfunc {
        std::tuple<Args...> params;
        Rt(*func)(Args...);

        template<int... S>
        Rt __seq_call(seq<S...>) {
            return func(std::get<S>(params)...);
        }

        Rt call() {
            return __seq_call(typename gens<sizeof...(Args)>::type());
        }
    };

__LANG_SUBSPACE_END

__LANG_NAMESPACE

    // Expands a tuple into the pramaters for a given function and calls said function.
    template<class Rt, class... Args>
    inline Rt expand_into(Rt(*func)(Args...), std::tuple<Args...> const &params) {
        __1::__vfunc<Rt, Args...> v{params, func};
        return v.call();
    }

__LANG_NAMESPACE_END
