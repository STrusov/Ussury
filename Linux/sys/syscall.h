/**\file*********************************************************************
 *                                                                     \brief
 * Вызов ядра системы
 *
 ****************************************************************************
 */

/**\addtogroup Linux_syscall
 *@{*/

#ifndef __LINUX_SYSCALL_VERSION_H__
#define __LINUX_SYSCALL_VERSION_H__ 660 // версия совместимого ядра Linux
#pragma once

#include <stddef.h>
#include "types.h"
#include "../errno.h"

/**
 * Обработчик инструкции \c syscall в ядре \c entry_SYSCALL_64 ожидает:
 *
 * - \c rax  номер системного вызова
 * - \c rcx  адрес возврата (в пространстве пользователя)
 * - \c r11  копия регистра флагов (по конвенции вызовов регистр не сохраняется вызываемой подпрограммой)
 * - \c rdi  аргумент 0 (нумерация сохранена в соответствии с описанием из \c linux/arch/x86/entry/entry_64.S
 * - \c rsi  аргумент 1
 * - \c rdx  аргумент 2
 * - \c r10  аргумент 3
 * - \c r8   аргумент 4
 * - \c r9   аргумент 5
 * \c rbx, \c rbp, \c r12 ... \c r15  по конвенции вызовов сохраняются вызываемой подпрограммой.
 * Содержимое \c rcx и \c r11 разрушается инструкцией \c syscall.
 *
 * System V Application Binary Interface AMD64 Architecture Processor Supplement
 * определяет конвенцию вызова:
 *
 * - \c rdi  аргумент 1 (0 в ядре)
 * - \c rsi  аргумент 2 (1)
 * - \c rdx  аргумент 3 (2)
 * - \c rcx  аргумент 4 (3)
 * - \c r8   аргумент 5 (4)
 * - \c r9   аргумент 6 (5)
 *
 * Таким образом, если в производящей \c syscall функции сохранить порядок аргументов,
 * потребуются следующее копирования регистров перед вызовом шлюза:
 * - \c rdi => \c rdi
 * - \c rsi => \c rsi
 * - \c rdx => \c rdx
 * - \c rcx => \c r10
 * - \c r8  => \c r8
 * - \c r9  => \c r9
 *
 * То есть всего одно копирование из \c rcx в \c r10. Плюс загрузка \c rax номером
 * для диспетчера - его будем передавать последним аргументом.
 *
 * Такой подход позволит гарантированно избежать лишних инструкций, если функции
 * вызова не будут встроены, а в остальных случаях не полагаться на оптимизатор.
 *
 * Вместо \c rax используем загрузку в \c eax, поскольку возможных номеров
 * существенно меньше 2**32 и происходит беззнаковое расширение до 64-х разрядов.
 *
 * \note Очевидно, что применять макросы вместо функций в данном случае неприемлемо.
 * Кому не очевидно, см\. https://bugzilla.altlinux.org/show_bug.cgi?id=38212#c2
 */
static inline void lnxcall() {}

// TODO Clang 17 генерирует лишние movsxd, хорошо бы исключить b для 32-х разрядных аргументов.
typedef long _Karg_t;

static inline
long lnxcall0(unsigned nr)
{
   register unsigned n __asm__ ("eax") = nr;
   register long result __asm__ ("rax");
   __asm__ volatile (
      "syscall"
      : "=r"(result)
      :
        "r"(n)
      : "rcx", "r11", "memory", "cc"
   );
   return result;
}

static inline
long lnxcall1(_Karg_t ka0, unsigned nr)
{
   register _Karg_t a0 __asm__ ("rdi") = ka0;
   register unsigned n __asm__ ("eax") = nr;
   register long result __asm__ ("rax");
   __asm__ volatile (
      "syscall"
      : "=r"(result)
      : "r"(a0),
        "r"(n)
      : "rcx", "r11", "memory", "cc"
   );
   return result;
}

static inline
long lnxcall2(_Karg_t ka0, _Karg_t ka1, unsigned nr)
{
   register _Karg_t a0 __asm__ ("rdi") = ka0;
   register _Karg_t a1 __asm__ ("rsi") = ka1;
   register unsigned n __asm__ ("eax") = nr;
   register long result __asm__ ("rax");
   __asm__ volatile (
      "syscall"
      : "=r"(result)
      : "r"(a0), "r"(a1),
        "r"(n)
      : "rcx", "r11", "memory", "cc"
   );
   return result;
}

static inline
long lnxcall3(_Karg_t ka0, _Karg_t ka1, _Karg_t ka2, unsigned nr)
{
   register _Karg_t a0 __asm__ ("rdi") = ka0;
   register _Karg_t a1 __asm__ ("rsi") = ka1;
   register _Karg_t a2 __asm__ ("rdx") = ka2;
   register unsigned n __asm__ ("eax") = nr;
   register long result __asm__ ("rax");
   __asm__ volatile (
      "syscall"
      : "=r"(result)
      : "r"(a0), "r"(a1), "r"(a2),
        "r"(n)
      : "rcx", "r11", "memory", "cc"
   );
   return result;
}

