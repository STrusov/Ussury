/**\file*********************************************************************
 *                                                                     \brief
 *  Обработка строк
 *
 ****************************************************************************
 */

/**\addtogroup C_Library
 *@{*/
/**\addtogroup C_String
 *@{*/

#ifndef __STDC_VERSION_STRING_H__
#define __STDC_VERSION_STRING_H__ __STDC_VERSION__
#pragma once

// size_t, NULL
#include "stddef.h"

#include "assert.h"

/**\addtogroup C_string_copying  7.26.2 Функции копирования.
 *@{*/

/**
 * 7.26.2.4 Функция strcpy.
 *
 * Копирует массив символов из \a src в \a dst, включая завершающий <tt>'\0'</tt>.
 *
 * \pre     Копируемые объекты не пересекаются.
 *
 * \param   dst   Массив, куда копировать.
 * \param   src   Исходный массив символов.
 *
 * \return  \a dst.
 */
_Crt_function
char *strcpy(char * restrict dst, const char * restrict src)
{
   assert(dst);
   assert(src);

   char * r = dst;
   while ( (*dst++ = *src++) != 0 ) {/**/}
   return r;
}


/**
 * 7.26.2.5 Функция strncpy.
 *
 * Копирует не более \a n символов из адресуемого \a src массива
 * в адресуемый \a dst массив. Символы после <tt>'\0'</tt> не копируются.
 *
 * \pre     Копируемые объекты не пересекаются.
 *
 * \param   dst   Массив, куда копировать.
 * \param   src   Исходный массив символов.
 * \param   n     Максимальное количество копируемых символов.
 *
 * \return  \a dst.
 */
_Crt_function
char * strncpy(char * restrict dst, const char * restrict src, size_t n)
{
   assert(dst || n == 0);
   assert(src || n == 0);

   char * r = dst;
   while (n && (*dst++ = *src++) != 0)
     n--;
   if(n)
     *dst++ = 0;
   return r;
}

/**@} C_string_comparison */

/**\addtogroup C_string_copying  7.26.4 Функции сравнения.
 *@{*/

/**
 * \7.26.4.2 Функция strcmp.
 *
 * Сравнивает строки посимвольно.
 *
 * \param   s1 Строка для сравнения.
 * \param   s2 Строка для сравнения.
 *
 * \return
 *          * 0, если строки равны;
 *          * больше 0, если \a s1 > \a s2;
 *          * меньше 0, если \a s1 < \a s2.
 */
_Crt_function
int strcmp(const char *s1, const char *s2)
{
   assert(s1);
   assert(s2);

   for ( ; ; ++s1, ++s2 )
     if ( !*s1 || *s1 != *s2 )
       return  *(const unsigned char*)(s1) - *(const unsigned char*)(s2);
}

/**@} C_string_comparison */

/**\addtogroup C_string_misc  7.26.6 Прочие функции.
 *@{*/

/**
 * 7.26.6.4 Функция strlen.
 *
 * Подсчитывает количество ненулевых символов (длину строки).
 *
 * \param   s  Строка.
 *
 * \return  Длина строки.
 */
_Crt_function
size_t strlen(const char *s)
{
   assert(s);
   size_t count = 0;
   while ( s[count] ) count++;
   return count;
}

/**@} C_string_misc */


#endif//__STDC_VERSION_STRING_H__

/**@} C_String */
/**@} C_Library */
