/**\file*********************************************************************
 *                                                                     \brief
 *  Диагностика
 *
 ****************************************************************************
 */

/**\addtogroup C_Library
 *@{*/
/**\addtogroup C_Diagnostics
 *@{*/

/**
 * Макрос определяется в соответствии со значением NDEBUG при каждом включении заголовка.
 */
#undef assert
#ifdef NDEBUG
   #define assert(...) ((void)0)
#elif defined(__CDT_PARSER__)
   #define assert(X) ((void)0)
#else
   #define assert(expr) \
      if ( !!(expr) ) {;} else { \
         static const char emsg[] = "\nУтверждение ("#expr") ложно в функции ";\
         static const char fmsg[] = " (файл "__FILE__", строка ";               \
         __assert_print(emsg, sizeof(emsg) - 1, __func__, sizeof(__func__) - 1,\
                        fmsg, sizeof(fmsg) - 1, __LINE__);  \
      }
#endif

#ifndef __STDC_VERSION_ASSERT_H__
#define __STDC_VERSION_ASSERT_H__ __STDC_VERSION__
#pragma once


#if __STDC_VERSION__ <= 202311L
   #define static_assert _Static_assert
#endif


#ifndef NDEBUG

   #include "../Linux/unistd.h"

   _Noreturn
   void static inline __assert_print(const char * msg, unsigned msg_size,
                                     const char * fn, unsigned fn_size,
                                     const char * file, unsigned file_size,
                                     unsigned line)
   {
      // stdio.h не годится из-за возможных блокировок #stderr.
      write(STDERR_FILENO, msg, msg_size);
      write(STDERR_FILENO, fn, fn_size);
      write(STDERR_FILENO, file, file_size);

      char buf[sizeof(int)*3 + 3];
      char * s = &buf[sizeof(buf)];
      *--s = '\n';
      *--s = '.';
      *--s = ')';
      while (1) {
        *--s = line % 10 + '0';
        if ((line /= 10) == 0)
           break;
      }
      write(STDERR_FILENO, s, &buf[sizeof(buf)] - s);

      sys_exit(0);
      __builtin_trap();
   }

#endif

#endif//__STDC_VERSION_ASSERT_H__

/**@} C_Diagnostics */
/**@} C_Library */
