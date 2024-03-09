/**\file*********************************************************************
 *                                                                     \brief
 *  Определения общепринятого (Common definitions)
 *
 ****************************************************************************
 */

/**\addtogroup C_Library
 *@{*/
/**\addtogroup C_Common_definitions
 *@{*/

#ifndef __STDC_VERSION_STDDEF_H__
#define __STDC_VERSION_STDDEF_H__ __STDC_VERSION__
#pragma once


// Успокаиваем анализатор Eclipse CDT
#ifdef __CDT_PARSER__
   #define restrict
   #define _Noreturn
   #define _Static_assert(X)
#endif//__CDT_PARSER__

#define _Crt_function __attribute__((weak))
#define _Crt_variable __attribute__((weak))

#if __STDC_VERSION__ < 202311L
   #define typeof __typeof__
#endif

/**
 * Знаковый тип, соответствующий типу разницы двух указателей.
 */
typedef typeof(((int*)0)-((int*)0))  ptrdiff_t;

_Static_assert((ptrdiff_t)-1 < 0);

/**
 * Тип результата оператора \c sizeof (целое без знака).
 */
typedef typeof(sizeof(int)) size_t;

/**
 * Целочисленный тип, диапазон значений позволяет хранить
 * любой символ поддерживаемых \c locale.
 */
#ifndef __cplusplus
   typedef int wchar_t;
#endif

/**
 * Нулевой указатель.
 */
#if (__STDC_VERSION__ >= 202311L) || (__cplusplus >= 201103L)
   #define NULL nullptr
#else
   #define NULL ((void*)0) // не годится для __cplusplus <= 201103L
#endif

/**
 * Смещение в байтах от начала объекта с типом \a type до его части,
 * адресуемой \a member.
 */
#define offsetof(type, member) __builtin_offsetof(type, member)


#endif//__STDC_VERSION_STDDEF_H__

/**@} C_Common_definitions */
/**@} C_Library */