static inline
long lnxcall4(_Karg_t ka0, _Karg_t ka1, _Karg_t ka2, _Karg_t ka3, unsigned nr)
{
   register _Karg_t a0 __asm__ ("rdi") = ka0;
   register _Karg_t a1 __asm__ ("rsi") = ka1;
   register _Karg_t a2 __asm__ ("rdx") = ka2;
   register _Karg_t a3 __asm__ ("r10") = ka3;
   register unsigned n __asm__ ("eax") = nr;
   register long result __asm__ ("rax");
   __asm__ volatile (
      "syscall"
      : "=r"(result)
      : "r"(a0), "r"(a1), "r"(a2), "r"(a3),
        "r"(n)
      : "rcx", "r11", "memory", "cc"
   );
   return result;
}

static inline
long lnxcall5(_Karg_t ka0, _Karg_t ka1, _Karg_t ka2, _Karg_t ka3, _Karg_t ka4, unsigned nr)
{
   register _Karg_t a0 __asm__ ("rdi") = ka0;
   register _Karg_t a1 __asm__ ("rsi") = ka1;
   register _Karg_t a2 __asm__ ("rdx") = ka2;
   register _Karg_t a3 __asm__ ("r10") = ka3;
   register _Karg_t a4 __asm__ ("r8")  = ka4;
   register unsigned n __asm__ ("eax") = nr;
   register long result __asm__ ("rax");
   __asm__ volatile (
      "syscall"
      : "=r"(result)
      : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4),
        "r"(n)
      : "rcx", "r11", "memory", "cc"
   );
   return result;
}

static inline
long lnxcall6(_Karg_t ka0, _Karg_t ka1, _Karg_t ka2, _Karg_t ka3, _Karg_t ka4, _Karg_t ka5, unsigned nr)
{
   register _Karg_t a0 __asm__ ("rdi") = ka0;
   register _Karg_t a1 __asm__ ("rsi") = ka1;
   register _Karg_t a2 __asm__ ("rdx") = ka2;
   register _Karg_t a3 __asm__ ("r10") = ka3;
   register _Karg_t a4 __asm__ ("r8")  = ka4;
   register _Karg_t a5 __asm__ ("r9")  = ka5;
   register unsigned n __asm__ ("eax") = nr;
   register long result __asm__ ("rax");
   __asm__ volatile (
      "syscall"
      : "=r"(result)
      : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
        "r"(n)
      : "rcx", "r11", "memory", "cc"
   );
   return result;
}

#define _Sys_function __attribute__((weak)) inline

/**
 * AMD64 ABI, A.2.1/5:
 *
 * Возвращаемое вызовом ядра значение в диапазоне \c -4095 .. \c -1
 * является кодом ошибки, отнятым от нуля, т.е. \c -errno.
 *
 * В \c linux/tools/include/nolibc/sys.h рассматривает как ошибки все отрицательные.
*/
static inline long sys_return(long r)
{
   if (/*-4095 <= r && */ r < 0) {
      __linux_errno = -r;
      return -1;
   }
   return r;
}


#define __NR_read 0

static inline ssize_t sys_read(int fd, void *buf, size_t n)
{
   return lnxcall3(fd, (_Karg_t)buf, n, __NR_read);
}


#define __NR_write 1

static inline
ssize_t sys_write(int fd, const void *buf, size_t n)
{
   return lnxcall3(fd, (_Karg_t)buf, n, __NR_write);
}


#define __NR_open 2

static inline
int sys_open(const char * file, int oflag, mode_t mode)
{
   return lnxcall3((_Karg_t)file, oflag, mode, __NR_open);
}


#define __NR_close 3

static inline
int sys_close(int fd)
{
   return lnxcall1(fd, __NR_close);
}


#define __NR_stat 4


#define __NR_fstat 5

static inline
int sys_fstat(int fd, struct stat *statbuf)
{
   return lnxcall2(fd, (_Karg_t)statbuf, __NR_fstat);
}


#define __NR_lstat 6
#define __NR_poll 7
#define __NR_lseek 8


#define __NR_mmap 9

static inline
void* sys_mmap(void * addr, size_t len, int prot, int flags, int fd, off_t offset)
{
   return (void*)lnxcall6((_Karg_t)addr, len, prot, flags, fd, offset, __NR_mmap);
}


#define __NR_mprotect 10


#define __NR_munmap 11

static inline
int sys_munmap(void *addr, size_t len)
{
   return lnxcall2((_Karg_t)addr, len, __NR_munmap);
}


#define __NR_mremap 25

static inline
void* sys_mremap(void *addr, size_t old_len, size_t new_len, int flags, void *new_address)
{
   return (void*)lnxcall5((_Karg_t)addr, old_len, new_len, flags, (_Karg_t)new_address, __NR_mremap);
}


#define __NR_exit 60

_Noreturn
static inline
void sys_exit(int status)
{
   lnxcall1(status, __NR_exit);
}


#endif//__LINUX_SYSCALL_VERSION_H__

/**@} Linux_syscall */
