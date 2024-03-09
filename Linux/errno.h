/**\file*********************************************************************
 *                                                                     \brief
 *  Ошибки
 *
 ****************************************************************************
 */

/**\addtogroup LSB_Core
 *@{*/
/**\addtogroup LSB_errno
 *@{*/

#ifndef __LSB_ERRNO_VERSION_H__
#define __LSB_ERRNO_VERSION_H__ 500
#pragma once

// TODO потоки
__attribute__((weak))
int __linux_errno;

static inline
int * __errno_location()
{
   return &__linux_errno;
}

#define __linux_errno  (*__errno_location())

#define ENOMEM 12          /**< Недостаточно памяти. */

#define EINVAL 22          /**< Недопустимый аргумент. */

#define ENOSYS 38          /**< Системный вызов не поддерживается. */

extern int *__errno_location(void);

#endif//__LSB_ERRNO_VERSION_H__

/**@} LSB_errno */
/**@} LSB_Core */
