/**\file*********************************************************************
 *                                                                     \brief
 * Целочисленные типы (Integer types)
 *
 * Заголовок <stdint.h> объявляет целочисленные типы заданной размерности
 * и определяет соответствующие им макросы. Так же содержит
 * макроопределения размеров целочисленных типов других стандартных заголовков.
 *
 ****************************************************************************
 */

/**\addtogroup C_Library
 *@{*/
/**\addtogroup C_Integer_types
 *@{*/

#ifndef __STDC_VERSION_STDINT_H__
#define __STDC_VERSION_STDINT_H__ __STDC_VERSION__
#pragma once


/**\defgroup Exact-width   7.22.1.1 Целые с точным количеством разрядов
 *
 * Целое содержит ровно столько разрядов, сколько указано в наименовании.
 *
 * Обязательны для платформ, где поддерживаются типы соответствующего размера
 * и знаковый тип хранится в дополнительном коде (то есть значение получается
 * инверсией разрядов с последующим увеличением на 1).
 *
 * С23 разрешает (см. 6.2.6.2/3) только дополнительный код для знаковых.
 *
 *@{*/
typedef signed   char   int8_t;     ///< 8-ми разрядное со знаком.
typedef unsigned char   uint8_t;    ///< 8-ми разрядное без знака.
typedef signed   short  int16_t;    ///< 16-ти разрядное со знаком.
typedef unsigned short  uint16_t;   ///< 16-ти разрядное без знака.
typedef signed   int    int32_t;    ///< 32-х разрядное со знаком.
typedef unsigned int    uint32_t;   ///< 32-х разрядное без знака.
typedef signed   long   int64_t;    ///< 64-х разрядное со знаком.
typedef unsigned long   uint64_t;   ///< 64-х разрядное без знака.
/**@}  */

_Static_assert(sizeof(int8_t) == 1);
_Static_assert(sizeof(int16_t) == 2);
_Static_assert(sizeof(int32_t) == 4);
_Static_assert(sizeof(int64_t) == 8);

/**\defgroup Minimum-width 7.22.1.2 Целые с гарантированным минимумом разрядов
 *
 * Целое содержит разрядов не менее, чем указано в наименовании.
 *
 *@{*/
typedef int8_t    int_least8_t;     ///< Не менее 8-ми разрядов со знаком.
typedef uint8_t   uint_least8_t;    ///< Не менее 8-ми разрядов без знака.
typedef int16_t   int_least16_t;    ///< Не менее 16-ти разрядов со знаком.
typedef uint16_t  uint_least16_t;   ///< Не менее 16-ти разрядов без знака.
typedef int32_t   int_least32_t;    ///< Не менее 32-х разрядов со знаком.
typedef uint32_t  uint_least32_t;   ///< Не менее 32-х разрядов без знака.
typedef int64_t   int_least64_t;    ///< Не менее 64-х разрядов со знаком.
typedef uint64_t  uint_least64_t;   ///< Не менее 64-х разрядов без знака.
/**@}  */

/**\defgroup Fastest-minimum-width 7.22.1.3 Быстрейшие целые с гарантированным минимумом разрядов
 *
 * Данные типы как правило (но не гарантировано для всех случаев)
 * обрабатываются быстрее и содержат разрядов не менее, чем указано
 * в наименовании типа.
 *
 *@{*/
typedef int8_t    int_fast8_t;      ///< Быстрое 8-ми разрядное со знаком.
typedef uint8_t   uint_fast8_t;     ///< Быстрое 8-ми разрядное без знака.
typedef int16_t   int_fast16_t;     ///< Быстрое 16-ти разрядное со знаком.
typedef uint16_t  uint_fast16_t;    ///< Быстрое 16-ти разрядное без знака.
typedef int32_t   int_fast32_t;     ///< Быстрое 32-х разрядное со знаком.
typedef uint32_t  uint_fast32_t;    ///< Быстрое 32-х разрядное без знака.
typedef int64_t   int_fast64_t;     ///< Быстрое 64-х разрядное со знаком.
typedef uint64_t  uint_fast64_t;    ///< Быстрое 64-х разрядное без знака.
/**@}  */

/**\defgroup Pointer-holders 7.22.1.4 Целые, пригодные для хранения указателей.
 *
 * Произвольный указатель на \c void может быть преобразован к данному типу
 * и при обратном преобразовании окажется равен исходному.
 *
 * Стандарт не требует обязательное наличие данных типов.
 *
 *@{*/
typedef int64_t   intptr_t;   ///< Целое со знаком, способное хранить значение указателя.
typedef uint64_t  uintptr_t;  ///< Целое без знака, способное хранить значение указателя.
/**@}  */

_Static_assert(sizeof(intptr_t) == sizeof(void*));
_Static_assert(sizeof(uintptr_t) == sizeof(void*));


/**\defgroup Greatest-width 7.22.1.5 Целые наибольшей разрядности
 *@{*/
typedef int64_t   intmax_t;   ///< Вмещает любое целое со знаком.
typedef uint64_t  uintmax_t;  ///< Вмещает любое целое без знака.
/**@}  */


#endif//__STDC_VERSION_STDINT_H__

/**@} C_Integer_types */
/**@} C_Library */