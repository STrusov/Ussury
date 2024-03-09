/**
 * LSB Core 5.0:
 *
 * Table 7-1. Non Conforming Instructions
 * LAHF        SAHF
 * SYSCALL     SYSRET
 * SYSENTER    SYSEXIT
 * CMPXCHG16B  FFXSR
 *
 * Conforming applications shall not invoke the implementations underlying
 * system call interface directly. The interfaces in the implementation base
 * libraries shall be used instead.
 *
 * Rationale: Implementation-supplied base libraries may use the system call
 * interface but applications must not assume any particular operating system
 * or kernel version is present.
 *
 * Linux kernel, entry_SYSCALL_64:
 *
 * 64-bit SYSCALL instruction entry. Up to 6 arguments in registers.
 * This is the only entry point used for 64-bit system calls.
 *
 * Итак, статическое связывание запрещено для совместимых с LSB библиотек.
 * Потому что... приложение не знает, где его запускают.
 * А кто знает?
 * Сборщики пакетов утверждают, что все приложения должны устанавливаться
 * исключительно из созданного ими репозитория.
 */
