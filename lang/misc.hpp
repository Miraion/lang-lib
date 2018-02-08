//
// Created by Jeremy S on 2017-12-29.
//

#pragma once

#include "foundation/__base.hpp"
#include <iostream>

__LANG_SUBSPACE

    template<typename T, typename First>
    inline bool __one_of(T const &base, First const &first) {
        return base == first;
    }

    template<typename T, typename First, typename... Rest>
    inline bool __one_of(T const &base, First const &first, Rest const &... rest) {
        return (base == first) || __one_of(base, rest...);
    }

__LANG_SUBSPACE_END

__LANG_NAMESPACE

    // redefinition of std::__1::__compressed_pair
    template<class _T1, class _T2>
    using compressed_pair = std::__1::__compressed_pair<_T1, _T2>;

    typedef unsigned char byte;

    // compile/run time integer power calculator
    constexpr long pow(long base, unsigned long p) noexcept {
        long a = 1;
        loop until(p) {
            a *= base;
        }
        return a;
    }

    template<class T>
    T abs(T a) noexcept {
        return a < 0 ? -a : a;
    }

    // Creates a shared pointer which will not delete memory upon destruction
    template<class _Tp>
    inline std::shared_ptr<_Tp> make_quiet_ptr(_Tp *__p) {
        return std::shared_ptr<_Tp>{__p, [](auto *){}};
    }

    inline void thread_sleep_milli(size_t n) {
        std::this_thread::sleep_for(std::chrono::nanoseconds{pow(10, 6) * n});
    }

    inline void thread_sleep_nano(size_t n) {
        std::this_thread::sleep_for(std::chrono::nanoseconds{n});
    }

    inline std::string to_2digit_string(size_t x) {
        if (x < 10)
            return "0" + std::to_string(x);
        else
            return std::to_string(x);
    }

    inline std::string to_3digit_string(size_t x) {
        if (x < 10)
            return "00" + std::to_string(x);
        else if (x < 100)
            return "0" + std::to_string(x);
        else
            return std::to_string(x);
    }

    template<typename T, typename... Ts>
    inline bool is_one_of(T const &base, Ts const &... refs) {
        return __1::__one_of(base, refs...);
    }

    /**
     * Moves n instances of T from one memory buffer to another.
     * Undefined behaviour if src and dst buffers overlap.
     *
     * This function does not call any copy constructors, it simply
     * copies bytes. This could cause issues for objects with non
     * trivial copy constructors and should only be used if one buffer
     * will be deallocated without being destoryed.
     */
    template<typename T>
    void copy_memory(T *dst, T const *src, std::size_t n) {
        auto _dst = (byte *) dst;
        auto _src = (byte *) src;
        loop until(sizeof(T) * n) {
            _dst[i] = _src[i];
        }
    }

    /**
     * Similar to the above function, but with an middle man buffer so
     * that the src and dst buffers may overlap.
     */
    template<typename T>
    void move_memory(T *dst, T const *src, std::size_t n) {
        auto _dst = (byte *) dst;
        auto _src = (byte *) src;
        byte buf[n * sizeof(T)];
        loop until(n * sizeof(T)) {
            buf[i] = _src[i];
        }
        loop until(n * sizeof(T)) {
            _dst[i] = buf[i];
        }
    }

__LANG_NAMESPACE_END
