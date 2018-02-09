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

#define __LANG_MEM_NAMESPACE namespace lang { namespace mem {

#define __LANG_ALLOC_TEMPLATE(T, A) template<class T, template<class> class A>

#define __LANG_OBJECT_TRAITS(T)     \
typedef T           value_type;     \
typedef T *         pointer;        \
typedef T const *   const_pointer;  \
typedef T &         reference;      \
typedef T const &   const_reference;\

#if (_LIBCPP_STD_VER > 14) || (__cplusplus >= 201700L)
#define __LANG_STD_17
#endif

#define loop for
#define until(__n) (size_t i = 0; i < (__n); ++i)
#define loop_until(__v, __n) for (size_t (__v) = 0; (__v) < (__n); ++(__v))
#define loop_range(__v, __s, __e) for (size_t (__v) = (__s); (__v) < (__e); ++(__v))
