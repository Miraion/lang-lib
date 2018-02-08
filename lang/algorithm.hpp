//
// Created by Jeremy S on 2018-02-06.
//

#pragma once

#include "seq_types/sequence.hpp"
#include "comparison.hpp"

__LANG_SUBSPACE

    template<class Comparison, typename T>
    inline T *__quick_sort_partition(T *b, T *e) {
        T* p = b;
        for (T *q = b; q != e; ++q) {
            if (Comparison::compare(*q, *p)) {
                std::swap(*q, *p);
                ++p;
                std::swap(*q, *p);
            }
        }
        return p;
    }

    template<class Comparison, typename T>
    inline void __quick_sort(T *b, T *e) {
        if (e - b >= 2) {
            T *p = __quick_sort_partition<Comparison, T>(b, e);
            __quick_sort<Comparison, T>(b, p);
            __quick_sort<Comparison, T>(p + 1, e);
        }
    }

__LANG_SUBSPACE_END


__LANG_NAMESPACE

    template<class Comparison = less, typename T, class A>
    inline void quick_sort(basic_sequence<T, A> &seq) {
        __1::__quick_sort<Comparison, T>(seq.begin(), seq.end());
    }

    template<class Comparison = less, typename T>
    inline void quick_sort(T *b, T *e) {
        __1::__quick_sort<Comparison, T>(b, e);
    };

__LANG_NAMESPACE_END
