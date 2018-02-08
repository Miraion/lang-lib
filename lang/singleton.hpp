//
// Created by Jeremy S on 2018-01-29.
//

#pragma once

#include "foundation/__base.hpp"

__LANG_NAMESPACE

    template<class T>
    class singleton {
    public:

        typedef T type;

        template<class... Args>
        static void init(Args... args);

        static type &instance();

        static type get() { return instance(); }

    private:

        static type *__instance_;

    };


    template<class T>
    template<class... Args>
    inline
    void
    singleton<T>::init(Args... args)
    {
        delete __instance_;
        __instance_ = new type{std::forward<Args>(args)...};
    }

    template<class T>
    inline
    typename singleton<T>::type &
    singleton<T>::instance()
    {
        if (__instance_) {
            return *__instance_;
        } else {
            __instance_ = new type{};
            return *__instance_;
        }
    }


#ifdef __LANG_STD_17
    template<class T>
    inline typename singleton<T>::type *singleton<T>::__instance_ = nullptr;
#endif

__LANG_NAMESPACE_END
