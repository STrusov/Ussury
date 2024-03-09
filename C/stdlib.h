/**\file*********************************************************************
 *                                                                     \brief
 * Общеполезное
 *
 ****************************************************************************
 */

/**\addtogroup C_Library
 *@{*/
/**\addtogroup C_General_utilities
 *@{*/

#ifndef __STDC_VERSION_STDLIB_H__
#define __STDC_VERSION_STDLIB_H__ __STDC_VERSION__
#pragma once


#ifndef NULL
   #include "stddef.h"
#endif

/**\addtogroup C_exit_status   Статусы завершения.
 *@{
 *
 * Значения передаются аргументами в функцию exit().
 */
#define EXIT_SUCCESS    0  /**< Успешное завершение */
#define EXIT_FAILURE    1  /**< Аварийное завершение */
/**@}*/

/**\addtogroup C_Environment  7.24.4 Взаимодействие с системой.
 *@{*/

/**
 * 7.24.4.4 Функция exit.
 *
 * Аккуратно завершает программу:
 * - Вызывает зарегистрированные atexit() обработчики.
 * - Закрывает открытые потоки ввода-вывода.
 * - Возвращает управление системе.
 *
 * \post    Занимаемые программой ресурсы освобождены.
 *
 * \param   status   EXIT_SUCCESS или EXIT_FAILURE.
 */
_Crt_function
_Noreturn void exit(int status)
{
   // TODO
}

/**
 * 7.24.4.6 Функция getenv.
 *
 * \param   name
 * \return
 */
char *getenv(const char *name);

/**
 * 7.24.4.8 Функция system.
 *
 * Вызывает обработчик команд системы \a string.
 *
 * \param   string   Команда системе или нулевой указатель,
 *                   если требуется определить наличие обработчика команд.
 *
 * \return  Для \a string \c == \c NULL возвращает ненулевое значение,
 *          если система способна исполнять команды.
 */
_Crt_function
int system(const char *string)
{
   // TODO
}

/**@} C_Environment */

// 14.4.65. stdlib.h
extern char **environ;


#endif//__STDC_VERSION_STDLIB_H__

/**@} C_General_utilities */
/**@} C_Library */
