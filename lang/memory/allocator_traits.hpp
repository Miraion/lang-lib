//
// Created by Jeremy S on 2018-01-16.
//

#pragma once

#include "../foundation/__base.hpp"

#define ALLOCATOR_TRAITS(T)                     \
typedef T                   type;               \
typedef type                value_type;         \
typedef value_type*         pointer;            \
typedef value_type const*   const_pointer;      \
typedef value_type&         reference;          \
typedef value_type const&   const_reference;    \
typedef unsigned long       size_type;          \
typedef std::ptrdiff_t      difference_type;    \

#define FORWARD_ALLOCATOR_TRAITS(C)                  \
typedef typename C::value_type      value_type;      \
typedef typename C::pointer         pointer;         \
typedef typename C::const_pointer   const_pointer;   \
typedef typename C::reference       reference;       \
typedef typename C::const_reference const_reference; \
typedef typename C::size_type       size_type;       \
typedef typename C::difference_type difference_type; \

__LANG_NAMESPACE

template<typename T>
struct max_allocations {
    static constexpr std::size_t value = static_cast<std::size_t>(-1) / sizeof(T);
};

__LANG_NAMESPACE_END
