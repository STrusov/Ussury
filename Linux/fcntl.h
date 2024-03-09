/**\file*********************************************************************
 *                                                                     \brief
 *
 *
 ****************************************************************************
 */

/**\addtogroup LSB_Core
 *@{*/
/**\addtogroup LSB_fcntl
 *@{*/

#ifndef __LSB_FCNTL_VERSION_H__
#define __LSB_FCNTL_VERSION_H__ 500
#pragma once


#include "sys/syscall.h"

#define O_RDONLY  00 /**< Только чтение. */
#define O_WRONLY  01 /**< Только запись. */
#define O_RDWR    02 /**< Запись-чтение. */

#define O_CREAT   0100  /**< Если файл не существует, то создаётся. */
#define O_TRUNC   01000 /**< Если файл существует и открывается для записи, то содержимое отсекается. */

#define O_EXCL    0200  /**< Если файл существует, то O_CREAT завершится ошибкой. */
#define O_APPEND  02000 /**< Режим добавления в хвост. */

#ifdef __clang__
/**
 * Функция open.
 *
 * \param   file  Имя открываемого файла.
 * \param   oflag
 *
 * \return  Дескриптор открытого файла.
 */
__attribute__((overloadable))
_Sys_function
__attribute__((overloadable))
int open(const char * file, int oflag)
{
   int r = sys_open(file, oflag, 0);
   return sys_return(r);
}
#endif//#ifdef __clang__

/**
 * Функция open.
 *
 * \param   file  Имя открываемого файла.
 * \param   oflag
 * \param   mode  При O_CREAT определяет права доступа к создаваемому файлу.
 *
 * \return  Дескриптор открытого файла
 */
_Sys_function
int open(const char * file, int oflag, mode_t mode)
{
   int r = sys_open(file, oflag, mode);
   return sys_return(r);
}

#endif//__LSB_FCNTL_VERSION_H__

/**@} LSB_fcntl */
/**@} LSB_Core */
