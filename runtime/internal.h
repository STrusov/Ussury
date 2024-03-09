/**\file*********************************************************************
 *                                                                     \brief
 *
 *
 ****************************************************************************
 */

/**\addtogroup Runtime
 *@{*/
/**\addtogroup Rtl_Internal
 *@{*/

#ifndef __STDC_VERSION_RTL_INTERNAL_H__
#define __STDC_VERSION_RTL_INTERNAL_H__ 10
#pragma once

/**
 * Вызывается во время выполнения при нарушении стека.
 *
 * \post
 */
__attribute__((weak))
void __stack_chk_fail(void)
{
   __builtin_trap();
}

#endif//__STDC_VERSION_RTL_INTERNAL_H__

/**@} Rtl_Internal */
/**@} Runtime */
