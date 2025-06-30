/* ISO C9x  7.18  Integer types <stdint.h>
 * Based on ISO/IEC SC22/WG14 9899 Committee draft (SC22 N2794)
 *
 *  THIS SOFTWARE IS NOT COPYRIGHTED
 *
 *  Contributor: Danny Smith <danny_r_smith_2001@yahoo.co.nz>
 *
 *  This source code is offered for use in the public domain. You may
 *  use, modify or distribute it freely.
 *
 *  This code is distributed in the hope that it will be useful but
 *  WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 *  DISCLAIMED. This includes but is not limited to warranties of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  Date: 2000-12-02
 */

#ifndef _STDINT_H
#define _STDINT_H

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifndef _STDINT_HEADER_NOT_SUPPORT
#if defined(_MSC_VER) && (_MSC_VER < 1600)
#define _STDINT_HEADER_NOT_SUPPORT
#elif  defined(__GNUC__) && ((__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 5)))
#define _STDINT_HEADER_NOT_SUPPORT
#else
  /* Other compilers always assume support for <stdint.h>. */
#endif
#endif

/* If the compiler provides the stdint.h header, fall back to the compiler's stdint.h,
 * which might have additional definitions.
 */
#if !defined(_STDINT_HEADER_NOT_SUPPORT)

/* Remove the include guard before including the <stdint.h> header file to ensure it is included. */
#undef _STDINT_H
#include <stdint.h>
#ifndef _STDINT_H
#define _STDINT_H
#endif

#else

/* Avoid duplication with definitions in other stdint.h header files */
#if !definded(_STDINT) && !defined(_GCC_STDINT_H)
#if defined(_MSC_VER)
#define _STDINT
#elif defined(__GNUC__)
#define _GCC_STDINT_H
#endif

#define __need_wint_t
#define __need_wchar_t
#include <stddef.h>

#ifndef _LONGLONG
#if defined(_MSC_VER) && (_MSC_VER < 1800)
#define _LONGLONG __int64
#else
#define _LONGLONG long long
#endif
#endif

#ifndef __GNUC_EXTENSION__
#if defined(__GNUC__)
#define __GNUC_EXTENSION__  __extension__
#else
#define __GNUC_EXTENSION__
#endif
#endif

#if defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#endif

/* 7.18.1.1  Exact-width integer types */
typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef short              int16_t;
typedef unsigned short     uint16_t;
typedef int                int32_t;
typedef unsigned           uint32_t;
__GNUC_EXTENSION__ typedef _LONGLONG          int64_t;
__GNUC_EXTENSION__ typedef unsigned _LONGLONG uint64_t;

/* 7.18.1.2  Minimum-width integer types */
typedef signed char        int_least8_t;
typedef unsigned char      uint_least8_t;
typedef short              int_least16_t;
typedef unsigned short     uint_least16_t;
typedef int                int_least32_t;
typedef unsigned           uint_least32_t;
__GNUC_EXTENSION__ typedef _LONGLONG          int_least64_t;
__GNUC_EXTENSION__ typedef unsigned _LONGLONG uint_least64_t;

/*  7.18.1.3  Fastest minimum-width integer types
 *  Not actually guaranteed to be fastest for all purposes
 *  Here we use the exact-width types for 8 and 16-bit ints.
 */
typedef char               int_fast8_t;
typedef unsigned char      uint_fast8_t;
typedef short              int_fast16_t;
typedef unsigned short     uint_fast16_t;
typedef int                int_fast32_t;
typedef unsigned int       uint_fast32_t;
__GNUC_EXTENSION__ typedef _LONGLONG          int_fast64_t;
__GNUC_EXTENSION__ typedef unsigned _LONGLONG uint_fast64_t;

/* 7.18.1.4  Integer types capable of holding object pointers */
#ifndef _INTPTR_T_DEFINED
#define _INTPTR_T_DEFINED
#if defined(__INTPTR_TYPE__)
__GNUC_EXTENSION__ typedef __INTPTR_TYPE__ intptr_t;
#elif defined(_WIN64) || defined(__x86_64__) || (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 8))
__GNUC_EXTENSION__ typedef _LONGLONG intptr_t;
#else
typedef int intptr_t;
#endif
#endif

#ifndef _UINTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED
#if defined(__UINTPTR_TYPE__)
__GNUC_EXTENSION__ typedef __UINTPTR_TYPE__ uintptr_t;
#elif defined(_WIN64) || defined(__x86_64__) || (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 8))
__GNUC_EXTENSION__ typedef unsigned _LONGLONG uintptr_t;
#else
typedef unsigned int uintptr_t;
#endif
#endif

