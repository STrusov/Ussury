/**\file*********************************************************************
 *                                                                     \brief
 *
 *
 ****************************************************************************
 */

/**\addtogroup LSB_Core
 *@{*/
/**\addtogroup LSB_sys_types
 *@{*/

#ifndef __LSB_TYPES_VERSION_H__
#define __LSB_TYPES_VERSION_H__ 500
#pragma once

#include <stdint.h>


typedef int64_t ssize_t;
_Static_assert(sizeof(ssize_t) ==sizeof(size_t));

/**
 * TODO: LSB Core Generic 5.0 определяет этот тип как 32-х разрядный.
 */
typedef int64_t off_t;

typedef unsigned int mode_t;


#endif//__LSB_TYPES_VERSION_H__

/**@} LSB_sys_types */
/**@} LSB_Core */
