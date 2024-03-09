/**\file*********************************************************************
 *                                                                     \brief
 *
 *
 ****************************************************************************
 */

/**\addtogroup LSB_Core
 *@{*/
/**\addtogroup LSB_sys_stat
 *@{*/

#ifndef __LSB_SYS_STAT_VERSION_H__
#define __LSB_SYS_STAT_VERSION_H__ 500
#pragma once

#include "syscall.h"

#define S_IXOTH   01 /**< Выполнение разрешено всем. */
#define S_IWOTH   02 /**< Запись разрешена всем. */
#define S_IROTH   04 /**< Чтение разрешено всем. */
#define S_IRWXO   07 /**< Все права всем. */

#define S_IXGRP   (S_IXOTH << 3) /**< Выполнение разрешено группе */
#define S_IWGRP   (S_IWOTH << 3) /**< Запись разрешена группе */
#define S_IRGRP   (S_IROTH << 3) /**< Чтение разрешено группе */
#define S_IRWXG   (S_IRWXO << 3) /**< Все права группе */

#define S_IXUSR   (S_IXOTH << 6) /**< Выполнение разрешено пользователю. */
#define S_IWUSR   (S_IWOTH << 6) /**< Запись разрешена пользователю. */
#define S_IRUSR   (S_IROTH << 6) /**< Чтение разрешено пользователю. */
#define S_IRWXU   (S_IRWXO << 6) /**< Все права пользователю. */

#ifdef __x86_64

/**
 * См\. \c linux/arch/x86/include/uapi/asm/stat.h
 * TODO: заменить типы действительными для пространства пользователя
 */
struct stat {
   unsigned long  st_dev;
   unsigned long  st_ino;
   unsigned long  st_nlink;

   unsigned int   st_mode;
   unsigned int   st_uid;
   unsigned int   st_gid;
   unsigned int   __pad0;
   unsigned long  st_rdev;
   long           st_size;
   long           st_blksize;
   long           st_blocks;  /**< Количество 512-ти байтных блоков. */

   unsigned long  st_atime;
   unsigned long  st_atime_nsec;
   unsigned long  st_mtime;
   unsigned long  st_mtime_nsec;
   unsigned long  st_ctime;
   unsigned long  st_ctime_nsec;
   unsigned long  __unused[3];
};

#endif

/**
 * Функция fstat.
 *
 * \param   fd
 * \param   statbuf
 *
 * \return  \c 0 при успехе или \c -1 в случае ошибки.
 */
_Sys_function
int fstat(int fd, struct stat *statbuf)
{
   int r = sys_fstat(fd, statbuf);
   return sys_return(r);
}

#endif//__LSB_SYS_STAT_VERSION_H__

/**@} LSB_sys_stat */
/**@} LSB_Core */
