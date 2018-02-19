//
// Created by Jeremy S on 2018-01-16.
//

#pragma once

#include "../foundation/__base.hpp"
#include "hybrid.hpp"
#include "object_traits.hpp"

__LANG_NAMESPACE

    template<typename T,
             typename PolicyType = mem::hybrid<T>,
             typename TraitsType = object_traits<T>>
    class allocator : public PolicyType, public TraitsType {
    public:

        typedef PolicyType policy;
        typedef TraitsType traits;

        FORWARD_ALLOCATOR_TRAITS(policy)

        template<typename U>
        struct rebind {
            typedef allocator<U,
                    typename policy::template rebind<U>::type,
                    typename traits::template rebind<U>::type> type;
        };

        allocator() = default;

        template<typename U, typename PolicyU, typename TraitsU>
        allocator(allocator<U, PolicyU, TraitsU> const &other)
                : policy(other), traits(other)
        {}

    };


    // Two allocators are not equal unless a specialization says so
    template<typename T, typename PolicyT, typename TraitsT,
            typename U, typename PolicyU, typename TraitsU>
    bool operator==(allocator<T, PolicyT, TraitsT> const& left,
                    allocator<U, PolicyU, TraitsU> const& right)
    {
        return false;
    }

    // Also implement inequality
    template<typename T, typename PolicyT, typename TraitsT,
            typename U, typename PolicyU, typename TraitsU>
    bool operator!=(allocator<T, PolicyT, TraitsT> const& left,
                    allocator<U, PolicyU, TraitsU> const& right)
    {
        return !(left == right);
    }

    // Comparing an allocator to anything else should not show equality
    template<typename T, typename PolicyT, typename TraitsT,
            typename OtherAllocator>
    bool operator==(allocator<T, PolicyT, TraitsT> const& left,
                    OtherAllocator const& right)
    {
        return false;
    }

    // Also implement inequality
    template<typename T, typename PolicyT, typename TraitsT,
            typename OtherAllocator>
    bool operator!=(allocator<T, PolicyT, TraitsT> const& left,
                    OtherAllocator const& right)
    {
        return !(left == right);
    }

    // Specialize for the heap policy
    template<typename T, typename TraitsT,
            typename U, typename TraitsU,
            typename Policy>
    bool operator==(allocator<T, Policy, TraitsT> const& left,
                    allocator<U, Policy, TraitsU> const& right)
    {
        return true;
    }

    // Also implement inequality
    template<typename T, typename TraitsT,
            typename U, typename TraitsU,
            typename Policy>
    bool operator!=(allocator<T, Policy, TraitsT> const& left,
                    allocator<U, Policy, TraitsU> const& right)
    {
        return !(left == right);
    }

__LANG_NAMESPACE_END


