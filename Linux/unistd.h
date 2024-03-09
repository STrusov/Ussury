/**\file*********************************************************************
 *                                                                     \brief
 *
 *
 ****************************************************************************
 */

/**\addtogroup LSB_Core
 *@{*/
/**\addtogroup LSB_unistd
 *@{*/

#ifndef __LSB_UNISTD_VERSION_H__
#define __LSB_UNISTD_VERSION_H__ 500
#pragma once

#include "sys/syscall.h"


#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2


/**
 * Функция close.
 *
 * Закрывает файл.
 *
 * \pre
 * \post
 * \param fd   Описатель файла.
 *
 * \return  \с 0 при успехе, \с -1 при ошибке.
 */
_Sys_function
int close(int fd)
{
   int r = sys_close(fd);
   return sys_return(r);
}


/**
 * Функция read.
 *
 * Читает \a n байт из файла в память по адресу \a buf.
 *
 * \post    При неудаче #errno содержит код ошибки.
 *
 * \param   fd    Описатель файла
 * \param   buf   Начало области памяти, куда читать данные.
 * \param   n     Размер считываемых данных в байтах.
 *
 * \return  Количество считанных байт (может оказаться меньше запрашиваемого)
 *          или -1 в случае ошибки.
 */
_Sys_function
ssize_t read(int fd, void *buf, size_t n)
{
   ssize_t r = sys_read(fd, buf, n);
   return sys_return(r);
}


/**
 * Функция write.
 *
 * Записывает в файл не более \a n байт из буфера.
 *
 * \post    При неудаче errno содержит код ошибки.
 *
 * \param   fd    Описатель файла
 * \param   buf   Начало записываемых данных.
 * \param   n     Размер записываемых данных в байтах.
 *
 * \return  Количество записанных байт, или -1 в случае ошибки.
 */
_Sys_function
ssize_t write(int fd, const void *buf, size_t n)
{
   ssize_t r = sys_write(fd, buf, n);
   return sys_return(r);
}


#endif//__LSB_UNISTD_VERSION_H__

/**@} LSB_unistd */
/**@} LSB_Core */
