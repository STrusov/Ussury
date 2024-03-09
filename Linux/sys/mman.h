/**\file*********************************************************************
 *                                                                     \brief
 *  Виртуальная память
 *
 ****************************************************************************
 */

/**\addtogroup LSB_Core
 *@{*/
/**\addtogroup LSB_sys_mman
 *@{*/

#ifndef __LSB_SYS_MMAN_VERSION_H__
#define __LSB_SYS_MMAN_VERSION_H__ 500

#pragma once

#include "syscall.h"

#define MAP_FAILED   ((void*)-1)

#define PROT_NONE    0x0   /**< Страницы не доступны.        */
#define PROT_READ    0x1   /**< Страницы доступны на чтение. */
#define PROT_WRITE   0x2   /**< Страницы доступны на запись. */
#define PROT_EXEC    0x4   /**< Страницы с машинным кодом, разрешено исполнение. */

/**
 * Общие страницы отображаются в адресные пространства имеющих доступ процессов.
 * В случае отображения файла, для управляемой записи используется msync().
 * Маскирует #MAP_SYNC и прочие не входящие в #LEGACY_MAP_MASK.
 */
#define MAP_SHARED   0x01

/**
 * Схоже с #MAP_SHARED, но проверяет допустимые для файловой системы флаги.
 * См\. \c file_operations.
 */
#define MAP_SHARED_VALIDATE   0x03

/**
 * При изменении страниц отображения в различных адресных пространствах
 * создаются модифицированные копии (copy-on-write).
 */
#define MAP_PRIVATE  0x02

/**
 * Не округлять адрес вниз по размеру страницы (4К).
 */
#define MAP_FIXED    0x10

/**
 * В отличие от #MAP_FIXED, если создаваемое отображение пересекается с
 * имеющимся, возвращается ошибка #EEXIST.
 */
#define MAP_FIXED_NOREPLACE   0x100000

/**
 * Не использовать именованный файл для создания отображения
 * (то есть будет использована подкачка, но см\. #MAP_NORESERVE).
 * Аргументы должны быть: <tt>fd == -1</tt>, <tt>offset == 0</tt>.
 */
#define MAP_ANONYMOUS   0x20
#define MAP_ANON  MAP_ANONYMOUS

/**
 * Определяет сегмент стека, где часть страниц зарезервирована и подключается
 * ядром при попытке доступа. Механизм защитных страниц достаточно сложен,
 * что бы недвусмысленно описать его в нескольких строчках комментария.
 */
#define MAP_GROWSDOWN   0x0100

#define MAP_DENYWRITE   0x0800   /**< игнорируется ядром */
#define MAP_EXECUTABLE  0x1000   /**< игнорируется ядром */

/**
 * Запретить выгрузку страниц в подкачку.
 * Приводит к #EPERM при нулевом \c RLIMIT_MEMLOCK и отсутствии \c CAP_IPC_LOCK.
 * Приводит к #EAGAIN при выходе за \c RLIMIT_MEMLOCK и отсутствии \c CAP_IPC_LOCK.
 */
#define MAP_LOCKED   0x2000

/**
 * Если разрешен overcommit, не резервируется место в подкачке для
 * отображений с обычным размером страниц памяти.
 * Для файловых отображений с большими страницами не резервируется всегда.
 * Для анонимных отображений с большими страницами игнорируется.
 * Наличие резерва гарантирует отсутствие #SIGSEGV при попытке записи,
 * иначе может не найтись свободной физической страницы.
 * см\. \c do_mmap() в \c linux/mm/mman.c.
 */
#define MAP_NORESERVE   0x4000

/**
 * Функция mmap.
 *
 * \param   addr  Начальный адрес отображения, либо 0 для выбора системой.
 * \param   len   Назмер в байтах.
 * \param   prot
 * \param   flags
 * \param   fd
 * \param   offset
 *
 * Лимит обусловлен возможным созданием coredump в формате ELF.
 * Не актуален для современного формата, но задан как 65530.
 * <tt>vm.max_map_count = DEFAULT_MAX_MAP_COUNT</tt>.
 */
_Sys_function
void* mmap(void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
   void* r = sys_mmap(addr, len, prot, flags, fd, offset);
   return sys_return((long)r);
}


#define MREMAP_MAYMOVE  1
#define MREMAP_FIXED 2

_Sys_function
void* mremap(void *addr, size_t old_len, size_t new_len, int flags, void *new_address)
{
   void* r = sys_mremap(addr, old_len, new_len, flags, new_address);
   return sys_return((long)r);
}

_Sys_function
int munmap(void *addr, size_t len)
{
   int r = sys_munmap(addr, len);
   return sys_return(r);
}


#endif//__LSB_SYS_MMAN_VERSION_H__

/**@} LSB_Errors */
/**@} LSB_Core */
