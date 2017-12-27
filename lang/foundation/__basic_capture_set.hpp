//
// Created by Jeremy S on 2017-12-27.
//

#pragma once

#include "__base.hpp"
#include "__dym_array.hpp"
#include <memory>

__LANG_SUBSPACE

    template<class CaptureType>
    class __basic_capture_set : __dym_array<CaptureType, default_allocator> {
    public:
        typedef __dym_array<CaptureType, default_allocator> __base;
        typedef __basic_capture_set<CaptureType>            __self;

        typedef CaptureType                                 capture_type;
        typedef typename CaptureType::value_type            value_type;
        typedef typename CaptureType::pointer               pointer;
        typedef typename CaptureType::reference             reference;

        __basic_capture_set();
        __basic_capture_set(__self const &other);
        __basic_capture_set(__self &&other) noexcept;
        template<typename... Args>
        explicit __basic_capture_set(Args&&... args);

        using __base::at;
        using __base::operator[];
        using __base::size;
        using __base::begin;
        using __base::end;
        using __base::erase;

        void capture(reference __r);

    };

    template<class CT>
    inline
    __basic_capture_set<CT>::__basic_capture_set()
            : __base()
    {}

    template<class CT>
    inline
    __basic_capture_set<CT>::__basic_capture_set(__self const &other)
            : __base(dynamic_cast<__base>(other))
    {}

    template<class CT>
    inline
    __basic_capture_set<CT>::__basic_capture_set(__self &&other) noexcept
            : __base(std::move(dynamic_cast<__base>(other)))
    {}

    template<class CT>
    template<typename... Args>
    inline
    __basic_capture_set<CT>::__basic_capture_set(Args&& ...args)
            : __base(capture_type{args}...)
    {}

    template<class CT>
    inline
    void
    __basic_capture_set<CT>::capture(reference __r)
    {
        this->emplace_back(__r);
    }

__LANG_SUBSPACE_END
