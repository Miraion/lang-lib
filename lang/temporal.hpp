//
// Created by Jeremy S on 2017-12-19.
//

#pragma once

/*

        temporal.hpp synopsis

 namespace lang
 {

 typedef unsigned long time_t;

 inline constexpr time_t milliseconds;  // since c++17
 inline constexpr time_t centiseconds;  // since c++17
 inline constexpr time_t seconds;       // since c++17
 inline constexpr time_t minutes;       // since c++17

 extern const time_t milliseconds;      // until c++17
 extern const time_t centiseconds;      // until c++17
 extern const time_t seconds;           // until c++17
 extern const time_t minutes;           // until c++17

 template<time_t Ratio>
 class duration {
 private:
     unsigned long __count_;
     template<time_t OtherRatio>
     using precision_rt =
            typename std::conditional<OtherRatio < Ratio, duration<OtherRatio>, duratio<Ratio>::type;
 public:
     static const time_t ratio = Ratio;
     duration() = default;
     duration(duration<Ratio> const &) = default;
     explicit duration(unsigned long);
     unsigned long count() const;
     void set_count(unsigned long);

     template<time_t OtherRatio>
     precision_rt<OtherRatio> operator+(duration<OtherRatio> const &) const;
     template<time_t OtherRatio>
     precision_rt<OtherRatio> operator-(duration<OtherRatio> const &) const;

     // printable time formats
     std::string hh_mm_ss() const;
     std::string hh_mm() const;
     std::string mm_ss() const;
     std::string mm_ss_xx() const;
     std::string mm_ss_xxx() const;
     std::string ss_xx() const;
     std::string ss_xxx() const;
 };

 template<time_t Precision>
 class clock {
 private:
     unsigned long __count_;
     pthread __thread_;
     bool __interrupt_;
     struct __t_attr;
 public:
     clock() = default;
     clock(clock<Precision> const &other) = default;
     void start();
     duration<Precision> stop();
     duration<Precision> lap();
     duration<Precision> get() const;
     void reset();
 };

 }

 */

#include <thread>
#include <string>
#include "foundation/__base.hpp"

__LANG_NAMESPACE

    typedef unsigned long time_t;

#ifdef __LANG_STD_17
    inline constexpr time_t milliseconds = 1000000;
    inline constexpr time_t centiseconds = 10 * milliseconds;
    inline constexpr time_t seconds      = 1000 * milliseconds;
    inline constexpr time_t minutes      = 60 * seconds;
#else
    extern const time_t milliseconds;
    extern const time_t centiseconds;
    extern const time_t seconds;
    extern const time_t minutes;