/* 7.18.1.5  Greatest-width integer types */
__GNUC_EXTENSION__ typedef _LONGLONG          intmax_t;
__GNUC_EXTENSION__ typedef unsigned _LONGLONG uintmax_t;

/* 7.18.2  Limits of specified-width integer types */
#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

/* 7.18.2.1  Limits of exact-width integer types */
#define INT8_MIN  (-128)
#define INT16_MIN (-32768)
#define INT32_MIN (-2147483647 - 1)
#define INT64_MIN (-9223372036854775807LL - 1)

#define INT8_MAX  127
#define INT16_MAX 32767
#define INT32_MAX 2147483647
#define INT64_MAX 9223372036854775807LL

#define UINT8_MAX  0xff                  /* 255U */
#define UINT16_MAX 0xffff                /* 65535U */
#define UINT32_MAX 0xffffffff            /* 4294967295U */
#define UINT64_MAX 0xffffffffffffffffULL /* 18446744073709551615ULL */

/* 7.18.2.2  Limits of minimum-width integer types */
#define INT_LEAST8_MIN  INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INT_LEAST8_MAX  INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

/* 7.18.2.3  Limits of fastest minimum-width integer types */
#define INT_FAST8_MIN  INT8_MIN
#define INT_FAST16_MIN INT16_MIN
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX  INT8_MAX
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MAX INT64_MAX

#define UINT_FAST8_MAX  UINT8_MAX
#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX

/* 7.18.2.4  Limits of integer types capable of holding
    object pointers */
#if defined(_WIN64) || defined(__x86_64__) || (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 8))
#define INTPTR_MIN  INT64_MIN
#define INTPTR_MAX  INT64_MAX
#define UINTPTR_MAX UINT64_MAX
#else
#define INTPTR_MIN  INT32_MIN
#define INTPTR_MAX  INT32_MAX
#define UINTPTR_MAX UINT32_MAX
#endif

    /* 7.18.2.5  Limits of greatest-width integer types */
#define INTMAX_MIN  INT64_MIN
#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

/* 7.18.3  Limits of other integer types */
#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX

#define SIG_ATOMIC_MIN INTPTR_MIN
#define SIG_ATOMIC_MAX INTPTR_MAX

#define SIZE_MAX UINTPTR_MAX

#ifndef WCHAR_MIN /* also in wchar.h */
#define WCHAR_MIN 0
#define WCHAR_MAX 0xffff /* UINT16_MAX */
#endif

/*
 * wint_t is unsigned short for compatibility with MS runtime
 */
#define WINT_MIN 0
#define WINT_MAX 0xffff /* UINT16_MAX */

#endif /* !defined ( __cplusplus) || defined __STDC_LIMIT_MACROS */

 /* 7.18.4  Macros for integer constants */
#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

/* 7.18.4.1  Macros for minimum-width integer constants

    Accoding to Douglas Gwyn <gwyn@arl.mil>:
    "This spec was changed in ISO/IEC 9899:1999 TC1; in ISO/IEC
    9899:1999 as initially published, the expansion was required
    to be an integer constant of precisely matching type, which
    is impossible to accomplish for the shorter types on most
    platforms, because C99 provides no standard way to designate
    an integer constant with width less than that of type int.
    TC1 changed this to require just an integer constant
    *expression* with *promoted* type."
*/

#define INT8_C(val)   ((int8_t) + (val))
#define UINT8_C(val)  ((uint8_t) + (val##U))
#define INT16_C(val)  ((int16_t) + (val))
#define UINT16_C(val) ((uint16_t) + (val##U))

#define INT32_C(val)  val##L
#define UINT32_C(val) val##UL
#define INT64_C(val)  val##LL
#define UINT64_C(val) val##ULL

/* 7.18.4.2  Macros for greatest-width integer constants */
#define INTMAX_C(val)  INT64_C(val)
#define UINTMAX_C(val) UINT64_C(val)
#endif /* !defined ( __cplusplus) || defined __STDC_CONSTANT_MACROS */

#if defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic pop
#endif

#endif /* !definded(_STDINT) && !defined(_GCC_STDINT_H) */
#endif /* !defined(_STDINT_HEADER_NOT_SUPPORT) */
#endif /* _STDINT_H */
