//
// Created by Jeremy S on 2017-12-04.
//

#pragma once

#include <cstdlib>
#include <memory>
#include <thread>
#include <string>

#define __LANG_NAMESPACE namespace lang {
#define __LANG_NAMESPACE_END }

#define __LANG_SUBSPACE namespace lang { namespace __1 {
#define __LANG_SUBSPACE_END }}

#define __LANG_ALLOC_TEMPLATE(T, A) template<class T, template<class> class A>

#if _LIBCPP_STD_VER > 14
#define __LANG_STD_17
#endif

#define loop for
#define until(__n) (size_t i = 0; i < (__n); ++i)
#define loop_until(__v, __n) for (size_t (__v) = 0; (__v) < (__n); ++(__v))
#define loop_range(__v, __s, __e) for (size_t (__v) = (__s); (__v) < (__e); ++(__v))

__LANG_NAMESPACE

    // redefinition of std::__1::__compressed_pair
    template<class _T1, class _T2>
    using compressed_pair = std::__1::__compressed_pair<_T1, _T2>;

    template<class T>
    using default_allocator = std::allocator<T>;

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

__LANG_NAMESPACE_END