#endif

    template<time_t Ratio>
    class duration {
    private:
        unsigned long __count_ = 0;

        template<time_t OtherRatio>
        using precision_rt = typename std::conditional<OtherRatio < Ratio,
                duration<OtherRatio>,
                duration<Ratio>>::type;

    public:

        static const time_t ratio = Ratio;

        duration() = default;
        duration(duration<Ratio> const &other) = default;
        explicit duration(unsigned long n);
        inline unsigned long count() const { return __count_; }
        inline void set_count(unsigned long c) { __count_ = c; }

        template<time_t OtherRatio>
        precision_rt<OtherRatio> operator+(duration<OtherRatio> const &other) const;

        template<time_t OtherRatio>
        precision_rt<OtherRatio> operator-(duration<OtherRatio> const &other) const;

        std::string hh_mm_ss() const;
        std::string hh_mm() const;
        std::string mm_ss() const;
        std::string mm_ss_xx() const;
        std::string mm_ss_xxx() const;
        std::string ss_xx() const;
        std::string ss_xxx() const;
    };

    template<time_t CastRatio, time_t BaseRatio>
    inline
    duration<CastRatio>
    duration_cast(duration<BaseRatio> const &base)
    {
        if (BaseRatio > CastRatio)
            return duration<CastRatio>{base.count() * (BaseRatio / CastRatio)};
        else
            return duration<CastRatio>{base.count() / (CastRatio / BaseRatio)};
    }

    template<time_t Ratio>
    inline
    duration<Ratio>::duration(unsigned long n)
            : __count_(n)
    {}

    template<time_t Ratio>
    template<time_t OtherRatio>
    inline
    typename duration<Ratio>::template precision_rt<OtherRatio>
    duration<Ratio>::operator+(duration<OtherRatio> const &other) const
    {
        const time_t r = precision_rt<OtherRatio>::ratio;
        duration<r> result{0};
        result.set_count(duration_cast<r>(*this).count());
        result.set_count(result.count() + duration_cast<r>(other).count());
        return result;
    }

    template<time_t Ratio>
    template<time_t OtherRatio>
    inline
    typename duration<Ratio>::template precision_rt<OtherRatio>
    duration<Ratio>::operator-(duration<OtherRatio> const &other) const
    {
        const time_t r = precision_rt<OtherRatio>::ratio;
        duration<r> result{0};
        result.set_count(duration_cast<r>(*this).count());
        result.set_count(result.count() - duration_cast<r>(other).count());
        return result;
    }

    template<time_t Ratio>
    inline
    std::string
    duration<Ratio>::hh_mm_ss() const
    {
        size_t s = duration_cast<seconds>(*this).count();
        size_t m = s / 60;
        size_t h = m / 60;
        m %= 60;
        s %= 60;
        return to_2digit_string(h) + ":" + to_2digit_string(m) + ":" + to_2digit_string(s);
    }

    template<time_t Ratio>
    inline
    std::string
    duration<Ratio>::hh_mm() const
    {
        size_t m = duration_cast<minutes>(*this).count();
        size_t h = m / 60;
        m %= 60;
        return to_2digit_string(h) + ":" + to_2digit_string(m);
    }

    template<time_t Ratio>
    inline
    std::string
    duration<Ratio>::mm_ss() const
    {
        size_t s = duration_cast<seconds>(*this).count();
        size_t m = s / 60;
        s %= 60;
        return to_2digit_string(m) + ":" + to_2digit_string(s);
    }

    template<time_t Ratio>
    inline
    std::string
    duration<Ratio>::mm_ss_xx() const
    {
        size_t c = duration_cast<centiseconds>(*this).count();
        size_t s = c / 100;
        size_t m = s / 60;
        s %= 60;
        c %= 100;
        return to_2digit_string(m) + ":" + to_2digit_string(s) + ":" + to_2digit_string(c);
    }

    template<time_t Ratio>
    inline
    std::string
    duration<Ratio>::mm_ss_xxx() const
    {
        size_t mi = duration_cast<milliseconds>(*this).count();
        size_t s = mi / 1000;
        size_t m = s / 60;
        s %= 60;
        mi %= 1000;
        return to_2digit_string(m) + ":" + to_2digit_string(s) + ":" + to_3digit_string(mi);
    }

    template<time_t Ratio>
    inline
    std::string
    duration<Ratio>::ss_xx() const
    {
        size_t c = duration_cast<centiseconds>(*this).count();
        size_t s = c / 100;
        c %= 100;
        return to_2digit_string(s) + ":" + to_2digit_string(c);
    }

    template<time_t Ratio>
    inline
    std::string
    duration<Ratio>::ss_xxx() const
    {
        size_t mi = duration_cast<milliseconds>(*this).count();
        size_t s = mi / 1000;
        mi %= 1000;
        return to_2digit_string(s) + ":" + to_3digit_string(mi);
    }

    template<time_t Precision>
    class clock {
    private:

        unsigned long __count_ = 0;
        pthread_t __thread_{};
        bool __interrupt_ = false;

        struct __t_attr {
            unsigned long *c;
            bool *b;
            size_t ratio;
        };

    public:

        clock() = default;
        clock(clock<Precision> const &other) = default;

        void start();
        duration<Precision> stop();
        duration<Precision> lap();
        duration<Precision> get() const;
        inline void reset() { __count_ = 0; }

    };

    template<time_t Precision>
    inline
    void
    clock<Precision>::start()
    {
        __interrupt_ = false;
        size_t ratio = 0;
        // correct for computational time
        if (Precision == milliseconds)
            ratio = 740000;
        else
            ratio = Precision;

        auto t_arg = new __t_attr{&__count_, &__interrupt_, ratio};
        pthread_create(&__thread_, nullptr, [](void *arg) -> void *{
            auto count_ptr = ((__t_attr *) arg)->c;
            auto interrupt = ((__t_attr *) arg)->b;
            auto r = ((__t_attr *) arg)->ratio;
            delete (__t_attr *)arg;

            unsigned long count = 0;
            while (!(*interrupt)) {
                lang::thread_sleep_nano(r);
                ++count;
            }

            *count_ptr += count;
            pthread_exit(nullptr);
        }, t_arg);
    }

    template<time_t Precision>
    inline
    duration<Precision>
    clock<Precision>::stop()
    {
        __interrupt_ = true;
        pthread_join(__thread_, nullptr);
        return get();
    }

    template<time_t Precision>
    inline
    duration<Precision>
    clock<Precision>::get() const
    {
        return duration<Precision>{__count_};
    }

    template<time_t Precision>
    inline
    duration<Precision>
    clock<Precision>::lap()
    {
        stop();
        start();
        return get();
    }

__LANG_NAMESPACE_END
